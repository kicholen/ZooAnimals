#include "AnimalFarmField.h"
#include "Content.h"
#include "AnimalsManager.h"
#include <limits>

AnimalFarmField::AnimalFarmField(Vector2 fieldSize) {
	setTouchEnabled(false);
	setAnchor(0.0f, 0.0f);
	setSize(fieldSize);
	_state = afCreating;
	_count = 0;
	
	spColorRectSprite bg = new ColorRectSprite();
	bg->setColor(Color(222, 100, 100));
	bg->setSize(getSize());
	bg->setAnchor(0.0f, 0.0f);
	bg->setPosition(0.0f, 0.0f);
	bg->setPriority(-1000);
	addChild(bg);

	_zSortElements._vector.resize(0);
	_zSortElements._vector.reserve(5);

	addEventListener(TouchEvent::CLICK, CLOSURE(this, &AnimalFarmField::onTouchOver));
	addEventListener(TouchEvent::CLICK, CLOSURE(this, &AnimalFarmField::onTouchOver));
	AnimalsManager::instance.addEventListener(AnimalsManager::AnimalEvent::COUNT_CHANGED, CLOSURE(this, &AnimalFarmField::onAnimalCountChanged));

	_animationType = afaNone;
	_animalsFarmAnimation = new AnimalsFarmAnimations(Vector2(getWidth() * 0.95f, getHeight() * 0.9f));
	_animalsFarmAnimation->addRef();
	_lastTooltipShowTime = TOOLTIP_START_SHOW;
}

AnimalFarmField::~AnimalFarmField() {
	AnimalsManager::instance.removeEventListener(AnimalsManager::AnimalEvent::COUNT_CHANGED, CLOSURE(this, &AnimalFarmField::onAnimalCountChanged));
	_zSortElements._vector.resize(0);
	_animalsFarmAnimation->releaseRef();
}

void AnimalFarmField::setData(spAnimalModel model) {
	_model = model;

	for (int i = 0; i < _model->totalAnimalsCount(); i++) {
		_animalsFarmAnimation->addAnimal(createAnimal(CMath::intToString(i), _model));
	}

	//createAnimalButton("add", Vector2(this->getWidth() * 0.9f, this->getHeight() * 0.1f))->addEventListener(TouchEvent::CLICK, CLOSURE(this, &AnimalFarmField::addAnimal));
	//createAnimalButton("animate", Vector2(this->getWidth() * 0.9f, this->getHeight() * 0.4f))->addEventListener(TouchEvent::CLICK, CLOSURE(this, &AnimalFarmField::playNextAnimalsAnimation));

	spTileField tileField = createTileField();
	createSortElements(tileField);
	for (int i = _zSortElements.length() - 1; i >= 0; i -= 1) {
		_zSortElements[i]->setPriority(int(_zSortElements[i]->getY()));
	}

	_state = afWaiting;
}

spTileField AnimalFarmField::createTileField() {
	spTileField tileField = new TileField(getNumberOfTiles());
	tileField->setData(_model->animalName());
	tileField->setScale(getHeight() / tileField->getHeight());
	tileField->setName("tajle");
	tileField->setAnchor(0.0f, 0.0f);
	tileField->setPosition(0.0f, 0.0f);
	tileField->setPriority(-500);
	addChild(tileField);
	return tileField;
}

void AnimalFarmField::createSortElements(spTileField tileField) {
	/*pugi::xml_node animalParameters = Content::instance.getAnimalFarmSortParametersNode(_model->animalName());
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
	*/
	createFenceAtBottom(tileField);
	createFenceAtLeft(tileField);
	createInformationTable(tileField);
}

void AnimalFarmField::createFenceAtBottom(spTileField tileField) {
	Point tilesNumber = getNumberOfTiles();
	int j = tilesNumber.y;
	int gatePosition = (int)FlashUtils::CMath::Rand(3, tilesNumber.x - 2);
	
	for (int i = 0; i < tilesNumber.x; i++) {
		spSprite tileSprite;
		if (i == 0) {
			tileSprite = tileField->createTileSprite("fenceFrontSide", Vector2(TILE_SIZE_X * tileField->getScaleX(), TILE_SIZE_Y * tileField->getScaleY()), Point(i, j), FlashUtils::CMath::intToString(i) + FlashUtils::CMath::intToString(j) + "_tile", 3000);
		}
		else if (i == tilesNumber.x - 1) {
			tileSprite = tileField->createTileSprite("fenceFrontSide", Vector2(TILE_SIZE_X * tileField->getScaleX(), TILE_SIZE_Y * tileField->getScaleY()), Point(i, j), FlashUtils::CMath::intToString(i) + FlashUtils::CMath::intToString(j) + "_tile", 3000);
			tileSprite->setScaleX(-tileSprite->getScaleX());
		}
		else if (gatePosition == i) {
			tileSprite = tileField->createTileSprite("fenceFrontGate", Vector2(TILE_SIZE_X * tileField->getScaleX(), TILE_SIZE_Y * tileField->getScaleY()), Point(i, j), FlashUtils::CMath::intToString(i) + FlashUtils::CMath::intToString(j) + "_tile", 3000);
		}
		else {
			tileSprite = tileField->createTileSprite("fenceFrontCenter", Vector2(TILE_SIZE_X * tileField->getScaleX(), TILE_SIZE_Y * tileField->getScaleY()), Point(i, j), FlashUtils::CMath::intToString(i) + FlashUtils::CMath::intToString(j) + "_tile", 3000);
		}
		tileSprite->setAnchor(0.0f, 1.0f);
		tileSprite->setPosition(tileSprite->getScaleX() > 0 ? tileSprite->getX() * tileField->getScaleX() : tileSprite->getX() * tileField->getScaleX() - tileSprite->getDerivedWidth(), tileSprite->getY() * tileField->getScaleY() + 2.0f);
		addChild(tileSprite);
	}
}

void AnimalFarmField::createFenceAtLeft(spTileField tileField) {
	Point tilesNumber = getNumberOfTiles();
	int i = tilesNumber.x;
	for (int j = 0; j < tilesNumber.y; j++) {
		spSprite tileSprite = tileField->createTileSprite("fenceTop", Vector2(TILE_SIZE_X * tileField->getScaleX(), TILE_SIZE_Y * tileField->getScaleY()), Point(i, j), FlashUtils::CMath::intToString(i) + FlashUtils::CMath::intToString(j) + "_tile", 2900);
		tileSprite->setAnchor(0.0f, 0.0f);
		tileSprite->setPosition(- tileSprite->getDerivedWidth() / 2, tileSprite->getY() * tileField->getScaleY());
		tileSprite->setPriority(3000);
		addChild(tileSprite);
	}
}

void AnimalFarmField::createInformationTable(spTileField tileField) {
	Point tilesNumber = getNumberOfTiles();
	_animalPanel = new AnimalFarmPanel(Vector2(getWidth() * (float)BASE_SIZE_IN_PERCENT_X / 100.0f, getWidth() * (float)BASE_SIZE_IN_PERCENT_Y / 100.0f), Vector2(getWidth() * (float)EXPANDED_SIZE_IN_PERCENT_X / 100.0f, getWidth() * (float)EXPANDED_SIZE_IN_PERCENT_Y / 100.0f));
	_animalPanel->setAnchor(1.0f, 1.0f);
	_animalPanel->setPosition(getWidth(), getHeight() - _animalPanel->getHeight() / 2);
	_animalPanel->setData(_model);
	_animalPanel->attachTo(this);
	_animalPanel->setPriority(30000);
	_animalPanel->addEventListener(FarmServiceElement::FarmServiceElementEvent::PLAY_GAMES, CLOSURE(this, &AnimalFarmField::onGameChosen));

}

void AnimalFarmField::playNextAnimalsAnimation(Event *event) {
	_animationType = _animationType + 1 == afaCount ? static_cast<AnimalsFarmAnimationType>(0) : static_cast<AnimalsFarmAnimationType>(_animationType + 1);
	_animalsFarmAnimation->playAnimalsAnimation(_animationType);
}

void AnimalFarmField::addAnimal(Event *event) {
	/*int x = 10;
	_model->setAnimalsCount(_model->animalsCount() + x);
	while (x > 0) {
		_animalsFarmAnimation->addAnimal((createAnimal(CMath::intToString(_count), _model)));
		x--;
	}*/
}

spAnimalInFarmElement AnimalFarmField::createAnimal(const std::string& animalNumber, spAnimalModel model) {
	spAnimalInFarmElement animalElement = getChildT<AnimalInFarmElement>(animalNumber, oxygine::ep_ignore_error);
	if (!animalElement) {
		animalElement = new AnimalInFarmElement(model->animalName(), Vector2(getWidth() * 0.95f, getHeight() * 0.9f), model->jumpRange(), model->jumpHeight(), model->jumpTime(), model->jumpDelay(), model->isWaterAnimal());
		_count += 1;
		animalElement->setPosition(getWidth() * 0.025f, getHeight() * 0.05f);
		addChild(animalElement);
	}
	else {
		animalElement->setAnimalSprite(model->getName());
	}
	animalElement->setName(animalNumber);

	return animalElement;
}

void AnimalFarmField::removeLastAnimal() {
	spAnimalInFarmElement animalElement = _animalsFarmAnimation->removeAnimal();
	OX_ASSERT(animalElement);
	animalElement->detach();
	_count--;
}

spButton AnimalFarmField::createAnimalButton(const std::string& buttonName, Vector2 position) {
	spButton btn = new Button;
	btn->setResAnim(gameResources.getResAnim("button_yellow"));
	btn->setAnchor(0.5f, 0.5f);
	btn->setPosition(position);
	btn->attachTo(this);
	btn->setPriority(20);
	btn->setScale(this->getWidth() * 0.1f / btn->getWidth());

	return btn;
}

void AnimalFarmField::onTouchOver(Event *event) {
	Vector2 localPosition = safeCast<TouchEvent*>(event)->localPosition;

	animateAnimalsJump(localPosition);
	_animalPanel->closeExpandedViewIfOpen();
}

void AnimalFarmField::animateAnimalsJump(Vector2 position) {
	_animalsFarmAnimation->animateAnimalsJump(position);
}

void AnimalFarmField::doUpdate(const UpdateState& us) {
	bool isVisible = isOnScreen(this);
	_animalsFarmAnimation->doUpdate(us, isVisible);

	if (isVisible) {
		if (_lastTooltipShowTime > 0.0f) {
			_lastTooltipShowTime -= us.dt;
		}
		else {
			_lastTooltipShowTime = FlashUtils::CMath::Rand(TOOLTIP_RESHOW_MIN, TOOLTIP_RESHOW_MAX);

			// some action like animal is hungry or happy or wanna play or say sth
			_animalsFarmAnimation->attachElementToRandomAnimal(createTooltipElement());
			_tooltip->showForTime(5000);
		}
	}
}

void AnimalFarmField::onGameChosen(Event *event) {
	event->target = this;
	dispatchEvent(event);
}

void AnimalFarmField::onAnimalCountChanged(Event *ev) {
	//AnimalsManager::AnimalEvent *animalEvent = safeCast<AnimalsManager::AnimalEvent*>(ev);
	int countDiff = _model->totalAnimalsCount() - _count;
	if (countDiff != 0) {
		if (countDiff > 0) {
			for (int i = _model->totalAnimalsCount(); i < _model->totalAnimalsCount() + countDiff; i++) {
				_animalsFarmAnimation->addAnimal(createAnimal(CMath::intToString(i), _model));
			}
		}
		else {
			for (int i = _model->totalAnimalsCount() + countDiff; i == _model->totalAnimalsCount(); i--) {
				removeLastAnimal();
			}
		}
	}
}

Point AnimalFarmField::getNumberOfTiles() {
	float ratio = (getSize().x / 32.0f) / (getSize().y / 32.0f);
	return Point(int(ratio * 6.0f), 6);
}

spTooltipElement AnimalFarmField::createTooltipElement() {
	if (!_tooltip) {
		_tooltip = new TooltipElement(Vector2(ANIMAL_PERCENT_SIZE / 100.0f * getWidth(), ANIMAL_PERCENT_SIZE / 100.0f * getHeight()), "circle_border", "", 10);
	}
	return _tooltip;
}
