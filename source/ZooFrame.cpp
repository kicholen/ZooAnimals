#include "ZooFrame.h"
#include "FarmControlPanel.h"
#include "AnimalsManager.h"
#include "ChooseGameDifficultyFrame.h"
#include "PresentAnimalsFrame.h"
#include "MoneyManager.h"
#include "ExpManager.h"
#include "ProcessMaster.h"
#include "FeedAnimationProcess.h"
#include "SpectatorsArea.h"
#include "FarmManager.h"

#include "MemoryFrame.h"
#include "MatchTwoFrame.h"
#include "ConnectDotsFrame.h"
#include "FindShadowFrame.h"
#include "PopObjectsFrame.h"
#include "DiscoverImageFrame.h"

ZooFrame::ZooFrame(const std::string& regionName) {
	_region = regionName;
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
	AnimalsManager::instance.removeEventListener(AnimalsManager::AnimalEvent::ANIMAL_FED, CLOSURE(this, &ZooFrame::onAnimalFed));
	_view->removeChildren();
	_resources.unload();
	_farmArray._vector.resize(0);
}

void ZooFrame::_preShowing(Event *) {
	//FlurryAnalytics::instance.onLevelEnterEvent(_whichLevel.c_str());
	AnimalsManager::instance.addEventListener(AnimalsManager::AnimalEvent::ANIMAL_FED, CLOSURE(this, &ZooFrame::onAnimalFed));
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
			break;
		}
		else if (action.id == "memory" || action.id == "dots" || action.id == "shadow" || action.id == "match" || action.id == "pop" || action.id == "discover") {
			spChooseGameDifficultyFrame chooserFrame = new ChooseGameDifficultyFrame();

			Action innerAction = transitionShowFrameAsDialog(chooserFrame, 0, 0);

			if (innerAction.id == "back" || innerAction.id == "_btn_back_") {
				// do nothing
				int asd = 10;
			}
			else {
				if (action.id == "memory") {
					spMemoryFrame memoryFrame = new MemoryFrame(innerAction.id);
					transitionShowFrame(memoryFrame);
				}
				else if (action.id == "dots") {
					spConnectDotsFrame connectFrame = new ConnectDotsFrame(innerAction.id);
					transitionShowFrame(connectFrame);
				}
				else if (action.id == "shadow") {
					spFindShadowFrame findShadow = new FindShadowFrame(innerAction.id);
					transitionShowFrame(findShadow);
				}
				else if (action.id == "match") {
					spMatchTwoFrame matchTwo = new MatchTwoFrame(innerAction.id);
					transitionShowFrame(matchTwo);
				}
				else if (action.id == "pop") {
					spPopObjectsFrame pop = new PopObjectsFrame(innerAction.id);
					transitionShowFrame(pop);
				}
				else if (action.id == "discover") {
					spDiscoverImageFrame disc = new DiscoverImageFrame(innerAction.id);
					transitionShowFrame(disc);
				}
			}
		}
		else if (action.id == "present") {
			spPresentAnimalsFrame presentFrame = new PresentAnimalsFrame(_region);
			transitionShowFrameAsDialog(presentFrame);
		}
	}

	return _lastAction;
}

void ZooFrame::onAnimalFed(Event *event) {
	AnimalsManager::AnimalEvent *animalEvent = safeCast<AnimalsManager::AnimalEvent*>(event);

	spProcessMaster master = new ProcessMaster();
	master->addProcess(new FeedAnimationProcess(getFarmFieldByModel(animalEvent->model), _rotatingContainer));
	master->start(_view);
}

void ZooFrame::onFinished(Event *event) {
	FarmControlPanel::FarmControlPanelEvent *fcpe = safeCast<FarmControlPanel::FarmControlPanelEvent*>(event);
	generateAction(fcpe->_name);
}

void ZooFrame::onGameChosen(Event *event) {
	FarmServiceElement::FarmServiceElementEvent *ev = safeCast<FarmServiceElement::FarmServiceElementEvent*>(event);
	
	// TODO move it from here to end of the game
	spAnimalModel model = safeSpCast<AnimalFarmField>(event->currentTarget)->getModel();
	AnimalsManager::instance.increaseHappinessByPoints(model, 3);
	ExpManager::instance.increaseExpByPoints(3);
	MoneyManager::instance.increaseMoneyOnGameFinished(model->getLevel(), model->getGameValue(ev->_name), "easy");

	generateAction(ev->_name);
}

void ZooFrame::setData() {
	_farmArray._vector.resize(0);
	_farmArray._vector.reserve(10);

	spColorRectSprite background = new ColorRectSprite();
	background->setColor(Color(88, 144, 217));
	background->setSize(_view->getSize());
	background->setAnchor(0.0f, 0.0f);
	background->setPosition(0.0f, 0.0f);
	background->attachTo(_view);
	background->setPriority(-200);
	if (_rotatingContainer) {
		_rotatingContainer->detach();
		_rotatingContainer = 0;
	}
	_rotatingContainer = new SlidingActor();
	_rotatingContainer->setSize(_view->getWidth(), _view->getHeight());
	_rotatingContainer->setPriority(-5);

	spColorRectSprite rectangleContainer = new ColorRectSprite();
	rectangleContainer->setSize(_rotatingContainer->getWidth(), _rotatingContainer->getHeight());
	rectangleContainer->setColor(Color(0, 0, 0, 0));

	int farmHeight = (int)floor(_view->getHeight() * 0.75f / 32.0f);
	// ratio 1.5
	Vector2 fieldSize = Vector2(farmHeight * 32.0f * 1.5f, farmHeight * 32.0f);
	float bottomEmptySpace = _view->getHeight() - fieldSize.y;
	float positionX = 0.0f;
	float offset = 0.0f;
	float lastFieldWidth = 0.0f;

	animalMap animaMap = AnimalsManager::instance.getPossesedAnimalsByRegion(_region);
	for (animalMap::iterator innerIterator = animaMap.begin(); innerIterator != animaMap.end(); ++innerIterator) {
		spAnimalFarmField field = new AnimalFarmField(fieldSize);
		field->setCull(true);
		_farmArray.push(field);
		field->setData(innerIterator->second);
		field->setAnchor(0.5f, 0.5f);
		field->setY(field->getDerivedHeight() / 2);
		field->addEventListener(FarmServiceElement::FarmServiceElementEvent::PLAY_GAMES, CLOSURE(this, &ZooFrame::onGameChosen));
			
		spTileField tileField = new TileField(Point(field->getNumberOfTiles().x, 3));
		tileField->setData("pavement", "first");
		tileField->setScale(fieldSize.x / tileField->getWidth());
		tileField->setName("tajlee");
		tileField->setAnchor(0.5f, 0.0f);
		tileField->setY(fieldSize.y);
		tileField->setPriority(-50);
		tileField->setCull(true);

		rectangleContainer->addChild(tileField);

		if (positionX == 0.0f) {
			positionX = field->getDerivedWidth() / 2 + offset;
			field->setX(positionX);
			tileField->setX(positionX);
			positionX += field->getDerivedWidth() + offset;
		}
		else {
			field->setX(positionX);
			tileField->setX(positionX);
			positionX += field->getDerivedWidth() + offset;
		}
		rectangleContainer->addChild(field);
		lastFieldWidth = field->getDerivedWidth();
	}

	spSpectatorsArea area = new SpectatorsArea();
	area->setSize(positionX - lastFieldWidth / 2, bottomEmptySpace);
	area->setY(fieldSize.y);
	area->init(FarmManager::instance.getFarmHumanCount());
	rectangleContainer->addChild(area);

	_rotatingContainer->setLocked(true);
	rectangleContainer->setSize(positionX - lastFieldWidth / 2, _rotatingContainer->getHeight());
	_rotatingContainer->setContent(rectangleContainer);
	_rotatingContainer->setPosition(_view->getSize() / 2 - _rotatingContainer->getSize() / 2);
	_rotatingContainer->attachTo(_view);

	/*
	spFarmControlPanel controlPanel = new FarmControlPanel(_view->getSize(), Vector2(getRoot()->getWidth() * 0.10f, getRoot()->getHeight() * 0.95f));
	_view->addChild(controlPanel);
	controlPanel->addEventListener(FarmControlPanel::FarmControlPanelEvent::PLAY_GAMES, CLOSURE(this, &ZooFrame::onFinished));
	controlPanel->show();
	*/
	spTweenButton button = addButton("back", "BACK", Vector2(getRoot()->getWidth() * 0.9f, getRoot()->getHeight() * 0.9f));
	addButton("present", "present", Vector2(getRoot()->getWidth() * 0.9f, getRoot()->getHeight() * 0.9f - button->getDerivedWidth()));

	addButton("store", "store", Vector2(getRoot()->getWidth() * 0.6f, getRoot()->getHeight() * 0.9f));
}

spAnimalFarmField ZooFrame::getFarmFieldByModel(spAnimalModel model) {
	for (int i = 0; i < _farmArray.length(); i++) {
		if (_farmArray[i]->getModel() == model) {
			return _farmArray[i];
		}
	}

	return 0;
}