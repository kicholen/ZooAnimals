#include "ZooGateFrame.h"
#include "TileField.h"
#include "FlashUtils.h"
#include "AnimalsManager.h"
#include "ZooFrame.h"

using namespace FlashUtils;

ZooGateFrame::ZooGateFrame() {
	init("LandingPageFrame.xml", true);
	selectTransitions();
}

void ZooGateFrame::selectTransitions() {
	spTransition transition = new TransitionFade;
	setTransitionIn(transition);
	setTransitionOut(transition);
}

void ZooGateFrame::_postHiding(Event *) {
	//FlurryAnalytics::instance.onLevelLeaveEvent(_whichLevel.c_str());
	_view->removeChildren();
	_resources.unload();
}

void ZooGateFrame::_preShowing(Event *) {
	//FlurryAnalytics::instance.onLevelEnterEvent(_whichLevel.c_str());
	selectTransitions();
	_resources.load();
	setData();
}

Action ZooGateFrame::loop() {
	while (1) {
		Action action = waitAction();
		if (action.id == "back" || action.id == "_btn_back_") {
			break;
		}
		else if (action.id == "close") {
			break;
		}
		else if (action.id == "farm" || action.id == "winter" || action.id == "underwater" || action.id == "steppe" || action.id == "asia" || action.id == "australia") {
			spZooFrame zooFrame = new ZooFrame(action.id);
			transitionShowFrame(zooFrame);
		}
	}

	return _lastAction;
}

void ZooGateFrame::onZooSectionChosen(Event *event) {
	//FarmServiceElement::FarmServiceElementEvent *ev = safeCast<FarmServiceElement::FarmServiceElementEvent*>(event);
	//AnimalsManager::instance.increaseHappinessByPoints(safeSpCast<AnimalFarmField>(event->currentTarget)->getModel(), 3);
	//generateAction(event->_name);
}

void ZooGateFrame::setData() {
	float tileSize = _view->getHeight() / 15;

	spTileField tileField = new TileField(Point(27, 15), true);
	float scale = _view->getHeight() / tileField->getHeight();
	tileField->setData("zoo_first_ground");
	tileField->setScale(scale);
	tileField->setName("ground_tiles");
	tileField->setAnchor(0.5f, 1.0f);
	tileField->setY(_view->getHeight());
	tileField->setX(_view->getWidth() / 2);
	tileField->setPriority(-51);
	tileField->attachTo(_view);

	float tilesToViewOffsetX = (tileField->getDerivedWidth() - _view->getWidth()) / 2;

	spTileField tileFieldObjects = new TileField(Point(27, 15), true);
	tileFieldObjects->setData("zoo_first_objects");
	tileFieldObjects->setScale(scale);
	tileFieldObjects->setName("object_tiles");
	tileFieldObjects->setAnchor(0.5f, 1.0f);
	tileFieldObjects->setY(_view->getHeight());
	tileFieldObjects->setX(_view->getWidth() / 2);
	tileFieldObjects->setPriority(-50);
	tileFieldObjects->attachTo(_view);

	_spawner = new SpectatorSpawner(25, tileSize * 0.8f);
	_spawner->setTouchChildrenEnabled(false);
	_spawner->setTouchEnabled(false);
	_spawner->setSize(_view->getSize());
	_view->addChild(_spawner);
	
	addBaseTrackToSpawner(tileSize);

	if (AnimalsManager::instance.isRegionPopulated("farm")) {
		addFarmTrackToSpawner(tileSize, tilesToViewOffsetX);
		addSignPost(tileSize, "farm", Vector2(tileSize / 2.0f, tileSize * 10.0f - tileSize / 2.0f));
	}
	if (AnimalsManager::instance.isRegionPopulated("winter")) {
		addWinterTrackToSpawner(tileSize, tilesToViewOffsetX);
		addSignPost(tileSize, "winter", Vector2(tileSize * 8.0f + tileSize / 2.0f - tilesToViewOffsetX, tileSize / 2.0f));
	}
	if (AnimalsManager::instance.isRegionPopulated("underwater")) {
		addUnderwaterTrackToSpawner(tileSize, tilesToViewOffsetX);
		addSignPost(tileSize, "underwater", Vector2(_view->getWidth() - tileSize / 2.0f, tileSize / 2.0f));
	}
	if (AnimalsManager::instance.isRegionPopulated("steppe")) {
		addSteppeTrackToSpawner(tileSize, tilesToViewOffsetX);
		addSignPost(tileSize, "steppe", Vector2(tileSize / 2.0f, tileSize / 2.0f + 3.0f * tileSize));
	}
	if (AnimalsManager::instance.isRegionPopulated("asia")) {
		addAsiaTrackToSpawner(tileSize, tilesToViewOffsetX);
		addSignPost(tileSize, "asia", Vector2(tileSize * 15.0f + tileSize / 2.0f - tilesToViewOffsetX, tileSize / 2.0f));
	}
	if (AnimalsManager::instance.isRegionPopulated("australia")) {
		addAustraliaTrackToSpawner(tileSize, tilesToViewOffsetX);
		addSignPost(tileSize, "australia", Vector2(_view->getWidth() - tileSize / 2.0f, tileSize / 2.0f + 3.0f * tileSize));
	}

	addPossibleSpritesToSpawner();

	/*
	spFarmControlPanel controlPanel = new FarmControlPanel(_view->getSize(), Vector2(getRoot()->getWidth() * 0.10f, getRoot()->getHeight() * 0.95f));
	_view->addChild(controlPanel);
	controlPanel->addEventListener(FarmControlPanel::FarmControlPanelEvent::PLAY_GAMES, CLOSURE(this, &ZooFrame::onFinished));
	controlPanel->show();
	*/
	spTweenButton button = addButton("back", "BACK", Vector2(getRoot()->getWidth() * 0.9f, getRoot()->getHeight() * 0.9f));
}

void ZooGateFrame::addBaseTrackToSpawner(float tileSize) {
	VectorArray<Vector2> trackVector;
	// main track
	trackVector.push(Vector2(0.0f, _view->getHeight() - tileSize / 2));
	trackVector.push(Vector2(_view->getWidth(), _view->getHeight() - tileSize / 2));
	_spawner->addTrack(trackVector);
	trackVector.clear();

	// reverted main track
	trackVector.push(Vector2(_view->getWidth(), _view->getHeight() - tileSize / 2));
	trackVector.push(Vector2(0.0f, _view->getHeight() - tileSize / 2));
	_spawner->addTrack(trackVector);
	trackVector.clear();
}

void ZooGateFrame::addFarmTrackToSpawner(float tileSize, float tilesToViewOffsetX) {
	VectorArray<Vector2> trackVector;
	// entering zoo
	trackVector.push(Vector2(0.0f, _view->getHeight() - tileSize / 2 - 4 * tileSize));
	trackVector.push(Vector2(tileSize * 14 + tileSize / 2 - tilesToViewOffsetX, _view->getHeight() - tileSize / 2 - 4 * tileSize));
	trackVector.push(Vector2(tileSize * 14 + tileSize / 2 - tilesToViewOffsetX, _view->getHeight() - tileSize / 2));
	trackVector.push(Vector2(0.0f, _view->getHeight() - tileSize / 2));
	_spawner->addTrack(trackVector);
	trackVector.clear();
	
	// reverted entering zoo
	trackVector.push(Vector2(0.0f, _view->getHeight() - tileSize / 2 - 4 * tileSize));
	trackVector.push(Vector2(tileSize * 14 + tileSize / 2 - tilesToViewOffsetX, _view->getHeight() - tileSize / 2 - 4 * tileSize));
	trackVector.push(Vector2(tileSize * 14 + tileSize / 2 - tilesToViewOffsetX, _view->getHeight() - tileSize / 2));
	trackVector.push(Vector2(_view->getWidth(), _view->getHeight() - tileSize / 2));
	_spawner->addTrack(trackVector);
	trackVector.clear();
}

void ZooGateFrame::addWinterTrackToSpawner(float tileSize, float tilesToViewOffsetX) {
	VectorArray<Vector2> trackVector;
	// entering zoo
	trackVector.push(Vector2(tileSize * 7 + tileSize / 2 - tilesToViewOffsetX, 0.0f));
	trackVector.push(Vector2(tileSize * 7 + tileSize / 2 - tilesToViewOffsetX, tileSize / 2 + 2 * tileSize));
	trackVector.push(Vector2(tileSize * 9 + tileSize / 2 - tilesToViewOffsetX, tileSize / 2 + 2 * tileSize));
	trackVector.push(Vector2(tileSize * 9 + tileSize / 2 - tilesToViewOffsetX, tileSize / 2 + 4 * tileSize));
	trackVector.push(Vector2(tileSize * 9 + tileSize / 2 - tilesToViewOffsetX, _view->getHeight() - tileSize / 2 - 4 * tileSize));
	trackVector.push(Vector2(tileSize * 14 + tileSize / 2 - tilesToViewOffsetX, _view->getHeight() - tileSize / 2 - 4 * tileSize));
	trackVector.push(Vector2(tileSize * 14 + tileSize / 2 - tilesToViewOffsetX, _view->getHeight() - tileSize / 2));
	trackVector.push(Vector2(0.0f, _view->getHeight() - tileSize / 2));
	_spawner->addTrack(trackVector);
	trackVector.clear();

	// reverted entering zoo
	trackVector.push(Vector2(tileSize * 7 + tileSize / 2 - tilesToViewOffsetX, 0.0f));
	trackVector.push(Vector2(tileSize * 7 + tileSize / 2 - tilesToViewOffsetX, tileSize / 2 + 2 * tileSize));
	trackVector.push(Vector2(tileSize * 9 + tileSize / 2 - tilesToViewOffsetX, tileSize / 2 + 2 * tileSize));
	trackVector.push(Vector2(tileSize * 9 + tileSize / 2 - tilesToViewOffsetX, tileSize / 2 + 4 * tileSize));
	trackVector.push(Vector2(tileSize * 9 + tileSize / 2 - tilesToViewOffsetX, _view->getHeight() - tileSize / 2 - 4 * tileSize));
	trackVector.push(Vector2(tileSize * 14 + tileSize / 2 - tilesToViewOffsetX, _view->getHeight() - tileSize / 2 - 4 * tileSize));
	trackVector.push(Vector2(tileSize * 14 + tileSize / 2 - tilesToViewOffsetX, _view->getHeight() - tileSize / 2));
	trackVector.push(Vector2(_view->getWidth(), _view->getHeight() - tileSize / 2));
	_spawner->addTrack(trackVector);
	trackVector.clear();
}

void ZooGateFrame::addUnderwaterTrackToSpawner(float tileSize, float tilesToViewOffsetX) {
	VectorArray<Vector2> trackVector;
	// entering zoo
	trackVector.push(Vector2(_view->getWidth(), tileSize / 2 + 1 * tileSize));
	trackVector.push(Vector2(tileSize * 18 + tileSize / 2 - tilesToViewOffsetX, tileSize / 2 + 1 * tileSize));
	trackVector.push(Vector2(tileSize * 18 + tileSize / 2 - tilesToViewOffsetX, tileSize / 2 + 4 * tileSize));
	trackVector.push(Vector2(tileSize * 14 + tileSize / 2 - tilesToViewOffsetX, tileSize / 2 + 4 * tileSize));
	trackVector.push(Vector2(tileSize * 14 + tileSize / 2 - tilesToViewOffsetX, _view->getHeight() - tileSize / 2));
	trackVector.push(Vector2(0.0f, _view->getHeight() - tileSize / 2));
	_spawner->addTrack(trackVector);
	trackVector.clear();

	// reverted entering zoo
	trackVector.push(Vector2(_view->getWidth(), tileSize / 2 + 1 * tileSize));
	trackVector.push(Vector2(tileSize * 18 + tileSize / 2 - tilesToViewOffsetX, tileSize / 2 + 1 * tileSize));
	trackVector.push(Vector2(tileSize * 18 + tileSize / 2 - tilesToViewOffsetX, tileSize / 2 + 4 * tileSize));
	trackVector.push(Vector2(tileSize * 14 + tileSize / 2 - tilesToViewOffsetX, tileSize / 2 + 4 * tileSize));
	trackVector.push(Vector2(tileSize * 14 + tileSize / 2 - tilesToViewOffsetX, _view->getHeight() - tileSize / 2));
	trackVector.push(Vector2(_view->getWidth(), _view->getHeight() - tileSize / 2));
	_spawner->addTrack(trackVector);
	trackVector.clear();
}

void ZooGateFrame::addSteppeTrackToSpawner(float tileSize, float tilesToViewOffsetX) {
	VectorArray<Vector2> trackVector;
	// entering zoo
	trackVector.push(Vector2(0.0f, tileSize / 2 + 4 * tileSize));
	trackVector.push(Vector2(tileSize * 9 + tileSize / 2 - tilesToViewOffsetX, tileSize / 2 + 4 * tileSize));
	trackVector.push(Vector2(tileSize * 9 + tileSize / 2 - tilesToViewOffsetX, _view->getHeight() - tileSize / 2 - 4 * tileSize));

	trackVector.push(Vector2(tileSize * 14 + tileSize / 2 - tilesToViewOffsetX, _view->getHeight() - tileSize / 2 - 4 * tileSize));
	trackVector.push(Vector2(tileSize * 14 + tileSize / 2 - tilesToViewOffsetX, _view->getHeight() - tileSize / 2));
	trackVector.push(Vector2(0.0f, _view->getHeight() - tileSize / 2));
	_spawner->addTrack(trackVector);
	trackVector.clear();

	// reverted entering zoo
	trackVector.push(Vector2(0.0f, tileSize / 2 + 4 * tileSize));
	trackVector.push(Vector2(tileSize * 9 + tileSize / 2 - tilesToViewOffsetX, tileSize / 2 + 4 * tileSize));
	trackVector.push(Vector2(tileSize * 9 + tileSize / 2 - tilesToViewOffsetX, _view->getHeight() - tileSize / 2 - 4 * tileSize));

	trackVector.push(Vector2(tileSize * 14 + tileSize / 2 - tilesToViewOffsetX, _view->getHeight() - tileSize / 2 - 4 * tileSize));
	trackVector.push(Vector2(tileSize * 14 + tileSize / 2 - tilesToViewOffsetX, _view->getHeight() - tileSize / 2));
	trackVector.push(Vector2(_view->getWidth(), _view->getHeight() - tileSize / 2));
	_spawner->addTrack(trackVector);
	trackVector.clear();
}

void ZooGateFrame::addAsiaTrackToSpawner(float tileSize, float tilesToViewOffsetX) {
	VectorArray<Vector2> trackVector;
	// entering zoo
	trackVector.push(Vector2(tileSize * 14 + tileSize / 2 - tilesToViewOffsetX, 0.0f));
	trackVector.push(Vector2(tileSize * 14 + tileSize / 2 - tilesToViewOffsetX, _view->getHeight() - tileSize / 2));
	trackVector.push(Vector2(0.0f, _view->getHeight() - tileSize / 2));
	_spawner->addTrack(trackVector);
	trackVector.clear();

	// reverted entering zoo
	trackVector.push(Vector2(tileSize * 14 + tileSize / 2 - tilesToViewOffsetX, 0.0f));
	trackVector.push(Vector2(tileSize * 14 + tileSize / 2 - tilesToViewOffsetX, _view->getHeight() - tileSize / 2));
	trackVector.push(Vector2(_view->getWidth(), _view->getHeight() - tileSize / 2));
	_spawner->addTrack(trackVector);
	trackVector.clear();
}

void ZooGateFrame::addAustraliaTrackToSpawner(float tileSize, float tilesToViewOffsetX) {
	VectorArray<Vector2> trackVector;
	// entering zoo
	trackVector.push(Vector2(_view->getWidth(), tileSize / 2 + 4 * tileSize));
	trackVector.push(Vector2(tileSize * 14 + tileSize / 2 - tilesToViewOffsetX, tileSize / 2 + 4 * tileSize));
	trackVector.push(Vector2(tileSize * 14 + tileSize / 2 - tilesToViewOffsetX, _view->getHeight() - tileSize / 2));
	trackVector.push(Vector2(0.0f, _view->getHeight() - tileSize / 2));
	_spawner->addTrack(trackVector);
	trackVector.clear();

	// reverted entering zoo
	trackVector.push(Vector2(_view->getWidth(), tileSize / 2 + 4 * tileSize));
	trackVector.push(Vector2(tileSize * 14 + tileSize / 2 - tilesToViewOffsetX, tileSize / 2 + 4 * tileSize));
	trackVector.push(Vector2(tileSize * 14 + tileSize / 2 - tilesToViewOffsetX, _view->getHeight() - tileSize / 2));
	trackVector.push(Vector2(_view->getWidth(), _view->getHeight() - tileSize / 2));
	_spawner->addTrack(trackVector);
	trackVector.clear();
}

void ZooGateFrame::addSignPost(float tileSize, const std::string& region, const Vector2& position) {
	spButton button = new Button();
	button->setName(region);
	button->setResAnim(tilesResources.getResAnim("informationTable"));
	button->setAnchor(Vector2(0.5, 0.5));
	button->setTouchChildrenEnabled(false);
	setSpriteScaleBySize(button, Vector2(tileSize, tileSize));
	spTextField text = new TextField();
	text->setStyle(createTextStyle(gameResources.getResFont("nobile_bold")->getFont(), Color(255, 255, 255, 255), false, TextStyle::HALIGN_CENTER, TextStyle::VALIGN_MIDDLE));
	text->setText(region);
	text->setSize(button->getDerivedSize());
	button->addChild(text);
	button->attachTo(_view);
	button->setPosition(position);
	button->addEventListener(TouchEvent::CLICK, CLOSURE(this, &ZooGateFrame::onButtonClicked));
}

void ZooGateFrame::addPossibleSpritesToSpawner() {
	_spawner->addResAnim("human_1");
	_spawner->addResAnim("human_2");
	_spawner->addResAnim("human_3");
	_spawner->addResAnim("human_4");
	_spawner->addResAnim("human_5");
	_spawner->addResAnim("human_6");
	_spawner->addResAnim("human_7");
}

float ZooGateFrame::getXOnTilesToView(float x) {
	return 0.0f;
}

void ZooGateFrame::onButtonClicked(Event *ev) {
	generateAction(ev->target->getName());
}