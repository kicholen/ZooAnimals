#include "AnimalFarmField.h"
#include "Content.h"

AnimalFarmField::AnimalFarmField(Vector2 fieldSize) {
	setTouchEnabled(false);
	setAnchor(0.0f, 0.0f);
	setSize(fieldSize);
	_state = afCreating;
	_lastZSortTime = 0.0f;
	_canAnimalsUpdate = true;

	spColorRectSprite bg = new ColorRectSprite();
	bg->setColor(Color(222, 100, 100));
	bg->setSize(getSize());
	bg->setAnchor(0.0f, 0.0f);
	bg->setPosition(0.0f, 0.0f);
	bg->setPriority(-1000);
	addChild(bg);

	_animalsArray._vector.resize(0);
	_animalsArray._vector.reserve(10);
	_zSortElements._vector.resize(0);
	_zSortElements._vector.reserve(5);

	addEventListener(TouchEvent::CLICK, CLOSURE(this, &AnimalFarmField::onTouchOver));
}


AnimalFarmField::~AnimalFarmField() {
	_animalsArray._vector.resize(0);
	_zSortElements._vector.resize(0);
}

void AnimalFarmField::setData(string animalName, uint animalsCount) {
	VectorArray<int> parameters = getAnimalParameters(animalName);

	for (uint i = 0; i < animalsCount; i++) {
		_animalsArray.push(createAnimal(CMath::intToString(i), animalName, (float)parameters[0], (float)parameters[1], (float)parameters[2], Vector2(parameters[3], parameters[4]), parameters[5] == 0 ? false : true));
	}
	_species = animalName;
	parameters._vector.resize(0);

	createAddAnimalButton("add_animal", Vector2(this->getWidth() * 0.9f, this->getHeight() * 0.1f));
	spTileField tileField = createTileField();
	createCustomElements(tileField);

	_animateDuration = 5.0f * 1000;
	_state = afWaiting;
}

spTileField AnimalFarmField::createTileField() {
	float ratio = (getSize().x / 32.0f) / (getSize().y / 32.0f);
	spTileField tileField = new TileField(Point((int)ratio * 6, 6));
	tileField->setData(_species);
	tileField->setScale(getHeight() / tileField->getHeight());
	tileField->setName("tajle");
	//tileField->setSize(getSize());
	tileField->setAnchor(0.0f, 0.0f);
	tileField->setPosition(0.0f, 0.0f);
	tileField->setPriority(-500);
	addChild(tileField);
	return tileField;
}

void AnimalFarmField::createCustomElements(spTileField tileField) {
	pugi::xml_node animalParameters = Content::instance.getAnimalFarmSortParametersNode(_species);
	pugi::xml_node parameter = animalParameters.first_child();
	
	while (parameter) {
		int i = parameter.first_attribute().as_int();
		int j = parameter.first_attribute().next_attribute().as_int();
		spSprite tileSprite = tileField->createTileSprite(parameter.name(), Vector2(TILE_SIZE_X, TILE_SIZE_Y), Point(i, j), FlashUtils::CMath::intToString(i) + FlashUtils::CMath::intToString(j) + "_tile", 1);
		tileSprite->setAnchor(0.0f, 1.0f);
		tileSprite->setY(tileSprite->getY() + TILE_SIZE_Y);
		_zSortElements.push(tileSprite);
		addChild(tileSprite);
		parameter = parameter.next_sibling();
	}
}

VectorArray<int> AnimalFarmField::getAnimalParameters(string animalName) {
	pugi::xml_node animalParameters = Content::instance.getAnimalJumpParametersNode(animalName);
	pugi::xml_attribute attribute = animalParameters.first_attribute();
	VectorArray<int> attributesArray;
	attributesArray._vector.resize(0);
	attributesArray._vector.reserve(6);

	while (!attribute.empty()) {
		attributesArray.push(attribute.as_int());
		attribute = attribute.next_attribute();
	}

	return attributesArray;
}

void AnimalFarmField::addAnimal(Event *event) {
	VectorArray<int> parameters = getAnimalParameters(_species);
	int x = 10;
	while (x > 0) {
		_animalsArray.push(createAnimal(CMath::intToString(_animalsArray._vector.size() + 1), _species, (float)parameters[0], (float)parameters[1], (float)parameters[2], Vector2((float)parameters[3], (float)parameters[4]), parameters[5] == 0 ? false : true));
		x--;
	}
	parameters._vector.resize(0);
}

spAnimalInFarmElement AnimalFarmField::createAnimal(string animalNumber, string spriteName, float jumpRange, float jumpHeight, float jumpTime, Vector2 delayRandom, bool isWaterAnimal) {
	spAnimalInFarmElement animalElement = getChildT<AnimalInFarmElement>(animalNumber, oxygine::ep_ignore_error);
	if (!animalElement) {
		animalElement = new AnimalInFarmElement(spriteName, getSize(), jumpRange, jumpHeight, jumpTime, delayRandom, isWaterAnimal);
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
}

void AnimalFarmField::onTouchOver(Event *event) {
	Vector2 localPosition = safeCast<TouchEvent*>(event)->localPosition;

	tryToAnimateAnimals(localPosition);
}

void AnimalFarmField::tryToAnimateAnimals(Vector2 position) {
	for (int i = _animalsArray.length() - 1; i >= 0; i -= 1) {
		_animalsArray[i]->animateJump(position);
	}
}

void AnimalFarmField::doUpdate(const UpdateState &us) {
	if (_lastZSortTime > 0.0f) {
		_lastZSortTime -= us.dt;
	}
	else {
		_lastZSortTime = SORT_Z_DELAY;

		if (isOnScreen(this)) {
			setAnimalsPriorityByY();

			if (!_canAnimalsUpdate) {
				_canAnimalsUpdate = true;
				for (int i = _animalsArray.length() - 1; i >= 0; i -= 1) {
					_animalsArray[i]->_canUpdate = _canAnimalsUpdate;
				}
			}
		}
		else if (_canAnimalsUpdate) {
			_canAnimalsUpdate = false;
			for (int i = _animalsArray.length() - 1; i >= 0; i -= 1) {
				_animalsArray[i]->_canUpdate = _canAnimalsUpdate;
			}
		}
	}
}

void AnimalFarmField::setAnimalsPriorityByY() {
	for (int i = _animalsArray.length() - 1; i >= 0; i -= 1) {
		_animalsArray[i]->setPriority(int(_animalsArray[i]->getShadowY()));
	}
	for (int i = _zSortElements.length() - 1; i >= 0; i -= 1) {
		_zSortElements[i]->setPriority(int(_zSortElements[i]->getY()));
	}
}