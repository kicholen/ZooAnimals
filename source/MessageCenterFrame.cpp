#include "MessageCenterFrame.h"
#include "MessageCenterManager.h"

MessageCenterFrame::MessageCenterFrame() {
	init("LandingPageFrame.xml", true);
	selectTransitions();
}

MessageCenterFrame::~MessageCenterFrame() {

}

void MessageCenterFrame::selectTransitions() {
	spTransition transition = new TransitionFade;
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
	spSlidingActor slidingActor = new SlidingActor();
	slidingActor->setSize(_view->getWidth(), _view->getHeight());

	spColorRectSprite rectangleContainer = new ColorRectSprite();
	rectangleContainer->setSize(slidingActor->getWidth(), slidingActor->getHeight());
	rectangleContainer->setColor(Color(0, 0, 0, 0));

	float positionY = slidingActor->getHeight() * 0.3f / 2.0f;
	float itemHeight = 0.0f;

	const std::vector<spMessageModel>& messages = MessageCenterManager::instance.getMessages();

	const Vector2& size = Vector2(slidingActor->getWidth() * 0.9f, slidingActor->getHeight() * 0.2f);

	for (uint i = 0; i < messages.size(); i++) {
		spMessageItem item = createMessageItem(size, messages[i]);
		item->setPosition(slidingActor->getWidth() / 2.0f, positionY);
		positionY += item->getDerivedHeight() + OFFSET_MESSAGES;
		itemHeight = item->getDerivedHeight();
		rectangleContainer->addChild(item);
	}

	rectangleContainer->setSize(slidingActor->getWidth(), positionY - itemHeight / 2.0f);
	slidingActor->setContent(rectangleContainer);
	slidingActor->setPosition((_view->getWidth() - slidingActor->getWidth()) / 2.0f, (_view->getHeight() - slidingActor->getHeight()));
	slidingActor->attachTo(_view);
}

spMessageItem MessageCenterFrame::createMessageItem(const Vector2& size, spMessageModel model) {
	spMessageItem item = new MessageItem(size, model);

	return item;
}