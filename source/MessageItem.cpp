#include "MessageItem.h"
#include "SharedResources.h"
#include "LanguageManager.h"
#include "time.h"
#include "clocale"

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
	createBackground();

	spTweenButton button = createButton("", "*CLAIM*"/*LanguageManager::instance.getText(100)*/);
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

void MessageItem::createDateTextfield(int dateMS) {
	TextStyle style;
	style.font = gameResources.getResFont("nobile_bold")->getFont();
	style.vAlign = TextStyle::VALIGN_MIDDLE;
	style.hAlign = TextStyle::HALIGN_RIGHT;
	style.color = Color(35, 145, 245);

	char bufor[64];
	time_t timeMS = dateMS;
	tm czasTM = *localtime(&timeMS);
	strftime(bufor, sizeof(bufor), "%x, %X", &czasTM);

	spTextField text = createTextFieldInBoundries(bufor, Vector2(getWidth() * 0.2f - 10.0f, getHeight() * 0.3f), style);
	text->setX(getWidth() * 0.8f - 10.0f);
	addChild(text);
}

void MessageItem::createTitleTexfield(int lockitId) {
	TextStyle style;
	style.font = gameResources.getResFont("nobile_bold")->getFont();
	style.vAlign = TextStyle::VALIGN_MIDDLE;
	style.hAlign = TextStyle::HALIGN_CENTER;
	style.color = Color(35, 145, 245);

	spTextField text = createTextFieldInBoundries("*TITLE*"/*LanguageManager::instance.getText(lockitId)*/, Vector2(getWidth() * 0.5f, getHeight() * 0.4f), style);
	text->setPosition(getWidth() * 0.33f, 0.0f);
	addChild(text);
}

void MessageItem::createDescriptionTextfield(int lockitId) {
	TextStyle style;
	style.font = gameResources.getResFont("nobile_bold")->getFont();
	style.vAlign = TextStyle::VALIGN_MIDDLE;
	style.hAlign = TextStyle::HALIGN_CENTER;
	style.multiline = true;
	style.color = Color(35, 145, 245);

	spTextField textField = initActor(new TextField,
		arg_style = style,
		arg_hAlign = TextStyle::HALIGN_LEFT,
		arg_vAlign = TextStyle::VALIGN_DEFAULT,
		arg_width = getWidth() * 0.6f,
		arg_height = getHeight() * 0.4f,
		arg_input = false,
		arg_priority = 10,
		arg_text = "blabala achievement blabla" /*LanguageManager::instance.getText(lockitId)*/);
	textField->setPosition(getHeight() * 1.1f, getHeight() * 0.5f);
	textField->setFontSize2Scale(15);
	addChild(textField);
}

void MessageItem::createIconSprite(const std::string& resourceName) {
	spSprite iconSprite = new Sprite();
	iconSprite->setResAnim(gameResources.getResAnim(resourceName));
	setSpriteScaleBySize(iconSprite, Vector2(getHeight(), getHeight()));
	iconSprite->attachTo(this);
}

void MessageItem::createBackground() {
	spBox9Sprite cardBackground = new Box9Sprite;
	cardBackground->setHorizontalMode(Box9Sprite::STRETCHING);
	cardBackground->setVerticalMode(Box9Sprite::STRETCHING);
	cardBackground->setAnchor(0.5f, 0.5f);
	cardBackground->setResAnim(gameResources.getResAnim("colo3Box9"));
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