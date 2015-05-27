#include "JumpOverFrame.h"
#include "Box2dFactory.h"
#include "QueueTextAnimation.h"

JumpOverFrame::JumpOverFrame() {
	init("LandingPageFrame.xml", false);
	selectTransitions();

	spColorRectSprite bg = new ColorRectSprite;
	bg->setName("_background");
	bg->setColor(Color(88, 144, 217));
	bg->setSize(_content->getSize());
	bg->attachTo(_content);
	bg->setPriority(10);
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
			pauseWorld(0);
		}
		else if (action.id == "debug") {
			showHideDebug();
		}
		else if (action.id == "restart") {
			safeSpCast<Box2dContainer>(_view->getChild("box2d"))->forceRestart();
		}
	}

	return _lastAction;
}

void JumpOverFrame::setData() {
	_world = new Box2dContainer(_view->getSize());
	_world->attachTo(_view);
	_world->setPosition(0.0f, 0.0f);
	_world->setName("box2d");
	
	spBox2dFactory factory = new Box2dFactory(_world->_world, _world, P2M_RATIO);

	_world->addPlayer(safeCast<Player2d*>(factory->createEntity(0, Vector2(getRoot()->getWidth() * 0.2f, getRoot()->getHeight() * 0.2f), 2, false)));
	
	_world->addEntity(factory->createEntity(4, Vector2(0.0f, _view->getHeight() * 0.1f), 0, false, Vector2(getRoot()->getWidth(), getRoot()->getHeight() * 0.03f)));
	_world->addEntity(factory->createEntity(4, Vector2(_view->getWidth(), _view->getHeight() * 0.1f), 0, false, Vector2(getRoot()->getWidth(), getRoot()->getHeight() * 0.03f)));
	
	_world->addEntity(factory->createEntity(5, Vector2(_view->getWidth(), _view->getHeight() * 0.2f), 0, false));

	_world->pauseWorldAfter(2);

	spActor touchActor = new Actor();
	touchActor->setSize(_view->getSize());
	touchActor->addEventListener(TouchEvent::TOUCH_DOWN, CLOSURE(_world.get(), &Box2dContainer::onTouchDown));
	touchActor->addEventListener(TouchEvent::TOUCH_UP, CLOSURE(_world.get(), &Box2dContainer::onTouchUp));
	touchActor->attachTo(_view);

	spQueueTextAnimation animation = new QueueTextAnimation();
	animation->setAnchor(0.5f, 0.5f);
	animation->setSize(_view->getSize() / 2.0f);
	animation->setPosition(_view->getSize() / 2.0f);
	animation->attachTo(_view);
	animation->addTextToQueue("start");
	animation->addTextToQueue("1");
	animation->addTextToQueue("2");
	animation->addTextToQueue("3");
	animation->start(CLOSURE(this, &JumpOverFrame::pauseWorld));

	addButton("restart", "restart", Vector2(_view->getWidth() * 0.1f, _view->getHeight() * 0.9f));
	addButton("back", "BACK", Vector2(_view->getWidth() * 0.3f, _view->getHeight() * 0.9f));
	addButton("pause", "PAUSE", Vector2(_view->getWidth() * 0.6f, _view->getHeight() * 0.9f));
	addButton("debug", "DEBUG", Vector2(_view->getWidth() * 0.9f, _view->getHeight() * 0.9f));
}


void JumpOverFrame::showHideDebug() {
	safeSpCast<Box2dContainer>(_view->getChild("box2d"))->showHideDebug();
}

void JumpOverFrame::pauseWorld(Event *ev) {
	safeSpCast<Box2dContainer>(_view->getChild("box2d"))->pauseWorld();
}
