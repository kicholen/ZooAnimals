#include "ChooseSpriteFrame.h"
#include "SharedResources.h"

ChooseSpriteFrame::ChooseSpriteFrame() {
	init("ChooseSpriteFrame.xml", false);

	selectTransitions();
}

void ChooseSpriteFrame::selectTransitions() {
	spTransition transition = new TransitionFade;//TransitionSimpleBlende;
	setTransitionIn(transition);
	setTransitionOut(transition);
}

Action ChooseSpriteFrame::loop(){
	while (1) {
		Action action = waitAction();
		if (action.id == "close") {
			break;
		}
	}

	return _lastAction;
}

void ChooseSpriteFrame::_postHiding(Event *) {
	_view->removeChildren();
	
	_resources.unload();
}

void ChooseSpriteFrame::_preShowing(Event *) {
	selectTransitions();
	setData();
	_resources.load();
}

void ChooseSpriteFrame::setData() {
	spColorRectSprite touchQuad = new ColorRectSprite();
	touchQuad->setSize(_view->getWidth(), _view->getHeight());
	touchQuad->setColor(Color(0, 0, 255, 0));
	touchQuad->setPosition(0.0f, 0.0f);
	touchQuad->attachTo(_view);
	touchQuad->setName("close");
	handleClick(touchQuad);

	spSlidingActor _slidingActor = new SlidingActor();
	_slidingActor->setSize(_view->getWidth()/3 * 2, _view->getHeight()/3 * 2);

	spColorRectSprite rectangleContainer = new ColorRectSprite();
	rectangleContainer->setSize(_slidingActor->getWidth(), _slidingActor->getHeight() * 2);
	rectangleContainer->setColor(Color(200, 200, 200, 255));
	float offset = 5.0f;
	float positionY = 0.0f;
	int j = editResources.getCount() - 1;
	for (int i = j; i > 0; --i) {
		spButton button = new Button();
		std::string resourceName = editResources.get(i)->getName();
		button->setResAnim(editResources.getResAnim(resourceName));
		button->setY(positionY);
		button->attachTo(rectangleContainer);
		if (button->getWidth() > rectangleContainer->getWidth()) {
			button->setScale(rectangleContainer->getWidth() / button->getWidth());
		}
		positionY += button->getDerivedHeight() + offset;
		button->setName(resourceName);
		button->addEventListener(TouchEvent::CLICK, CLOSURE(this, &ChooseSpriteFrame::onSpriteTouch));
	}
	rectangleContainer->setSize(_slidingActor->getWidth(), positionY);
	_slidingActor->setContent(rectangleContainer);
	_slidingActor->setPosition(_view->getSize()/2 - _slidingActor->getSize()/2);
	_slidingActor->attachTo(_view);
}

void ChooseSpriteFrame::onSpriteTouch(Event *event) {
	SpawnSpriteEvent spawnSpriteEvent(SpawnSpriteEvent::SPAWN_SPRITE);
	spObject name = new Object();
	name->setName(event->currentTarget->getName());
	spawnSpriteEvent.userDataObject = name;
	dispatchEvent(&spawnSpriteEvent);
}