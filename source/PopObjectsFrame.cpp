#include "PopObjectsFrame.h"


PopObjectsFrame::PopObjectsFrame() {
	init("LandingPageFrame.xml", true);
	selectTransitions();
}

PopObjectsFrame::~PopObjectsFrame() {

}

void PopObjectsFrame::selectTransitions() {
	spTransition transition = new TransitionFade;
	setTransitionIn(transition);
	setTransitionOut(transition);
}

void PopObjectsFrame::_postHiding(Event *) {
	_view->removeChildren();
	_resources.unload();
}

void PopObjectsFrame::_preShowing(Event *) {
	selectTransitions();
	_resources.load();
	setData();
}

Action PopObjectsFrame::loop() {
	while(1) {
		Action action = waitAction();
		if (action.id == "back" || action.id == "_btn_back_") {
			break;
		}
		else if(action.id == "close") {
			break;
		}
	}

	return _lastAction;
}

void PopObjectsFrame::onFinished(Event *event) {
	//_field->restart();
}

void PopObjectsFrame::setData() {
	spColorRectSprite background = new ColorRectSprite();
	background->setColor(Color(88, 144, 217));
	background->setSize(_view->getSize());
	background->setAnchor(0.0f, 0.0f);
	background->setPosition(0.0f, 0.0f);
	background->attachTo(_view);

	spParticleEmitter emitter = new ParticleEmitter(Vector2(_view->getWidth() / 2, _view->getWidth() / 2), Vector2(_view->getHeight() / 2, _view->getHeight() / 2), Vector2(-0.5, 0.5), Vector2(-0.5, 0.5), Vector2(1000, 1000), Vector2(2, 2), "sparkle"); 
	emitter->setDestroyParticleOnTouch(true);
	_view->addChild(emitter);
	//_popField = new PopObjectsField(Vector2(_view->getWidth() * 0.8f, _view->getHeight()), 3);
	//_popField->setPosition(getRoot()->getSize().x / 2 - _field->getDerivedWidth() / 2, getRoot()->getSize().y / 2 - _field->getDerivedHeight() / 2);
	//_popField->addEventListener(FindShadowField::FindShadowFieldEvent::SHADOW_FOUND, CLOSURE(this, &PopObjectsFrame::onFinished));
	//_view->addChild(_popField);

	addButton("back", "BACK", Vector2(getRoot()->getWidth() * 0.9f, getRoot()->getHeight() * 0.9f));
}