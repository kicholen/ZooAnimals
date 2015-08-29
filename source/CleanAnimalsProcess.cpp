#include "CleanAnimalsProcess.h"
#include "AnimalsManager.h"
#include "ProcessMaster.h"
#include "AnimateGoldAction.h"

CleanAnimalsProcess::CleanAnimalsProcess(spAnimalFarmField farm, spButton source, Event *event) {
	_farm = farm;
	_source = source;
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
	AnimalsManager::instance.cleaning(false);
	_positions.clear();
	_cloud = 0;
	getStage()->removeEventListener(TouchEvent::TOUCH_UP, CLOSURE(this, &CleanAnimalsProcess::spriteTouchUp));
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
		if (_count == 10) {
			spriteTouchUp(0);
		}
	}
	else {
		_canProcess = false;
		AnimalsManager::instance.cleanAnimalByModel(_farm->getModel());
		_farm->addChild(new AnimateGoldAction(_farm, 100, _cloud->getPosition(), Vector2(getStage()->getDerivedWidth(), 0.0f)));
		spriteTouchUp(0);
	}
}

bool CleanAnimalsProcess::completed() {
	return _completed;
}

void CleanAnimalsProcess::createCloudSprite() {
	_cloud = new DraggableSprite();
	_cloud->setPosition(_source->getParent()->getX(), _source->getParent()->getY() - _source->getDerivedHeight() / 4.0f);
	_cloud->setAnchor(Vector2(0.0f, 1.0f));
	_cloud->setDragBounds(Vector2(0.0f, _source->getDerivedHeight()), Vector2(_farm->getDerivedWidth() - _source->getDerivedWidth(), _farm->getDerivedHeight() - _source->getDerivedHeight()));
	_cloud->addEventListener(TouchEvent::TOUCH_UP, CLOSURE(this, &CleanAnimalsProcess::spriteTouchUp));
	getStage()->addEventListener(TouchEvent::TOUCH_UP, CLOSURE(this, &CleanAnimalsProcess::spriteTouchUp));
	_cloud->setVisible(true);
	_cloud->setPriority(30000);
	_cloud->setTouchEnabled(true);
	_cloud->setResAnim(tilesResources.getResAnim("cloud"));
	setSpriteScaleBySize(_cloud, _source->getDerivedSize());
	_farm->addChild(_cloud);
}

void CleanAnimalsProcess::spriteTouchUp(Event *event) {
	_canProcess = false;
	if (_cloud) {
		_cloud->setTouchChildrenEnabled(false);
		_cloud->setTouchEnabled(false);
		_cloud->addTween(Actor::TweenPosition(_source->getParent()->getX(), _source->getParent()->getY() - _source->getDerivedHeight() / 4.0f), 500, 1)->addDoneCallback(CLOSURE(this, &CleanAnimalsProcess::onCloudBack));
		getStage()->removeEventListener(TouchEvent::TOUCH_UP, CLOSURE(this, &CleanAnimalsProcess::spriteTouchUp));
	}
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