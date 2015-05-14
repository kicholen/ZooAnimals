#include "AnimalCardElement.h"
#include "SharedResources.h"
#include "LanguageManager.h"

AnimalCardElement::AnimalCardElement(const Vector2& size, spAnimalModel model) {
	setSize(size);
	setAnchor(0.5f, 0.5f);
	setTouchChildrenEnabled(false);
	setTouchEnabled(false);
	setData(model);
}

AnimalCardElement::~AnimalCardElement() {

}

void AnimalCardElement::switchAnimalModel(spAnimalModel model) {
	setAnimalSlot(model->animalName());
}

void AnimalCardElement::setData(spAnimalModel model) {
	setAnimalSlot(model->animalName());
	createBackground();
	float offset = 5.0f;

	// name
	spTextField nameTextField = createText(10, Vector2(getWidth() * 0.4f, getHeight() * 0.1f));
	nameTextField->setAnchor(0.5f, 0.0f);
	nameTextField->setX(getWidth() * 0.75f);
	addChild(nameTextField);

	// height
	spTextField heightTextField = createText(10, Vector2(getWidth() * 0.2f, getHeight() * 0.1f));
	heightTextField->setY(nameTextField->getY() + nameTextField->getTextRect().getBottom() + offset);
	heightTextField->setX(getWidth() * 0.5f);
	addChild(heightTextField);

	// wieght
	spTextField weightTextField = createText(10, Vector2(getWidth() * 0.2f, getHeight() * 0.1f));
	weightTextField->setY(heightTextField->getY() + heightTextField->getTextRect().getBottom() + offset);
	weightTextField->setX(getWidth() * 0.5f);
	addChild(weightTextField);

	// class
	spTextField classTextField = createText(10, Vector2(getWidth() * 0.2f, getHeight() * 0.1f));
	classTextField->setY(weightTextField->getY() + weightTextField->getTextRect().getBottom() + offset);
	classTextField->setX(getWidth() * 0.5f);
	addChild(classTextField);

	// fun fact
	spTextField factTextField = createText(51, Vector2(getWidth() * 0.8f, getHeight() * 0.3f), true);
	factTextField->setAnchor(0.5f, 0.0f);
	factTextField->setY(classTextField->getY() + classTextField->getTextRect().getBottom() + offset);
	factTextField->setX(getWidth() / 2.0f);
	addChild(factTextField);

	// photos 1 - 3
}

void AnimalCardElement::setAnimalSlot(const string& spriteName) {
	if (!_animalSlot) {
		_animalSlot = new AnimalSlot(spriteName);
		_animalSlot->setPriority(0);
		_animalSlot->attachTo(this);
		setActorScaleBySize(_animalSlot, Vector2(getWidth() * ANIMALSLOT_WIDTH_PERCENT / 100.0f, getWidth() * ANIMALSLOT_WIDTH_PERCENT / 100.0f));
		_animalSlot->setPosition(getWidth() * 0.04f + _animalSlot->getDerivedWidth() * 0.5f, getWidth() * 0.04f + _animalSlot->getDerivedWidth() * 0.5f);
	}
	else {
		_animalSlot->switchAnimalSprite(spriteName);
	}
}

void AnimalCardElement::createBackground() {
	spBox9Sprite cardBackground = initActor(new Box9Sprite,
		arg_anchor = Vector2(0.5f, 0.5f),
		arg_resAnim = gameResources.getResAnim("progress_bar_box9"),
		arg_position = Vector2(getWidth() / 2.0f, getHeight() / 2.0f),
		arg_attachTo = this,
		arg_size = getSize(),
		arg_priority = -1);
	cardBackground->setHorizontalMode(Box9Sprite::STRETCHING);
	cardBackground->setVerticalMode(Box9Sprite::STRETCHING);
	cardBackground->setGuides(7, 37, 7, 37);
}

spTextField AnimalCardElement::createText(int lockitId, const Vector2& boundries, bool multiline) {
	TextStyle style;
	style.font = gameResources.getResFont("nobile_bold")->getFont();
	style.vAlign = TextStyle::VALIGN_MIDDLE;
	style.hAlign = TextStyle::HALIGN_CENTER;
	style.multiline = multiline;
	style.color = Color(35, 145, 245);

	return createTextFieldInBoundries(LanguageManager::instance.getText(lockitId), boundries, style);
}