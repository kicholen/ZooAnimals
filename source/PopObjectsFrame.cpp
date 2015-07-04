#include "PopObjectsFrame.h"


PopObjectsFrame::PopObjectsFrame(const std::string& difficulty) {
	init("LandingPageFrame.xml", true);
	_difficulty = difficulty;
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
	addSwipeTrail();
	addPopField();

	addButton("back", "BACK", Vector2(getRoot()->getWidth() * 0.9f, getRoot()->getHeight() * 0.9f));
}

void PopObjectsFrame::addPopField() {
	_popField = new PopObjectsField();
	_popField->setPosition(getRoot()->getSize().x / 2 - _popField->getDerivedWidth() / 2, getRoot()->getSize().y / 2 - _popField->getDerivedHeight() / 2);
	//_popField->addEventListener(FindShadowField::FindShadowFieldEvent::SHADOW_FOUND, CLOSURE(this, &PopObjectsFrame::onFinished));
	_view->addChild(_popField);
}

void PopObjectsFrame::addSwipeTrail() {
	_swipeTrailElement = new SwipeTrailElement(20);
	_swipeTrailElement->setSize(_view->getSize());
	//_swipeTrailElement->setPriority(100);
	_swipeTrailElement->attachTo(_view);
}