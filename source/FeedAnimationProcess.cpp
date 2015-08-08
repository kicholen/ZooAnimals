#include "FeedAnimationProcess.h"

FeedAnimationProcess::FeedAnimationProcess(spAnimalFarmField farm, spSlidingActor container) {
	_farm = farm;
	_container = container;
	createFeederMan(50);
	_canProcess = true;
	_animalNumber = 0;
}

FeedAnimationProcess::~FeedAnimationProcess() {
	
}

void FeedAnimationProcess::process() {
	if (_part == 0) {
		_farm->playAnimalsAnimation(1);
		_feederMan->setPosition(Vector2(-_container->getContent()->getX() - _feederMan->getDerivedWidth(), getStage()->getHeight() - (getStage()->getHeight() - _farm->getDerivedHeight()) / 2.0f));
		_feederMan->addTween(Actor::TweenPosition(Vector2(_farm->getX(), _farm->getY() + _farm->getDerivedHeight() / 2.0f)), 1000)->addDoneCallback(CLOSURE(this, &FeedAnimationProcess::moveToNextPart));
		_canProcess = false;
	}
	else if (_part == 1) {
		// assume that animals are in good position ? speed them up anyway, it's taking too long
		// open gate
		_feederMan->addTween(Actor::TweenY(_feederMan->getY() - 100), 500)->addDoneCallback(CLOSURE(this, &FeedAnimationProcess::moveToNextPart));
		// close gate
		_canProcess = false;
	}
	else if (_part == 2) {
		if (_farm->getAnimation()->getAnimalsCount() > _animalNumber) {
			spAnimalInFarmElement animalElement = _farm->getAnimation()->getAnimalByNumber(_animalNumber);
			_animalNumber++;
			_feederMan->addTween(Actor::TweenPosition(Vector2(_farm->getX() - _farm->getDerivedWidth() / 2.0f + animalElement->getShadowX(), _farm->getY() - _farm->getDerivedHeight() / 2.0f + animalElement->getShadowY())), 1000)->addDoneCallback(CLOSURE(this, &FeedAnimationProcess::moveToNextPart));
			_canProcess = false;
		}
		else {
			_part = 10;
		}
	}
	else if (_part == 3) {
		// add feed animal animation
		_part--;
	}
	else if (_part == 10) {
		_feederMan->addTween(Actor::TweenY(0), 1000)->addDoneCallback(CLOSURE(this, &FeedAnimationProcess::moveToNextPart));
		_canProcess = false;
	}
	else {
		_farm->playAnimalsAnimation(0);
		_feederMan->detach();
		_feederMan = 0;
		_completed = true;
	}
}

bool FeedAnimationProcess::completed() {
	return _completed;
}

void FeedAnimationProcess::createFeederMan(float spectatorsHeight) {
	_feederMan = new WalkingSpectator();

	_feederMan->setResAnim(tilesResources.getResAnim("human_1"));
	float scale = spectatorsHeight / _feederMan->getHeight();
	_feederMan->setScale(_feederMan->getScaleX() * scale, scale);
	_container->getContent()->addChild(_feederMan);
}

void FeedAnimationProcess::moveToNextPart(Event *event) {
	_part++;
	_canProcess = true;
}