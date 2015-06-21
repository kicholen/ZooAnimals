#include "LoadingFrame.h"
#include "Content.h"
#include "Settings.h"
#include "FlurryAnalytics.h"
#include "GooglePlayInAppPurchaseManager.h"
#include "FacebookManager.h"
#include "AnimalsManager.h"
#include "MoneyManager.h"
#include "DatabaseManager.h"
#include "ExpManager.h"
#include "ShopManager.h"
#include "LanguageManager.h"
#include "SharedResources.h"
#include "LandingPageFrame.h"
#include "Multithreading.h"

LoadingFrame::LoadingFrame() {
	init("LoadingFrame.xml", false);
	selectTransitions();
	_version = "1";
	_resourcesCounter = 4;
}

void LoadingFrame::selectTransitions() {
	spTransition TransitionSimple;
	setTransitionIn(TransitionSimple);
	setTransitionOut(TransitionSimple);
}

void LoadingFrame::_postHiding(Event *) {
	_view->removeChildren();
	_resources.unload();
}

void LoadingFrame::_preShowing(Event *) {
	selectTransitions();
	setData();
	_resources.load();
}

Action LoadingFrame::loop(){
	while (1) {
		Action action = waitAction();
		if (action.id == "game_loaded") {
			spLandingPageFrame landingPage = new LandingPageFrame();
			transitionSwitchFrame(landingPage);
			break;
		}
	}

	return _lastAction;
}

void LoadingFrame::setData() {
	spColorRectSprite background = initActor(new ColorRectSprite,
		arg_attachTo = _view,
		arg_color = Color::LightBlue,
		arg_priority = -100,
		arg_size = _view->getSize());

	spSprite backgroundLogo = new Sprite();
	backgroundLogo->setAnchor(0.5f, 0.5f);
	backgroundLogo->setPosition(_view->getWidth() / 2.0f, _view->getHeight() / 2.0f);
	backgroundLogo->setPriority(-10);
	backgroundLogo->setResAnim(_resources.getResAnim("jellyappslogo"));
	_view->addChild(backgroundLogo);
	backgroundLogo->setColor(Color(0xFFB22222));// Color(11, 11, 11, 111));

	_progressBar = initActor(new ProgressBar,
		arg_attachTo = _view,
		arg_color = CMath::HexRGBToColor32(0xF9008D),
		arg_anchor = Vector2(0.5f, 0.5f),
		arg_position = Vector2(_view->getWidth() / 2.0f, _view->getHeight() / 2.0f),
		arg_resAnim = _resources.getResAnim("jellyappslogo"));

	_progressBar->setDirection(_progressBar->dir_0);
	_progressBar->setScale(_progressBar->getScale());
	_progressBar->setProgress(0.0f);

	initAnalytics();
}

void LoadingFrame::initAnalytics() {
	FlurryAnalytics::instance.init();
	FlurryAnalytics::instance.onSessionStarted();

	_progressBar->addTween(ProgressBar::TweenProgress(0.1f), 100)->setDoneCallback(CLOSURE(this, &LoadingFrame::initSettings));
}

void LoadingFrame::initSettings(Event *ev) {
	// whole game content
	Content::instance.init(_version);

	// basic settings
	Settings::instance.init(_version);
	Settings::instance.getValue("sounds").set_value(100);
	Settings::instance.getValue("music").set_value(100);

	// todo is it needed
	Settings::instance.save();
	_progressBar->addTween(ProgressBar::TweenProgress(0.3f), 100)->setDoneCallback(CLOSURE(this, &LoadingFrame::initManagers));
}

void LoadingFrame::initManagers(Event *ev) {
	Point *ad = new Point((int)getRoot()->getWidth(), (int)getRoot()->getHeight());
	SpriteSpawner::Initialize(*ad);

	GooglePlayInAppPurchaseManager::instance.init();
	FacebookManager::instance.init();
	AnimalsManager::instance.init(_version);
	MoneyManager::instance.init(_version);
	DatabaseManager::instance.init();
	ExpManager::instance.init(_version);
	ShopManager::instance.init(_version);
	LanguageManager::instance.init(0);

	_progressBar->addTween(ProgressBar::TweenProgress(0.5f), 100)->setDoneCallback(CLOSURE(this, &LoadingFrame::initSoundPlayer));
}

void LoadingFrame::initSoundPlayer(Event *ev) {
//	sPlayer.setResources(&gameResources);
	loadGameResource();
}

void LoadingFrame::loadGameResource() {
	gameResources.loadXML("res.xml", 0, false);

	spThreadLoading threadLoader = new ThreadLoading();
	threadLoader->addEventListener(ThreadLoading::COMPLETE, CLOSURE(this, &LoadingFrame::loadedResource));
	threadLoader->add(&gameResources);
	threadLoader->start(getStage());
}

void LoadingFrame::devPurposes() {
	
}

void LoadingFrame::loadedResource(Event *ev) {
	_resourcesCounter--;
	loadNext();
}

void LoadingFrame::loadNext() {
	if (_resourcesCounter == 3) {
		loadTileResource();
		_progressBar->removeTweens();
		_progressBar->addTween(ProgressBar::TweenProgress(0.7f), 100);
	}
	else if (_resourcesCounter == 2) {
		loadEditResource();
		_progressBar->removeTweens();
		_progressBar->addTween(ProgressBar::TweenProgress(0.8f), 100);
		_progressBar->setProgress(0.6f);
	}
	else if (_resourcesCounter == 1) {
		loadAnimalRecource();
		_progressBar->removeTweens();
		_progressBar->addTween(ProgressBar::TweenProgress(0.9f), 100);
	}
	else {
		generateAction("game_loaded");
		_progressBar->setProgress(1.0f);
	}
}

void LoadingFrame::loadAnimalRecource() {
	animalsResources.loadXML("xmls/AnimalsResources.xml", 0, false);

	spThreadLoading threadLoader = new ThreadLoading();
	threadLoader->addEventListener(ThreadLoading::COMPLETE, CLOSURE(this, &LoadingFrame::loadedResource));
	threadLoader->add(&animalsResources);
	threadLoader->start(getStage());
}

void LoadingFrame::loadEditResource() {
	editResources.loadXML("edit_res.xml", 0, false);

	spThreadLoading threadLoader = new ThreadLoading();
	threadLoader->addEventListener(ThreadLoading::COMPLETE, CLOSURE(this, &LoadingFrame::loadedResource));
	threadLoader->add(&editResources);
	threadLoader->start(getStage());
}

void LoadingFrame::loadTileResource() {
	tilesResources.loadXML("tiles_res.xml", 0, false);

	spThreadLoading threadLoader = new ThreadLoading();
	threadLoader->addEventListener(ThreadLoading::COMPLETE, CLOSURE(this, &LoadingFrame::loadedResource));
	threadLoader->add(&tilesResources);
	threadLoader->start(getStage());
}