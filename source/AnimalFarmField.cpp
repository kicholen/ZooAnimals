#include "AnimalFarmField.h"

AnimalFarmField::AnimalFarmField(Vector2 fieldSize) {
	setInputEnabled(false);
	setAnchor(0.0f, 0.0f);
	setSize(fieldSize);
	_state = afCreating;
	_lastAnimalsAnimateTime = 0.0f;

	spColorRectSprite bg = new ColorRectSprite();
	bg->setColor(Color(222, 100, 100));
	bg->setSize(getSize());
	bg->setAnchor(0.0f, 0.0f);
	bg->setPosition(0.0f, 0.0f);
	bg->setPriority(-1000);
	addChild(bg);


	_animalsArray._vector.resize(0);
	_animalsArray._vector.reserve(100);
	addEventListener(TouchEvent::CLICK, CLOSURE(this, &AnimalFarmField::onTouchOver));
}


AnimalFarmField::~AnimalFarmField() {
	_animalsArray._vector.resize(0);
}

void AnimalFarmField::setData(string animalName, uint animalsCount) {
	for (uint i = 0; i < animalsCount; i++) {
		_animalsArray.push(createAnimal(CMath::intToString(i), animalName));
	}
	_species = animalName;

	createAddAnimalButton("add_animal", Vector2(this->getWidth() * 0.9f, this->getHeight() * 0.1f));

	_animateDuration = 5.0f * 1000;
	_state = afWaiting;
}

void AnimalFarmField::addAnimal(Event *event) {
	_animalsArray.push(createAnimal(CMath::intToString(_animalsArray._vector.size() + 1), _species));
}

spAnimalInFarmElement AnimalFarmField::createAnimal(string animalNumber, string spriteName) {
	spAnimalInFarmElement animalElement = getChildT<AnimalInFarmElement>(animalNumber, oxygine::ep_ignore_error);
	if (animalElement == NULL) {
		animalElement = new AnimalInFarmElement(spriteName, getSize());
		addChild(animalElement);
	}
	else {
		animalElement->setAnimalSprite(spriteName);
	}
	animalElement->setName(animalNumber);

	return animalElement;
}

void AnimalFarmField::createAddAnimalButton(string buttonName, Vector2 position) {
	spButton btn = new Button;
	btn->setResAnim(gameResources.getResAnim("button_yellow"));
	btn->setAnchor(0.5f, 0.5f);
	btn->setPosition(position);
	btn->attachTo(this);
	btn->setPriority(20);
	btn->addEventListener(TouchEvent::CLICK, CLOSURE(this, &AnimalFarmField::addAnimal));
	btn->setScale(this->getWidth() * 0.1f / btn->getWidth());
	//spTweenButton button = createButton(buttonName, buttonName);
	//button->setPosition(position);
	//button->attachTo(this);
	//button->addEventListener(TouchEvent::CLICK, CLOSURE(this, &AnimalFarmField::addAnimal));
	//return button;
}

void AnimalFarmField::onTouchOver(Event *event) {
	Vector2 localPosition = safeCast<TouchEvent*>(event)->localPosition;

	tryToAnimateAnimals(localPosition);
}

void AnimalFarmField::tryToAnimateAnimals(Vector2 position) {
	//if (canAnimalsAnimate()) {
		for (int i = _animalsArray.length() - 1; i >= 0; i -= 1) {
			_animalsArray[i]->animateJump(position);
		}
		//_lastAnimalsAnimateTime = _animateDuration;
	//}
}
/*
bool AnimalFarmField::canAnimalsAnimate() {
	return _lastAnimalsAnimateTime <= 0.0f;
}
*/
void AnimalFarmField::doUpdate(const UpdateState &us) {
	if (_lastAnimalsAnimateTime > 0.0f) {
		_lastAnimalsAnimateTime -= us.dt;
	}

	setAnimalsPriorityByY();
}

void AnimalFarmField::setAnimalsPriorityByY() {
	for (int i = _animalsArray.length() - 1; i >= 0; i -= 1) {
		_animalsArray[i]->setPriority(int(_animalsArray[i]->getShadowY())); //_animalsArray[i]->getShadowY();
	}
}