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

	_animationType = afaNone;
	_animalsFarmAnimation = new AnimalsFarmAnimations(fieldSize);
	_animalsFarmAnimation->addRef();
}


AnimalFarmField::~AnimalFarmField() {
	_zSortElements._vector.resize(0);
	_animalsFarmAnimation->releaseRef();
}

void AnimalFarmField::setData(spAnimalModel model) {
	_model = model;

	for (int i = 0; i < _model->animalsCount(); i++) {
		_animalsFarmAnimation->addAnimal(createAnimal(CMath::intToString(i), _model));
	}

	createAnimalButton("add", Vector2(this->getWidth() * 0.9f, this->getHeight() * 0.1f))->addEventListener(TouchEvent::CLICK, CLOSURE(this, &AnimalFarmField::addAnimal));
	createAnimalButton("animate", Vector2(this->getWidth() * 0.9f, this->getHeight() * 0.4f))->addEventListener(TouchEvent::CLICK, CLOSURE(this, &AnimalFarmField::playNextAnimalsAnimation));

	spTileField tileField = createTileField();
	createCustomElements(tileField);
	for (int i = _zSortElements.length() - 1; i >= 0; i -= 1) {
		_zSortElements[i]->setPriority(int(_zSortElements[i]->getY()));
	}

	// 5% base
	// 15% max
	// 1% offset

	_animalPanel = new AnimalFarmPanel(Vector2(getWidth() * 0.05f, getWidth() * 0.05f), Vector2(getWidth() * 0.15f, getWidth() * 0.15f));
	_animalPanel->setPosition(getWidth() * 0.94f, getHeight() - getWidth() * 0.06f);
	_animalPanel->setData(_model);
	_animalPanel->attachTo(this);
	_animalPanel->setPriority(30000);

	_state = afWaiting;
}

spTileField AnimalFarmField::createTileField() {
	float ratio = (getSize().x / 32.0f) / (getSize().y / 32.0f);
	spTileField tileField = new TileField(Point((int)ratio * 6, 6));
	tileField->setData(_model->animalName());
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
	pugi::xml_node animalParameters = Content::instance.getAnimalFarmSortParametersNode(_model->animalName());
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

void AnimalFarmField::playNextAnimalsAnimation(Event *event) {
	_animationType = _animationType + 1 == afaCount ? static_cast<AnimalsFarmAnimationType>(0) : static_cast<AnimalsFarmAnimationType>(_animationType + 1);
	_animalsFarmAnimation->playAnimalsAnimation(_animationType);
}

void AnimalFarmField::addAnimal(Event *event) {
	int x = 10;
	_model->setAnimalsCount(_model->animalsCount() + x);
	while (x > 0) {
		_animalsFarmAnimation->addAnimal((createAnimal(CMath::intToString(_count + 1), _model)));
		x--;
	}
}

spAnimalInFarmElement AnimalFarmField::createAnimal(const string& animalNumber, spAnimalModel model) {
	spAnimalInFarmElement animalElement = getChildT<AnimalInFarmElement>(animalNumber, oxygine::ep_ignore_error);
	if (!animalElement) {
		animalElement = new AnimalInFarmElement(model->animalName(), getSize(), model->jumpRange(), model->jumpHeight(), model->jumpTime(), model->jumpDelay(), model->isWaterAnimal());
		_count += 1;
		addChild(animalElement);
	}
	else {
		animalElement->setAnimalSprite(model->getName());
	}
	animalElement->setName(animalNumber);

	return animalElement;
}

spButton AnimalFarmField::createAnimalButton(const string& buttonName, Vector2 position) {
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
	_animalsFarmAnimation->doUpdate(us, isOnScreen(this));
}