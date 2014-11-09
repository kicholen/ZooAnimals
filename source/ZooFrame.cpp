#include "ZooFrame.h"
#include "FarmControlPanel.h"

#include "ChooseMemoryDifficultyFrame.h"
#include "MatchTwoFrame.h"
#include "ConnectDotsFrame.h"
#include "FindShadowFrame.h"

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
	_farmArray._vector.resize(0);
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
		else if (action.id == "tiles") {
			if (_shouldRemoveTiles) {
				for (int i = _farmArray.length() - 1; i >= 0; i -= 1) {
					_farmArray[i]->getChildT<TileField>("tajle", oxygine::ep_ignore_error)->detach();
				}
			}
			else {
				for (int i = _farmArray.length() - 1; i >= 0; i -= 1) {
					_farmArray[i]->createTileField();
				}
			}
			_shouldRemoveTiles = !_shouldRemoveTiles;
		}
		else if (action.id == "memory") {
			spChooseMemoryDifficultyFrame chooserFrame = new ChooseMemoryDifficultyFrame();
			transitionShowFrame(chooserFrame);
		}
		else if (action.id == "dots") {
			spConnectDotsFrame connectFrame = new ConnectDotsFrame();
			transitionShowFrame(connectFrame);
		}
		else if (action.id == "shadow") {
			spFindShadowFrame findShadow = new FindShadowFrame();
			transitionShowFrame(findShadow);
		}
		else if (action.id == "match") {
			spMatchTwoFrame matchTwo = new MatchTwoFrame();
			transitionShowFrame(matchTwo);
		}
	}

	return _lastAction;
}

void ZooFrame::onFinished(Event *event) {
	FarmControlPanel::FarmControlPanelEvent *fcpe = safeCast<FarmControlPanel::FarmControlPanelEvent*>(event);
	generateAction(fcpe->_name);
}

void ZooFrame::setData() {
	_shouldRemoveTiles = true;
	_farmArray._vector.resize(0);
	_farmArray._vector.reserve(10);


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

	int farmHeight = (int)floor(_view->getHeight() * 0.75f / 32.0f);
	// ratio 1.5
	Vector2 fieldSize = Vector2(farmHeight * 32.0f * 1.5f, farmHeight * 32.0f);
	float bottomEmptySpace = _view->getHeight() - fieldSize.y;
	//Point tilesCounter = Point(floor(_view->getWidth() / 32.0f), floor(_view->getHeight() / 32.0f));
	//Vector2 fieldSize = Vector2(tilesCounter.x * 32, tilesCounter.y * 32);
	string animalsInside[] = {"tiger", "koala", "goat", "girafee", "octopus", "penguin"};//{"tiger", "monkey_2", "peacock", "parrot", "panda", "yak", "koala", "ostrich", "kangaroo", "kiwi", "tealplatypus", "crocodile", "goat", "cow", "sheep", "horse", "duck", "cat", "lion", "zebra", "rhino", "leopard", "gnu", "girafee", "octopus", "dolphin", "whale", "fish_1", "penguin", "polarbear", "wolf", "owl_winter", "walrus", "young_seal"};
	int sizeOfArray = sizeof(animalsInside) / sizeof( animalsInside[0]);
	float positionX = 0.0f;
	float offset = 15.0f;
	float lastFieldWidth = 0.0f;
	bool isUp = true;

	for (int i = 0; i < sizeOfArray; i++) {
		spAnimalFarmField field = new AnimalFarmField(fieldSize);
		_farmArray.push(field);
		field->setData(animalsInside[i], 25);//CMath::random(5, 10));
		field->setAnchor(0.5f, 0.5f);
		field->setY(field->getDerivedHeight() / 2);
		//field->setY(isUp ? rectangleContainer->getHeight() -  field->getDerivedHeight() / 2 : field->getDerivedHeight() / 2);
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
		isUp = !isUp;
	}

	/*for (int i = 0; i < sizeOfArray; i++) {
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
	}*/
	spColorRectSprite tempSpace = new ColorRectSprite();
	tempSpace->setColor(Color(152, 12, 42, 255));
	tempSpace->setSize(positionX - lastFieldWidth / 2, bottomEmptySpace);
	tempSpace->setAnchor(0.0f, 0.0f);
	tempSpace->setPosition(0.0f, _view->getHeight() - bottomEmptySpace);
	rectangleContainer->addChild(tempSpace);

	rectangleContainer->setSize(positionX - lastFieldWidth / 2, _rotatingContainer->getHeight());
	_rotatingContainer->setContent(rectangleContainer);
	_rotatingContainer->setPosition(_view->getSize() / 2 - _rotatingContainer->getSize() / 2);
	_rotatingContainer->attachTo(_view);

	//_counterBox = new CounterBoxElement(Vector2(getRoot()->getWidth() * 0.9f, getRoot()->getHeight() * 0.15f), 120);
	//_counterBox->addEventListener(CounterBoxElement::CounterBoxEvent::TIME_END, CLOSURE(this, &MemoryFrame::onTimesUp));
	//_counterBox->show(true);
	//_view->addChild(_counterBox);

	spFarmControlPanel controlPanel = new FarmControlPanel(_view->getSize(), Vector2(getRoot()->getWidth() * 0.10f, getRoot()->getHeight() * 0.95f));
	_view->addChild(controlPanel);
	controlPanel->addEventListener(FarmControlPanel::FarmControlPanelEvent::PLAY_GAMES, CLOSURE(this, &ZooFrame::onFinished));
	controlPanel->show();

	spTweenButton button = addButton("back", "BACK", Vector2(getRoot()->getWidth() * 0.9f, getRoot()->getHeight() * 0.9f));
	addButton("tiles", "remove/add tiles", Vector2(getRoot()->getWidth() * 0.9f, getRoot()->getHeight() * 0.9f - button->getDerivedWidth()));
}