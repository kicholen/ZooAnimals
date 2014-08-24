#include "AnimalSlot.h"
#include "SharedResources.h"

AnimalSlot::AnimalSlot(string id) {
	_id = id;
	setInputEnabled(false);
	setAnchor(0.5f, 0.5f);
	setSize(SLOT_SIZE_X , SLOT_SIZE_Y);

	createAnimalSprite();
	createBackground();
}

void AnimalSlot::switchAnimalSprite(string id) {
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
	cardBackground->setAnchor(0.5f, 0.5f);
	cardBackground->setResAnim(gameResources.getResAnim("memory_card_box9_a"));
	cardBackground->setScale(getWidth() / cardBackground->getWidth(), getHeight() / cardBackground->getHeight());
	cardBackground->setPosition(getWidth() / 2, getHeight() / 2);
	cardBackground->setGuides(36, 144, 36, 144);//(18, 144, 18, 144);
	cardBackground->attachTo(this);
	cardBackground->setPriority(-1);
}