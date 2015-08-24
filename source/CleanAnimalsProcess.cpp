#include "CleanAnimalsProcess.h"

CleanAnimalsProcess::CleanAnimalsProcess(spAnimalFarmField farm, spButton source, Event *event) {
	_farm = farm;
	_source = source;
	_source->setVisible(false);
	createCloudSprite();
	TouchEvent *touchEvent = safeCast<TouchEvent*>(event);
	Vector2 src = touchEvent->getPointer()->getPosition().cast<Vector2>();
	Vector2 pos = convert_stage2local(source, src);
	_cloud->drag.startDrag(pos);
	_canProcess = true;
	_animalsCleaned = 0;
	_isWater = farm->getModel()->isWaterAnimal();
}

CleanAnimalsProcess::~CleanAnimalsProcess() {
	_cloud = 0;
	if (_source) {
		_source->setVisible(true);
	}
}

void CleanAnimalsProcess::process() {
	if (_part == 0) {
		// count animals, make rain droplets
	}
	else {
		_completed = true;
	}
}

bool CleanAnimalsProcess::completed() {
	return _completed;
}

void CleanAnimalsProcess::createCloudSprite() {
	_cloud = new DraggableSprite();
	_cloud->setPosition(_source->getPosition());
	_cloud->setAnchor(Vector2(0.0f, 1.0f));
	_cloud->setDragBounds(Vector2(0.0f, _source->getDerivedHeight()), Vector2(_farm->getDerivedWidth() - _source->getDerivedWidth(), _farm->getDerivedHeight() - _source->getDerivedHeight()));
	_cloud->addEventListener(TouchEvent::TOUCH_UP, CLOSURE(this, &CleanAnimalsProcess::spriteTouchUp));
	_cloud->setVisible(true);
	_cloud->setPriority(30000);
	_cloud->setTouchEnabled(true);
	_cloud->setResAnim(gameResources.getResAnim("quad"));
	setSpriteScaleBySize(_cloud, _source->getDerivedSize());
	_farm->addChild(_cloud);
}

void CleanAnimalsProcess::spriteTouchUp(Event *event) {
	_cloud->setTouchChildrenEnabled(false);
	_cloud->setTouchEnabled(false);
	_cloud->addTween(Actor::TweenPosition(_source->getPosition()), 500, 1)->addDoneCallback(CLOSURE(this, &CleanAnimalsProcess::onCloudBack));
}

void CleanAnimalsProcess::onCloudBack(Event *event) {
	_cloud->detach();
	_completed = true;
}