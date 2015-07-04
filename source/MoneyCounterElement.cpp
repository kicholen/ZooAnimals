#include "MoneyCounterElement.h"
#include "MoneyManager.h"
#include "SharedResources.h"

MoneyCounterElement::MoneyCounterElement(MoneyCounterElementState state) {
	setSize(Vector2(getRoot()->getWidth() * 0.33f, getRoot()->getWidth() * 0.33f * 0.2f));
	setTouchEnabled(false);
	setTouchChildrenEnabled(false);
	setAnchor(0.5f, 0.5f);
	setDisplayState(state);
	_updateSizeTime = -10.0f;
	_money = MoneyManager::instance.getMoney();

	createBackground();
	_bronzeTextField = createMoneyTextfield(getBronzeMoney());
	_bronzeTextField->setAnchor(1.0f, 0.5f);
	_bronzeSprite = createMoneyIcon("smallGold");

	_silverTextField = createMoneyTextfield(getSilverMoney());
	_silverTextField->setAnchor(1.0f, 0.5f);
	_silverSprite = createMoneyIcon("mediumGold");

	_goldTextField = createMoneyTextfield(getGoldMoney());
	_goldTextField->setAnchor(1.0f, 0.5f);
	_goldSprite = createMoneyIcon("largeGold");

	updatePosition();

	getRoot()->addEventListener(MoneyManager::MoneyEvent::MONEY_COUNT, CLOSURE(this, &MoneyCounterElement::onMoneyChanged));
}

MoneyCounterElement::~MoneyCounterElement() {
	if (getRoot()) {
		getRoot()->removeEventListener(MoneyManager::MoneyEvent::MONEY_COUNT, CLOSURE(this, &MoneyCounterElement::onMoneyChanged));
	}
}

void MoneyCounterElement::setDisplayState(MoneyCounterElementState state) {
	if (state == mceBottom) {
		_showPosition = Vector2(getRoot()->getWidth() - getWidth() / 2, getRoot()->getHeight() - getHeight() / 2);
		_hidePosition = _showPosition + Vector2(0.0f, getHeight());
	}
	else if (state == mceTop) {
		_showPosition = Vector2(getRoot()->getWidth() - getWidth() / 2, getHeight() / 2);
		_hidePosition = _showPosition + Vector2(0.0f, -getHeight());
	}
	else {
		log::error("MoneyCounterElement::setDisplayState state not available.");
	}
}

void MoneyCounterElement::doUpdate(const UpdateState &us) {
	if (_updateSizeTime < 0.0f) {
		return;
	}

	_updateSizeTime -= us.dt;

	updatePosition();
}

void MoneyCounterElement::show(bool shouldAnimate) {
	if (shouldAnimate) {
		setPosition(_hidePosition);
		addTween(Sprite::TweenPosition(_showPosition), 300, 1, false, 0, Tween::ease_outBack);
	}
	else {
		setPosition(_showPosition);
	}
}

void MoneyCounterElement::hide(bool shouldAnimate) {
	if (shouldAnimate) {
		addTween(Sprite::TweenPosition(_hidePosition), 150, 1, false, 0, Tween::ease_outBack);
	}
	else {
		setPosition(_hidePosition);
	}
}

void MoneyCounterElement::createBackground() {
	_background = new Box9Sprite;
	_background->setAnchor(1.0f, 0.5f);
	_background->setHorizontalMode(Box9Sprite::STRETCHING);
	_background->setVerticalMode(Box9Sprite::STRETCHING);
	_background->setResAnim(gameResources.getResAnim("greyBox9"));
	_background->setSize(getSize());
	_background->setPosition(getWidth(), getHeight() / 2);
	_background->setGuides(9, 20, 9, 15);
	_background->attachTo(this);
	_background->setPriority(-1);
}

spTweenTextField MoneyCounterElement::createMoneyTextfield(int money) {
	spTweenTextField textField = new TweenTextField();
	textField->setStyle(createTextStyle(gameResources.getResFont("nobile_bold")->getFont(), Color(217, 88, 144), false, TextStyle::HALIGN_RIGHT, TextStyle::VALIGN_MIDDLE));
	textField->setFontSize2Scale(15 * (int)getRoot()->getWidth() / 320);
	textField->setValue(money);
	textField->setSize(getWidth() * 0.2f, getHeight() * 0.2f);
	textField->setPriority(2);
	textField->attachTo(this);

	return textField;
}

spSprite MoneyCounterElement::createMoneyIcon(const std::string& spriteName) {
	spSprite moneySprite = new Sprite();
	moneySprite->setAnchor(0.5f, 0.5f);
	moneySprite->setResAnim(tilesResources.getResAnim(spriteName));
	moneySprite->setPosition(getWidth() / 2, getHeight() / 2);
	moneySprite->attachTo(this);
	moneySprite->setPriority(-1);
	setSpriteScaleBySize(moneySprite, Vector2(getHeight() * 0.8f, getHeight() * 0.8f));

	return moneySprite;
}

void MoneyCounterElement::onMoneyChanged(Event *ev) {
	MoneyManager::MoneyEvent* moneyEvent = safeCast<MoneyManager::MoneyEvent*>(ev);
	_money = moneyEvent->money;
	updateTextFields();
}

void MoneyCounterElement::updateTextFields() {
	int bronzeMoney = getBronzeMoney();
	if (_bronzeTextField->getValue() != bronzeMoney) {
		_bronzeTextField->addTween(TweenTextField::TweenTextInt(bronzeMoney), ANIMATION_TIME, 1);
		_updateSizeTime = ANIMATION_TIME + 100.0f;
	}

	int silverMoney = getSilverMoney();
	if (_silverTextField->getValue() != silverMoney) {
		_silverTextField->addTween(TweenTextField::TweenTextInt(silverMoney), ANIMATION_TIME, 1);
		_updateSizeTime = ANIMATION_TIME + 100.0f;
	}

	int goldMoney = getGoldMoney();
	if (_goldTextField->getValue() != goldMoney) {
		_goldTextField->addTween(TweenTextField::TweenTextInt(goldMoney), ANIMATION_TIME, 1);
		_updateSizeTime = ANIMATION_TIME + 100.0f;
	}
}

void MoneyCounterElement::updatePosition() {
	float offset = 5.0f;
	Vector2 position = Vector2(getWidth() - offset, getHeight() / 2.0f);
	_bronzeTextField->setPosition(position.x, position.y - offset);
	position.x -= _bronzeTextField->getTextRect().getWidth() + _bronzeSprite->getDerivedWidth() / 2.0f;
	_bronzeSprite->setPosition(position);

	position.x -= _bronzeSprite->getDerivedWidth() / 2.0f;
	_silverTextField->setPosition(position.x, position.y - offset);
	position.x -= _silverTextField->getTextRect().getWidth() + _silverSprite->getDerivedWidth() / 2.0f;
	_silverSprite->setPosition(position);

	position.x -= _silverSprite->getDerivedWidth() / 2.0f;
	_goldTextField->setPosition(position.x, position.y - offset);
	position.x -= _goldTextField->getTextRect().getWidth() + _goldSprite->getDerivedWidth() / 2.0f;
	_goldSprite->setPosition(position);

	_background->setWidth(getWidth() - position.x + _goldSprite->getDerivedWidth() / 2.0f + offset);
}

int MoneyCounterElement::getBronzeMoney() {
	return _money % 100;
}

int MoneyCounterElement::getSilverMoney() {
	return _money / 100 % 100;
}

int MoneyCounterElement::getGoldMoney() {
	return _money / 100 / 100 % 100;
}