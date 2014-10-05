#include "ZooFrame.h"

ZooFrame::ZooFrame() {
	init("LandingPageFrame.xml", true);
	selectTransitions();
}

void ZooFrame::selectTransitions() {
	spTransition transition = new TransitionFade;
	setTransitionIn(transition);
	setTransitionOut(transition);
}

void ZooFrame::_postHiding(Event *) {
	//FlurryAnalytics::instance.onLevelLeaveEvent(_whichLevel.c_str());
	_view->removeChildren();
	_resources.unload();
}

void ZooFrame::_preShowing(Event *) {
	//FlurryAnalytics::instance.onLevelEnterEvent(_whichLevel.c_str());
	selectTransitions();
	_resources.load();
	setData();
}

Action ZooFrame::loop() {
	while (1) {
		Action action = waitAction();
		if (action.id == "back" || action.id == "_btn_back_") {
			break;
		}
		else if (action.id == "close") {
			break;
		}
		else if (action.id == "result") {
			break; // todo
		}
	}

	return _lastAction;
}

//void ZooFrame::onFinished(Event *event) {
//	int random = int(FlashUtils::CMath::Rand(2, 4));
//	_field->restart(random, "dog");
//}

void ZooFrame::setData() {
	spColorRectSprite background = new ColorRectSprite();
	background->setColor(Color(88, 144, 217));
	background->setSize(_view->getSize());
	background->setAnchor(0.0f, 0.0f);
	background->setPosition(0.0f, 0.0f);
	background->attachTo(_view);
	background->setPriority(-10);
	if (_rotatingContainer) {
		_rotatingContainer->detach();
		_rotatingContainer = 0;
	}
	_rotatingContainer = new RotatingContainer();
	_rotatingContainer->setSize(_view->getWidth(), _view->getHeight());
	_rotatingContainer->setPriority(-5);

	spColorRectSprite rectangleContainer = new ColorRectSprite();
	rectangleContainer->setSize(_rotatingContainer->getWidth(), _rotatingContainer->getHeight());
	rectangleContainer->setColor(Color(0, 0, 0, 0));
	Point tilesCounter = Point(_view->getWidth() * 0.8f / 32.0f, _view->getHeight() * 0.8f / 32.0f);
	Vector2 fieldSize = Vector2(tilesCounter.x * 32, tilesCounter.y * 32);
	string animalsInside[] = {"tiger", "koala", "goat", "girafee", "octopus", "penguin"};
	int sizeOfArray = sizeof(animalsInside) / sizeof( animalsInside[0]);
	float positionX = 0.0f;
	float offset = 15.0f;
	float lastFieldWidth = 0.0f;

	for (int i = 0; i < sizeOfArray; i++) {
		spAnimalFarmField field = new AnimalFarmField(fieldSize);
		field->setData(animalsInside[i], CMath::random(5, 10));
		field->setAnchor(0.5f, 0.5f);
		field->setY(rectangleContainer->getHeight() / 2);
		if (positionX == 0.0f) {
			positionX = field->getDerivedWidth() / 2 + offset;
			field->setX(positionX);
			positionX += field->getDerivedWidth() + offset;
		}
		else {
			field->setX(positionX);
			positionX += field->getDerivedWidth() + offset;
		}
		//field->setPosition(getRoot()->getSize().x / 2 - field->getDerivedWidth() / 2, getRoot()->getSize().y / 2 - field->getDerivedHeight() / 2);
		//field->addEventListener(AnimalFarmField::FindShadowFieldEvent::SHADOW_FOUND, CLOSURE(this, &FindShadowFrame::onFinished));
		rectangleContainer->addChild(field);
		lastFieldWidth = field->getDerivedWidth();
	}

	rectangleContainer->setSize(positionX - lastFieldWidth / 2, _rotatingContainer->getHeight());
	_rotatingContainer->setContent(rectangleContainer);
	_rotatingContainer->setPosition(_view->getSize() / 2 - _rotatingContainer->getSize() / 2);
	_rotatingContainer->attachTo(_view);

	//_counterBox = new CounterBoxElement(Vector2(getRoot()->getWidth() * 0.9f, getRoot()->getHeight() * 0.15f), 120);
	//_counterBox->addEventListener(CounterBoxElement::CounterBoxEvent::TIME_END, CLOSURE(this, &MemoryFrame::onTimesUp));
	//_counterBox->show(true);
	//_view->addChild(_counterBox);

	addButton("back", "BACK", Vector2(getRoot()->getWidth() * 0.9f, getRoot()->getHeight() * 0.9f));
}