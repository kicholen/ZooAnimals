#include "MessageCenterFrame.h"
#include "MessageCenterManager.h"
#include "SharedResources.h"
#include "RewardsManager.h"
#include "AddRewardsAction.h"

MessageCenterFrame::MessageCenterFrame() {
	init("LandingPageFrame.xml", false);
	selectTransitions();
}

MessageCenterFrame::~MessageCenterFrame() {

}

void MessageCenterFrame::selectTransitions() {
	spTransition transition = new TransitionMove;
	setTransitionIn(transition);
	setTransitionOut(transition);
}

void MessageCenterFrame::_postHiding(Event *) {
	//FlurryAnalytics::instance.onLevelLeaveEvent(_whichLevel.c_str());
	_view->removeChildren();
	_resources.unload();
}

void MessageCenterFrame::_preShowing(Event *) {
	//FlurryAnalytics::instance.onLevelEnterEvent(_whichLevel.c_str());
	selectTransitions();
	_resources.load();
	setData();
}


Action MessageCenterFrame::loop() {
	while (1) {
		Action action = waitAction();
		if (action.id == "back" || action.id == "_btn_back_") {
			break;
		}
	}

	return _lastAction;
}

void MessageCenterFrame::setData() {
	createBackground();
	createTitle();
	_slidingActor = new SlidingActor();
	_slidingActor->setSize(_view->getWidth() * 0.8f, _view->getHeight() * 0.6f);

	_rectangleContainer = new ColorRectSprite();
	_rectangleContainer->setSize(_slidingActor->getWidth(), _slidingActor->getHeight());
	_rectangleContainer->setColor(Color(123, 24, 32, 0));

	float positionY = _view->getHeight() * 0.3f / 2.0f;
	float itemHeight = 0.0f;

	const std::vector<spMessageModel>& messages = MessageCenterManager::instance.getMessages();

	const Vector2& size = Vector2(_slidingActor->getWidth(), _view->getHeight() * 0.2f);

	for (uint i = 0; i < messages.size(); i++) {
		spMessageItem item = createMessageItem(size, messages[i]);
		item->position = i;
		item->setPosition(_slidingActor->getWidth() / 2.0f, positionY);
		positionY += item->getDerivedHeight() + OFFSET_MESSAGES;
		itemHeight = item->getDerivedHeight();
		_rectangleContainer->addChild(item);
	}

	_rectangleContainer->setSize(_slidingActor->getWidth(), positionY - itemHeight / 2.0f);
	_slidingActor->setContent(_rectangleContainer);
	_slidingActor->setPosition((_view->getWidth() - _slidingActor->getWidth()) / 2.0f, (_view->getHeight() * 1.2f - _slidingActor->getHeight()) / 2.0f);
	_slidingActor->attachTo(_view);
}

spMessageItem MessageCenterFrame::createMessageItem(const Vector2& size, spMessageModel model) {
	spMessageItem item = new MessageItem(size, model);
	item->addEventListener(MessageItem::MessageItemEvent::REWARDS_CLAIMED, CLOSURE(this, &MessageCenterFrame::onRewardClaimed));
	item->addEventListener(MessageItem::MessageItemEvent::MESSAGE_DELETED, CLOSURE(this, &MessageCenterFrame::onMessageDeleted));
	return item;
}

void MessageCenterFrame::createBackground() {
	spBox9Sprite cardBackground = new Box9Sprite;
	cardBackground->setHorizontalMode(Box9Sprite::STRETCHING);
	cardBackground->setVerticalMode(Box9Sprite::STRETCHING);
	cardBackground->setAnchor(0.5f, 0.5f);
	cardBackground->setResAnim(gameResources.getResAnim("colorBox9"));
	cardBackground->setSize(_view->getWidth() * 0.8f, _view->getHeight() * 0.8f);
	cardBackground->setPosition(_view->getWidth() / 2.0f, _view->getHeight() / 2.0f);
	cardBackground->setGuides(9, 20, 9, 15);
	cardBackground->attachTo(_view);
	cardBackground->setPriority(-1);
}

void MessageCenterFrame::createTitle() {
	TextStyle style;
	style.font = gameResources.getResFont("nobile_bold")->getFont();
	style.vAlign = TextStyle::VALIGN_MIDDLE;
	style.hAlign = TextStyle::HALIGN_CENTER;
	style.color = Color(35, 145, 245);

	spTextField text = createTextFieldInBoundries("*TITLE*"/*LanguageManager::instance.getText(lockitId)*/, Vector2(_view->getWidth(), _view->getHeight() * 0.2f), style);
	text->setPosition(0.0f, _view->getHeight() * 0.1f);
	_view->addChild(text);

	spSprite iconSprite = new Sprite();
	iconSprite->setResAnim(gameResources.getResAnim("mail"));
	setSpriteScaleBySize(iconSprite, Vector2(_view->getHeight() * 0.2f, _view->getHeight() * 0.2f));
	iconSprite->setPosition(_view->getWidth() * 0.1f, _view->getHeight() * 0.1f);
	iconSprite->attachTo(_view);

	spTweenButton quitButton = new TweenButton();
	quitButton->setAnchor(0.5f, 0.5f);
	quitButton->setTouchChildrenEnabled(false);
	quitButton->setName("back");
	handleClick(quitButton);
	quitButton->setResAnim(gameResources.getResAnim("quitButton"));
	setSpriteScaleBySize(quitButton, Vector2(_view->getHeight() * 0.1f, _view->getHeight() * 0.1f));
	quitButton->setPosition(_view->getWidth() * 0.9f - quitButton->getDerivedWidth() / 2.0f, _view->getHeight() * 0.1f + quitButton->getDerivedHeight() / 2.0f);
	quitButton->attachTo(_view);
}

void MessageCenterFrame::onRewardClaimed(Event *event) {
	MessageItem::MessageItemEvent *messageEvent = safeCast<MessageItem::MessageItemEvent*>(event);
	spMessageItem item = safeSpCast<MessageItem>(event->currentTarget);
	int position = item->position;
	int height = item->getDerivedHeight();
	item->detach();
	revalidate(position, height);
	getRoot()->addChild(new AddRewardsAction(messageEvent->model->getRewards()));
	MessageCenterManager::instance.removeMessage(messageEvent->model);
}

void MessageCenterFrame::onMessageDeleted(Event *event) {
	MessageItem::MessageItemEvent *messageEvent = safeCast<MessageItem::MessageItemEvent*>(event);
	spMessageItem item = safeSpCast<MessageItem>(event->currentTarget);
	int position = item->position;
	int height = item->getDerivedHeight();
	item->detach();
	revalidate(position, height);
}

void MessageCenterFrame::revalidate(int removedPosition, float height) {
	spActor child = _rectangleContainer->getFirstChild();
	bool wasAnimated = false;
	bool wasCallbackAdded = false;

	while (child) {
		spMessageItem item = safeSpCast<MessageItem>(child);
		
		if (item->position > removedPosition) {
			item->position -= 1;
			if (!wasCallbackAdded) {
				item->addTween(Actor::TweenY(item->getY() - height - OFFSET_MESSAGES), 400)->addDoneCallback(CLOSURE(this, &MessageCenterFrame::onRevalidateFinished));
			}
			else {
				item->addTween(Actor::TweenY(item->getY() - height - OFFSET_MESSAGES), 400);
			}
			wasAnimated = true;
		}

		child = child->getNextSibling();
	}
	if (wasAnimated) {
		_view->setTouchChildrenEnabled(false);
		_view->setTouchEnabled(false);
	}
	_rectangleContainer->setHeight(_rectangleContainer->getHeight() - height - OFFSET_MESSAGES);
	_rectangleContainer->addRef();
	_slidingActor->setContent(_rectangleContainer);
	_rectangleContainer->releaseRef();
}

void MessageCenterFrame::onRevalidateFinished(Event *event) {
	_view->setTouchChildrenEnabled(true);
	_view->setTouchEnabled(true);
}