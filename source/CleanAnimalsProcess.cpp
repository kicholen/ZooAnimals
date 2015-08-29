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
	_lastDropletIndex = 0;
	_timeSinceLastDroplet = 0.0f;
	_isWater = farm->getModel()->isWaterAnimal();
	AnimalsManager::instance.cleaning(true);
	_updateCallback.p_this = this;
	_updateCallback.p_proxy = CLOSURE(this, &CleanAnimalsProcess::updateDroplet).p_proxy;
	_cloud->setCallbackDoUpdate(_updateCallback);
}

CleanAnimalsProcess::~CleanAnimalsProcess() {
	_droplets.clear();
	_positions.clear();
	AnimalsManager::instance.cleaning(false);
	_cloud = 0;
	getStage()->removeEventListener(TouchEvent::TOUCH_UP, CLOSURE(this, &CleanAnimalsProcess::spriteTouchUp));
}

void CleanAnimalsProcess::process() {
	spawnDroplets();
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
	_cloud->setTouchChildrenEnabled(false);
	_cloud->setResAnim(tilesResources.getResAnim("cloud"));
	setSpriteScaleBySize(_cloud, _source->getDerivedSize());
	_farm->addChild(_cloud);
}

void CleanAnimalsProcess::spriteTouchUp(Event *event) {
	_canProcess = false;
	for (int i = 0; i < _droplets.length(); i++) {
		_droplets[i]->detach();
	}
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

	for (int i = 0; i < _positions.size(); i++) {
		totalLength += _positions[i].distance(previousPosition);
		previousPosition.x = _positions[i].x;
		previousPosition.y = _positions[i].y;
	}

	return totalLength > minLength;
}

void CleanAnimalsProcess::spawnDroplets() {
	if (_droplets.length() == 20) {// todo make random inlined here?
		_droplets[_lastDropletIndex]->setPosition(_cloud->getX() + FlashUtils::CMath::Rand(0.0f, _cloud->getDerivedWidth()), _cloud->getY() + FlashUtils::CMath::Rand(_cloud->getDerivedHeight() / 4.0f, _cloud->getDerivedHeight() / 2.0f));
		_lastDropletIndex = _lastDropletIndex == 19 ? 0 : _lastDropletIndex + 1;
	}
	else {
		spSprite droplet = new Sprite();
		droplet->setPosition(_cloud->getX() + FlashUtils::CMath::Rand(0.0f, _cloud->getDerivedWidth()), _cloud->getY() + FlashUtils::CMath::Rand(_cloud->getDerivedHeight() / 4.0f, _cloud->getDerivedHeight() / 2.0f));
		droplet->setAnchor(Vector2(0.5f, 0.5f));
		droplet->setPriority(20000);
		droplet->setTouchEnabled(false);
		droplet->setResAnim(tilesResources.getResAnim("plate"));
		droplet->setColor(Color(102, 202, 200));
		setSpriteScaleBySize(droplet, _cloud->getDerivedSize() / 10.0f);
		_farm->addChild(droplet);
		_droplets.push(droplet);
	}
}

void CleanAnimalsProcess::updateDroplet(const UpdateState &us) {
	for (int i = 0; i < _droplets.length(); i++) {
		_droplets[i]->setY(_droplets[i]->getY() + us.dt * 0.1f);
	}
	
	_timeSinceLastDroplet += us.dt;
	if (1000.0f / DROPLETS_PER_SECOND > _timeSinceLastDroplet) {
		_timeSinceLastDroplet = 0.0f;
		spawnDroplets();
	}
}