#include "AnimalSlot.h"
#include "SharedResources.h"

AnimalSlot::AnimalSlot() {

}

AnimalSlot::AnimalSlot(const string& id) {
	_id = id;
	setTouchEnabled(false);
	setAnchor(0.5f, 0.5f);
	setSize(SLOT_SIZE_X , SLOT_SIZE_Y);

	createAnimalSprite();
	createBackground();
}

void AnimalSlot::switchAnimalSprite(const string& id) {
	_id = id;
	_animalSprite->setResAnim(animalsResources.getResAnim(id));
	setAlpha(255);
	_animalSprite->setAnchor(0.5f, 0.5f);
	_animalSprite->setPosition(getWidth() / 2, getHeight() / 2);

	float scaleInParent = 0.8f;
	float animalScale = 1.0f;
	if (_animalSprite->getWidth() > _animalSprite->getHeight()) {
		animalScale = scaleInParent * SLOT_SIZE_X / _animalSprite->getWidth();
	}
	else {
		animalScale = scaleInParent * SLOT_SIZE_Y / _animalSprite->getHeight();
	}
	_animalSprite->setScale(animalScale);
}

string AnimalSlot::getId() {
	return _id;
}

void AnimalSlot::createAnimalSprite() {
	_animalSprite = new Sprite();
	_animalSprite->setPriority(0);
	switchAnimalSprite(_id);
	addChild(_animalSprite);
}

void AnimalSlot::createBackground() {
	spBox9Sprite cardBackground = new Box9Sprite;
	cardBackground->setHorizontalMode(Box9Sprite::STRETCHING);
	cardBackground->setVerticalMode(Box9Sprite::STRETCHING);
	cardBackground->setAnchor(0.5f, 0.5f);
	cardBackground->setResAnim(gameResources.getResAnim("greyBox9"));
	cardBackground->setSize(getSize());
	cardBackground->setPosition(getWidth() / 2, getHeight() / 2);
	cardBackground->setGuides(9, 20, 9, 15);
	cardBackground->attachTo(this);
	cardBackground->setPriority(-1);
}