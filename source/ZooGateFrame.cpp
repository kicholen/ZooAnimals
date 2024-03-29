#include "ZooGateFrame.h"
#include "TileField.h"
#include "FlashUtils.h"
#include "AnimalsManager.h"
#include "ZooFrame.h"
#include "StartGameConfig.h"
#include "FarmManager.h"
#include "MessageCenterFrame.h"
#include "MessageCenterManager.h"
#include "ProcessMaster.h"
#include "AnimateFromToProcess.h"
#include "SpectatorsArea.h"

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
	MessageCenterManager::instance.removeEventListener(MessageCenterManager::MessageEvent::MESSAGE_COUNT_CHANGE, CLOSURE(this, &ZooGateFrame::onMessageCountChanged));
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
		else if (action.id == "mailbox") {
			spMessageCenterFrame messageFrame = new MessageCenterFrame();
			transitionShowFrameAsDialog(messageFrame);
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
	float tileSize = _view->getHeight() / 15.0f;

	spTileField tileField = new TileField(Point(27, 15), true);
	float scale = _view->getHeight() / tileField->getHeight();
	tileField->setScale(scale);
	tileField->setName("ground_tiles");
	tileField->setAnchor(0.5f, 1.0f);
	tileField->setY(_view->getHeight());
	tileField->setX(_view->getWidth() / 2);
	tileField->setPriority(-51);
	tileField->attachTo(_view);

	float tilesToViewOffsetX = (tileField->getDerivedWidth() - _view->getWidth()) / 2.0f;

	spTileField tileFieldObjects = new TileField(Point(27, 15), true);
	tileFieldObjects->setData("zoo_first_objects", "first");
	tileFieldObjects->setScale(scale);
	tileFieldObjects->setName("object_tiles");
	tileFieldObjects->setAnchor(0.5f, 1.0f);
	tileFieldObjects->setY(_view->getHeight());
	tileFieldObjects->setX(_view->getWidth() / 2);
	tileFieldObjects->setPriority(-50);
	tileFieldObjects->attachTo(_view);

	spTileField tileFieldFence = new TileField(Point(27, 15), true);
	tileFieldFence->setData("zoo_first_fence", "first");
	tileFieldFence->setScale(scale);
	tileFieldFence->setAnchor(0.5f, 1.0f);
	tileFieldFence->setY(_view->getHeight());
	tileFieldFence->setX(_view->getWidth() / 2);
	tileFieldFence->setPriority(-40);
	tileFieldFence->attachTo(_view);

	_spawner = new SpectatorSpawner(FarmManager::instance.getGateHumanCount(), tileSize * 0.8f);
	_spawner->setTouchChildrenEnabled(false);
	_spawner->setTouchEnabled(false);
	_spawner->setSize(_view->getSize());
	_view->addChild(_spawner);
	addMailBox(tileSize, Vector2(tileSize / 2.0f + tileSize * 15.0f - tilesToViewOffsetX, tileSize * 10.0f - tileSize / 2.0f));
	addBaseTrackToSpawner(tileSize);

	int regionsCount = 0;
	if (AnimalsManager::instance.isRegionPopulated("farm")) {
		addFarmTrackToSpawner(tileSize, tilesToViewOffsetX);
		addSignPost(tileSize, "farm", Vector2(tileSize / 2.0f, tileSize * 10.0f - tileSize / 2.0f));
		regionsCount++;
	}
	if (AnimalsManager::instance.isRegionPopulated("winter")) {
		addWinterTrackToSpawner(tileSize, tilesToViewOffsetX);
		addSignPost(tileSize, "winter", Vector2(tileSize * 8.0f + tileSize / 2.0f - tilesToViewOffsetX, tileSize / 2.0f));
		regionsCount++;
	}
	if (AnimalsManager::instance.isRegionPopulated("underwater")) {
		addUnderwaterTrackToSpawner(tileSize, tilesToViewOffsetX);
		addSignPost(tileSize, "underwater", Vector2(_view->getWidth() - tileSize / 2.0f, tileSize / 2.0f));
		regionsCount++;
	}
	if (AnimalsManager::instance.isRegionPopulated("steppe")) {
		addSteppeTrackToSpawner(tileSize, tilesToViewOffsetX);
		addSignPost(tileSize, "steppe", Vector2(tileSize / 2.0f, tileSize / 2.0f + 3.0f * tileSize));
		regionsCount++;
	}
	if (AnimalsManager::instance.isRegionPopulated("asia")) {
		addAsiaTrackToSpawner(tileSize, tilesToViewOffsetX);
		addSignPost(tileSize, "asia", Vector2(tileSize * 15.0f + tileSize / 2.0f - tilesToViewOffsetX, tileSize / 2.0f));
		regionsCount++;
	}
	if (AnimalsManager::instance.isRegionPopulated("australia")) {
		addAustraliaTrackToSpawner(tileSize, tilesToViewOffsetX);
		addSignPost(tileSize, "australia", Vector2(_view->getWidth() - tileSize / 2.0f, tileSize / 2.0f + 3.0f * tileSize));
		regionsCount++;
	}
	tileField->setData("zoo_first_ground", FarmManager::instance.getZooGateLevel(regionsCount));

	addPossibleSpritesToSpawner();

	spSpectatorsArea area = new SpectatorsArea();
	area->setPriority(-45);
	area->setSize(tileSize * 5.0f, tileSize * 4.0f);
	area->setPosition(tileSize * 18.0f - tilesToViewOffsetX, tileSize * 10.0f);
	area->init(FarmManager::instance.getGateShopHumanCount());
	_view->addChild(area);

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

void ZooGateFrame::addMailBox(float tileSize, const Vector2& position) {
	_mailBoxButton = new Button();
	_mailBoxButton->setName("mailbox");
	_mailBoxButton->setResAnim(tilesResources.getResAnim("mailbox"));
	_mailBoxButton->setAnchor(Vector2(0.5, 0.5));
	_mailBoxButton->setTouchChildrenEnabled(false);
	setSpriteScaleBySize(_mailBoxButton, Vector2(tileSize, tileSize));
	_mailBoxButton->attachTo(_view);
	_mailBoxButton->setPosition(position);
	_mailBoxButton->addEventListener(TouchEvent::CLICK, CLOSURE(this, &ZooGateFrame::onButtonClicked));
	if (MessageCenterManager::instance.getMessagesCount() > 0) {
		_mailBoxButton->addTween(TweenAnim(tilesResources.getResAnim("mailbox_anim")), 500, -1);
	}
	if (MessageCenterManager::instance.shouldAnimate()) {
		spProcessMaster master = new ProcessMaster();
		master->addProcess(new AnimateFromToProcess("mail", _view, Vector2(-100.0f, -100.0f), _mailBoxButton->getPosition()));
		master->start(_view);
	}
	MessageCenterManager::instance.addEventListener(MessageCenterManager::MessageEvent::MESSAGE_COUNT_CHANGE, CLOSURE(this, &ZooGateFrame::onMessageCountChanged));
}

void ZooGateFrame::addPossibleSpritesToSpawner() {
	for (int i = 1; i <= HUMAN_CHARS_COUNT; i++) {
		_spawner->addResAnim("human_" + CMath::intToString(i));
	}
}

float ZooGateFrame::getXOnTilesToView(float x) {
	return 0.0f;
}

void ZooGateFrame::onButtonClicked(Event *ev) {
	generateAction(ev->target->getName());
}

void ZooGateFrame::onMessageCountChanged(Event *event) {
	MessageCenterManager::MessageEvent *messageEvent = safeCast<MessageCenterManager::MessageEvent*>(event);
	if (messageEvent->_count > 0) {
		_mailBoxButton->addTween(TweenAnim(tilesResources.getResAnim("mailbox_anim")), 500, -1);
	}
	else {
		_mailBoxButton->removeTweens();
		_mailBoxButton->setResAnim(tilesResources.getResAnim("mailbox"));
	}
}