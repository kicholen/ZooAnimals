#include "CleanAnimalsProcess.h"
#include "AnimalsManager.h"

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
	_count = 0;
	_isWater = farm->getModel()->isWaterAnimal();
	AnimalsManager::instance.cleaning(true);
}

CleanAnimalsProcess::~CleanAnimalsProcess() {
	_positions.clear();
	_cloud = 0;
	if (_source) {
		_source->setVisible(true);
	}
}

void CleanAnimalsProcess::process() {
	if (_part == 0) {
		_positions.push_back(_cloud->getPosition());
		_part++;
	}
	else if (_part == 1) {
		_count++;
		_positions.push_back(_cloud->getPosition());
		if (isFarmCleaned()) {
			_part++;
		}
		else {
			_part--;
		}
		if (_count == 10) { // for vector overstacking or block
			spriteTouchUp(0);
		}
	}
	else {
		// only here dispatch event CLEANING_COMPLETED!
		spriteTouchUp(0);
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
	_cloud->setResAnim(tilesResources.getResAnim("cloud"));
	setSpriteScaleBySize(_cloud, _source->getDerivedSize());
	_farm->addChild(_cloud);
}

void CleanAnimalsProcess::spriteTouchUp(Event *event) {
	AnimalsManager::instance.cleaning(false);
	_cloud->setTouchChildrenEnabled(false);
	_cloud->setTouchEnabled(false);
	_cloud->addTween(Actor::TweenPosition(_source->getPosition()), 500, 1)->addDoneCallback(CLOSURE(this, &CleanAnimalsProcess::onCloudBack));
}

void CleanAnimalsProcess::onCloudBack(Event *event) {
	_cloud->drag.destroy();
	_cloud->detach();
	_completed = true;
}

bool CleanAnimalsProcess::isFarmCleaned() {
	Vector2 previousPosition = Vector2(_source->getX(), _source->getY());
	float totalLength = 0.0f;
	float minLength = (_farm->getDerivedHeight() + _farm->getDerivedWidth()) * 1.2;

	for (uint i = 0; i < _positions.size(); i++) {
		totalLength += _positions[i].distance(previousPosition);
		previousPosition.x = _positions[i].x;
		previousPosition.y = _positions[i].y;
	}

	return totalLength > minLength;
}