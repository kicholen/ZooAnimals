#include "JumpOverFrame.h"
#include "Box2dContainer.h"
#include "Box2dFactory.h"

JumpOverFrame::JumpOverFrame() {
	init("LandingPageFrame.xml", true);
	selectTransitions();
}

void JumpOverFrame::selectTransitions() {
	spTransition transition = new TransitionFade;
	setTransitionIn(transition);
	setTransitionOut(transition);
}

void JumpOverFrame::_postHiding(Event *) {
	//FlurryAnalytics::instance.onLevelLeaveEvent(_whichLevel.c_str());
	_view->removeChildren();
	_resources.unload();
}

void JumpOverFrame::_preShowing(Event *) {
	//FlurryAnalytics::instance.onLevelEnterEvent(_whichLevel.c_str());
	selectTransitions();
	_resources.load();
	setData();
}

Action JumpOverFrame::loop() {
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
		else if (action.id == "pause") {
			pauseWorld();
		}
		else if (action.id == "debug") {
			showHideDebug();
		}
	}

	return _lastAction;
}

void JumpOverFrame::setData() {
	spBox2dContainer world = new Box2dContainer();
	world->attachTo(_view);
	world->setPosition(0.0f, 0.0f);

//	world->addEventListener(Box2dContainer::LevelWonEvent::LEVEL_WON, CLOSURE(this, &JumpOverFrame::onGoBack));
//	world->addEventListener(Box2dContainer::LevelRestartEvent::LEVEL_RESTART, CLOSURE(this, &JumpOverFrame::onLevelRestart));
	world->setName("box2d");
	
	spBox2dFactory factory = new Box2dFactory(world->_world, world, P2M_RATIO);
	factory->addRef();

	world->addPlayer(safeCast<Player2d*>(factory->createEntity(EntityType::player2d, getRoot()->getSize() / 2.0f, BodyType::dynamicBody, false)));

	world->addEntity(factory->createEntity(EntityType::static2d, Vector2(_view->getWidth() * 0.4f, _view->getHeight() * 0.8f), BodyType::staticBody, false));
	world->addEntity(factory->createEntity(EntityType::static2d, Vector2(_view->getWidth() * 0.8f, _view->getHeight() * 0.8f), BodyType::staticBody, false));

	addButton("back", "BACK", Vector2(_view->getWidth() * 0.3f, _view->getHeight() * 0.9f));
	addButton("pause", "PAUSE", Vector2(_view->getWidth() * 0.6f, _view->getHeight() * 0.9f));
	addButton("debug", "DEBUG", Vector2(_view->getWidth() * 0.9f, _view->getHeight() * 0.9f));
}


void JumpOverFrame::showHideDebug() {
	safeSpCast<Box2dContainer>(_view->getChild("box2d"))->showHideDebug();
	/*
	spActor actor = _view->getFirstChild();
	while (actor) {
		if (actor->getName() == "box2d") {
			safeSpCast<Box2dContainer>(actor)->showHideDebug();
		}
		actor = actor->getNextSibling();
	}*/
}

void JumpOverFrame::pauseWorld() {
	safeSpCast<Box2dContainer>(_view->getChild("box2d"))->pauseWorld();
	/*
	spActor actor = _view->getFirstChild();
	while (actor) {
		if (actor->getName() == "box2d") {
			safeSpCast<Box2dContainer>(actor)->pauseWorld();
		}
		actor = actor->getNextSibling();
	}*/
}

void JumpOverFrame::addQuad() {
//	safeSpCast<Box2dContainer>(_view->getChild("box2d"))->addQuad();
/*	while (actor) {
		if (actor->getName() == "box2d") {
			safeSpCast<Box2dContainer>(actor)->addQuad();
		}
		actor = actor->getNextSibling();
	}*/
}