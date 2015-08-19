#include "MessageItem.h"
#include "SharedResources.h"
#include "LanguageManager.h"

MessageItem::MessageItem(const Vector2& size, spMessageModel model) {
	setSize(size);
	setAnchor(0.5f, 0.5f);
	setTouchEnabled(false);

	setData(model);
}

MessageItem::~MessageItem() {

}

void MessageItem::setData(spMessageModel model) {
	createDateTextfield(model->getDateMS());
	createDescriptionTextfield(model->getLockitDescription());
	createTitleTexfield(model->getLockitTitle());
	createIconSprite(model->getResourceName());
	
	spTweenButton button = createButton("", LanguageManager::instance.getText(100));
	if (model->getType() == 0) {
		button->addEventListener(TouchEvent::CLICK, CLOSURE(this, &MessageItem::onRewardsClaimed));
	}
	else {
		button->addEventListener(TouchEvent::CLICK, CLOSURE(this, &MessageItem::onMessageDeleted));
	}
	button->setAnchor(1.0f, 1.0f);
	button->setPosition(getSize());
	addChild(button);
	_model = model;
}

void MessageItem::createDateTextfield(int dateMS) { // todo set proper textfieldPositions
	//todo make timMs to date converter?
	TextStyle style;
	style.font = gameResources.getResFont("nobile_bold")->getFont();
	style.vAlign = TextStyle::VALIGN_MIDDLE;
	style.hAlign = TextStyle::HALIGN_CENTER;
	style.multiline = true;
	style.color = Color(35, 145, 245);

	spTextField text = createTextFieldInBoundries(CMath::intToString(dateMS), Vector2(50, 50), style);
	addChild(text);
}

void MessageItem::createTitleTexfield(int lockitId) {
	TextStyle style;
	style.font = gameResources.getResFont("nobile_bold")->getFont();
	style.vAlign = TextStyle::VALIGN_MIDDLE;
	style.hAlign = TextStyle::HALIGN_CENTER;
	style.multiline = true;
	style.color = Color(35, 145, 245);

	spTextField text = createTextFieldInBoundries(LanguageManager::instance.getText(lockitId), Vector2(50, 50), style);
	addChild(text);
}

void MessageItem::createDescriptionTextfield(int lockitId) {
	TextStyle style;
	style.font = gameResources.getResFont("nobile_bold")->getFont();
	style.vAlign = TextStyle::VALIGN_MIDDLE;
	style.hAlign = TextStyle::HALIGN_CENTER;
	style.multiline = true;
	style.color = Color(35, 145, 245);

	spTextField text = createTextFieldInBoundries(LanguageManager::instance.getText(lockitId), Vector2(50, 50), style);
	addChild(text);
}

void MessageItem::createIconSprite(const std::string& resourceName) {
	spSprite iconSprite = new Sprite();
	iconSprite->setResAnim(gameResources.getResAnim(resourceName));
	setSpriteScaleBySize(iconSprite, Vector2(getWidth() *  0.33f, getHeight()));
	iconSprite->attachTo(this);
}

void MessageItem::createBackground() {
	spBox9Sprite cardBackground = new Box9Sprite;
	cardBackground->setHorizontalMode(Box9Sprite::STRETCHING);
	cardBackground->setVerticalMode(Box9Sprite::STRETCHING);
	cardBackground->setAnchor(0.5f, 0.5f);
	cardBackground->setResAnim(gameResources.getResAnim("greyBox9"));
	cardBackground->setSize(getSize());
	cardBackground->setPosition(getWidth() / 2.0f, getHeight() / 2.0f);
	cardBackground->setGuides(9, 20, 9, 15);
	cardBackground->attachTo(this);
	cardBackground->setPriority(-1);
}

void MessageItem::onMessageDeleted(Event *event) {
	MessageItemEvent ev(MessageItemEvent::MESSAGE_DELETED, _model);
	dispatchEvent(&ev);
}

void MessageItem::onRewardsClaimed(Event *event) {
	MessageItemEvent ev(MessageItemEvent::REWARDS_CLAIMED, _model);
	dispatchEvent(&ev);
}