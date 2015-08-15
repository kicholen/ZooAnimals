#include "LevelUpItem.h"
#include "SharedResources.h"
#include "LanguageManager.h"

LevelUpItem::LevelUpItem(const Vector2& size, int requiredLevel) {
	setTouchEnabled(false);
	setTouchChildrenEnabled(false);
	setAnchor(0.5f, 0.5f);
	setSize(size);
	createBackground();
	createMainTextfield(requiredLevel);
	createStackContainer();
	_countTextfieldSize = Vector2(size.x * 0.2f, size.y * 0.2f);
}

LevelUpItem::~LevelUpItem()
{
}

void LevelUpItem::addReward(spRewardModel model) {
	//spTextField countText = createCountTextfield(model->getCount());
	spSprite rewardSprite = createRewardSprite(model);
	//countText->setX(rewardSprite->getDerivedWidth());
	//rewardSprite->addChild(countText);
}


void LevelUpItem::createMainTextfield(int requiredLevel) {
	TextStyle style;
	style.font = gameResources.getResFont("nobile_bold")->getFont();
	style.vAlign = TextStyle::VALIGN_MIDDLE;
	style.hAlign = TextStyle::HALIGN_CENTER;
	style.multiline = true;
	style.color = Color(35, 145, 245);

	spTextField textField = createTextFieldInBoundries(LanguageManager::instance.getText(100) + "*todo*" + CMath::intToString(requiredLevel), Vector2(getWidth(), getHeight() * 0.2f), style);
	textField->setY(OFFSET / 2.0f);
	addChild(textField);
}

void LevelUpItem::createBackground() {
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

void LevelUpItem::createStackContainer() {
	_stackContainer = new StackContainer(Vector2(getWidth(), getHeight() * 0.8f - OFFSET * 1.5f), 1);
	_stackContainer->attachTo(this);
	_stackContainer->setY(getHeight() * 0.2f + OFFSET);
}

spSprite LevelUpItem::createRewardSprite(spRewardModel model) {
	spSprite sprite = new Sprite();
	if (model->getType() == RewardType::rmHat) {
		sprite->setResAnim(gameResources.getResAnim(model->getName()));
	}
	else if (model->getType() == RewardType::rmAnimal){
		sprite->setResAnim(animalsResources.getResAnim(model->getName()));
	}
	else {
		sprite->setResAnim(gameResources.getResAnim("quad"));
	}
	sprite->setPosition(getWidth() * 0.33f, getHeight() / 2.0f);
	sprite->setTouchEnabled(false);
	sprite->setTouchChildrenEnabled(false);
	_stackContainer->addChild(sprite);

	return sprite;
}

spTextField LevelUpItem::createCountTextfield(int count) {
	TextStyle style;
	style.font = gameResources.getResFont("nobile_bold")->getFont();
	style.vAlign = TextStyle::VALIGN_MIDDLE;
	style.hAlign = TextStyle::HALIGN_CENTER;
	style.multiline = true;
	style.color = Color(35, 145, 245);

	spTextField textField = createTextFieldInBoundries("x" + CMath::intToString(count), _countTextfieldSize, style);
	return textField;
}
