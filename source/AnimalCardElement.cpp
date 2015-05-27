#include "AnimalCardElement.h"
#include "SharedResources.h"
#include "LanguageManager.h"

AnimalCardElement::AnimalCardElement(const Vector2& size, spAnimalModel model, ResAnim* thumbRes) {
	setSize(size);
	setAnchor(0.5f, 0.5f);
	_thumbRes = thumbRes;
	const string& resurceSufix = _thumbRes->getName() + ".xml";
	_realAnimalResource.loadXML("xmls/animals/" + resurceSufix, 0, false);
	_state = acsUnzoomed;

	setData(model);
	setTouchEnabled(false);
}

AnimalCardElement::~AnimalCardElement() {
	_realAnimalResource.unload();
	_realAnimalResource.free();
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
	nameTextField->setTouchEnabled(false);
	addChild(nameTextField);

	// height
	spTextField heightTextField = createText(10, Vector2(getWidth() * 0.2f, getHeight() * 0.1f));
	heightTextField->setY(nameTextField->getY() + nameTextField->getTextRect().getBottom() + offset);
	heightTextField->setX(getWidth() * 0.5f);
	heightTextField->setTouchEnabled(false);
	addChild(heightTextField);

	// wieght
	spTextField weightTextField = createText(10, Vector2(getWidth() * 0.2f, getHeight() * 0.1f));
	weightTextField->setY(heightTextField->getY() + heightTextField->getTextRect().getBottom() + offset);
	weightTextField->setTouchEnabled(false);
	weightTextField->setX(getWidth() * 0.5f);
	addChild(weightTextField);

	// class
	spTextField classTextField = createText(10, Vector2(getWidth() * 0.2f, getHeight() * 0.1f));
	classTextField->setY(weightTextField->getY() + weightTextField->getTextRect().getBottom() + offset);
	classTextField->setTouchEnabled(false);
	classTextField->setX(getWidth() * 0.5f);
	addChild(classTextField);

	// fun fact
	spTextField factTextField = createText(51, Vector2(getWidth() * 0.8f, getHeight() * 0.3f), true);
	factTextField->setAnchor(0.5f, 0.0f);
	factTextField->setY(classTextField->getY() + classTextField->getTextRect().getBottom() + offset);
	factTextField->setTouchEnabled(false);
	factTextField->setX(getWidth() / 2.0f);
	addChild(factTextField);

	// photos 1
	_photo = new Sprite();
	_photo->setPriority(30);
	_photo->setResAnim(_thumbRes);
	_photo->setAnchor(0.5f, 0.5f);
	_photo->setScale(getActorScaleBySize(_photo, Vector2(getWidth() * 0.8f, getHeight() * 0.25f)));
	_photo->setPosition(getWidth() / 2.0f, factTextField->getY() + factTextField->getTextRect().getBottom() + offset + _photo->getDerivedHeight() / 2.0f);
	_photo->addEventListener(TouchEvent::CLICK, CLOSURE(this, &AnimalCardElement::onPhotoClicked));
	_photo->attachTo(this);

	_touchBlocker = new TouchBlock(getRoot()->getSize() * 1.2f);
	_touchBlocker->setAnchor(0.5f, 0.5f);
	_touchBlocker->setPosition(getSize() / 2.0f);
	_touchBlocker->setPriority(20);
	addChild(_touchBlocker);
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
	cardBackground->setTouchEnabled(false);
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

void AnimalCardElement::onPhotoClicked(Event *ev) {
	if (_state != acsAnimating) {
		if (_state == acsUnzoomed) {
			spThreadLoading threadLoader = new ThreadLoading();
			threadLoader->addEventListener(ThreadLoading::COMPLETE, CLOSURE(this, &AnimalCardElement::onResourceLoaded));
			threadLoader->add(&_realAnimalResource);
			threadLoader->start(this);

			_baseScale = _photo->getScale();
			_basePosition = _photo->getPosition();
			_photo->addTween(Actor::TweenPosition(getSize() / 2.0f), 500);
			_photo->addTween(Actor::TweenScale(getActorScaleBySize(_photo, getRoot()->getSize() * 0.9f)), 600, 1, false, 0, Tween::ease_inCirc)->addDoneCallback(CLOSURE(this, &AnimalCardElement::onZoomEnded));
		}
		else {
			_realAnimalResource.unload();
			_photo->removeTweens();
			const Vector2& sizeBefore = _photo->getDerivedSize();
			_photo->setResAnim(_thumbRes);
			_photo->setScale(getActorScaleBySize(_photo, sizeBefore));
			_photo->addTween(Actor::TweenPosition(_basePosition), 500);
			_photo->addTween(Actor::TweenScale(_baseScale), 600, 1, false, 0, Tween::ease_inCirc)->addDoneCallback(CLOSURE(this, &AnimalCardElement::onUnzoomEnded));
		}
		_touchBlocker->changeState();
		_state = acsAnimating;
	}
}

// todo probably should w8 here, make counter if boh actions are completed
void AnimalCardElement::onResourceLoaded(Event *ev) {
	if (_photo) {
		const Vector2& sizeBefore = _photo->getDerivedSize();
		Vector2 positionBefore = _photo->getPosition();
		_photo->setResAnim(_realAnimalResource.getResAnim(_animalSlot->getId()));
		_photo->removeTweens(true);
		_photo->setScale(getActorScaleBySize(_photo, sizeBefore));
		_photo->setPosition(positionBefore);

		_photo->addTween(Actor::TweenPosition(getSize() / 2.0f), 500);
		_photo->addTween(Actor::TweenScale(getActorScaleBySize(_photo, getRoot()->getSize() * 0.9f)), 600, 1, false, 0, Tween::ease_inCirc);
	}
}

void AnimalCardElement::onZoomEnded(Event *ev) {
	_state = acsZoomed;
}

void AnimalCardElement::onUnzoomEnded(Event *ev) {
	_state = acsUnzoomed;
}