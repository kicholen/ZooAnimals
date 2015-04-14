#include "MemoryCard.h"
#include "SharedResources.h"

MemoryCard::MemoryCard(string cardId) {
	_state = mcNormal;
	setTouchEnabled(false);
	setAnchor(0.5f, 0.5f);
	setSize(DISPLAY_SIZE_X, DISPLAY_SIZE_Y);

	createBackground(-1);
	createQuestionMark();
	createMask();
	createMaskedSprite();
	createAnimalSprite(cardId);
}

void MemoryCard::set(string id) {
	_animalSlot->switchAnimalSprite(id);
	_mask->setPosition(_mask->getDerivedSize() * 3 / 2);
	setVisible(true);
	_state = mcNormal;
}

void MemoryCard::unSelect() {
	if (_state != mcSelected) {
		return;
	}

	_state = mcNormal;
	setRotation(0);
	removeTweens();
}

void MemoryCard::select() {
	if (_state != mcNormal) {
		return;
	}
	_state = mcSelected;
}


spTween MemoryCard::playAnimation(bool show) {
	if (_state != mcNormal) {
		return 0;
	}

	Vector2 position = show ? Vector2(getDerivedSize() / 2) : Vector2(_mask->getDerivedSize() * 3 / 2);
	return _mask->addTween(Sprite::TweenPosition(position), 500, 1);
}

spTween MemoryCard::dropTo(Vector2 pos) {
	int time = (int)fabs(getPosition().y - pos.y) * 2;
	spTween tween = addTween(Sprite::TweenPosition(pos),time);
	_state = mcDropped;
	return tween;
}

void MemoryCard::setCardScored() {
	_state = mcScored;
	setVisible(false);
}

spBox9Sprite MemoryCard::createBackground(short zPriority) {
	spBox9Sprite cardBackground = new Box9Sprite;
	cardBackground->setHorizontalMode(Box9Sprite::STRETCHING);
	cardBackground->setVerticalMode(Box9Sprite::STRETCHING);
	cardBackground->setAnchor(0.5f, 0.5f);
	cardBackground->setResAnim(gameResources.getResAnim("greyBox9"));
	cardBackground->setSize(getSize());
	cardBackground->setPosition(getWidth() / 2, getHeight() / 2);
	cardBackground->setGuides(9, 20, 9, 15);
	cardBackground->attachTo(this);
	cardBackground->setPriority(zPriority);
	return cardBackground;
}

void MemoryCard::createQuestionMark() {
	_questionMark = new Sprite();
	_questionMark->setAnchor(0.5f, 0.5f);
	_questionMark->setTouchEnabled(false);
	_questionMark->setResAnim(gameResources.getResAnim("questionMark"));
	_questionMark->setScale(getWidth() / _questionMark->getWidth() * 0.5f, getHeight() / _questionMark->getHeight() * 0.5f);
	_questionMark->setPosition(getWidth() / 2, getHeight() / 2);
	_questionMark->attachTo(this);
}

void MemoryCard::createMask() {
	_mask = new Sprite(); 
	_mask->setAnchor(0.5f, 0.5f);
	_mask->setAlpha(1);
	_mask->setTouchEnabled(false);
	_mask->setVisible(false);
	_mask->setRotation(45.0f * (float)DEG_TO_RAD);
	_mask->setResAnim(gameResources.getResAnim("greyBox9"));
	_mask->setScale(getHeight() / _mask->getWidth() * 1.5f, getHeight() / _mask->getHeight() * 1.5f);
	_mask->setPosition(_mask->getDerivedSize() * 3 / 2);
	addChild(_mask);
}

void MemoryCard::createMaskedSprite() {
	_masked = new MaskedSprite();
	addChild(_masked);

	_masked->setMask(_mask);
}

void MemoryCard::createAnimalSprite(string id) {
	_animalSlot = new AnimalSlot(id);
	_animalSlot->setAnchor(0.5f, 0.5f);
	_animalSlot->setPosition(getWidth() / 2, getHeight() / 2);
	_animalSlot->attachTo(_masked);
}