#include "FeedAnimationProcess.h"

FeedAnimationProcess::FeedAnimationProcess(spAnimalFarmField farm, spSlidingActor container) {
	_farm = farm;
	_container = container;
	createFeederMan(getRoot()->getHeight() * 0.07f); // todo remove this magical num
	_canProcess = true;
	_animalNumber = 0;
	_isWater = farm->getModel()->isWaterAnimal();
	_isBoatAttachedToFeederMan = false;
}

FeedAnimationProcess::~FeedAnimationProcess() {
	_feederMan = 0;
	_boat = 0;
}

void FeedAnimationProcess::process() {
	if (_part == 0) {
		for (int i = 0; i < _farm->getAnimation()->getAnimalsCount(); i++) {
			_farm->getAnimation()->getAnimalsArray()[i]->setJumpSpeedFactor(0.4f);
		}
		_farm->playAnimalsAnimation(1);
		
		Vector2 startPosition = std::rand() > RAND_MAX / 2 
			? Vector2(-_container->getContent()->getX() - _feederMan->getDerivedWidth(), getStage()->getHeight() - (getStage()->getHeight() - _farm->getDerivedHeight()) / 2.0f)
			: Vector2(-_container->getContent()->getX() + getStage()->getWidth() + _feederMan->getDerivedWidth(), getStage()->getHeight() - (getStage()->getHeight() - _farm->getDerivedHeight()) / 2.0f);

		_feederMan->setPosition(startPosition);
		_feederMan->addTween(Actor::TweenPosition(Vector2(_farm->getX() - _farm->getDerivedWidth() / 2.0f + _farm->getGateSprite()->getX() + _farm->getGateSprite()->getDerivedWidth() / 2.0f, _farm->getY() + _farm->getDerivedHeight() / 2.0f)), 2000)->setDoneCallback(CLOSURE(this, &FeedAnimationProcess::moveToNextPart));
		_canProcess = false;
	}
	else if (_part == 1) {
		float yPos = _feederMan->getY() - 100;
		if (_isWater) {
			_boat = createBoat(_feederMan->getDerivedWidth() * 2.0f);
			_boat->setPosition(_feederMan->getX(), yPos + _feederMan->getDerivedHeight() - _boat->getDerivedHeight() / 2.0f);
		}
		spSprite gateSprite = _farm->getGateSprite();
		gateSprite->addTween(Actor::TweenX(gateSprite->getX() - gateSprite->getDerivedWidth()), 500);
		_feederMan->addTween(Actor::TweenY(yPos), 500, 1, false, 500)->addDoneCallback(CLOSURE(this, &FeedAnimationProcess::moveToNextPart));
		gateSprite->addTween(Actor::TweenX(gateSprite->getX()), 500, 1, false, 2000);
		_canProcess = false;
	}
	else if (_part == 2) {
		if (_isWater && !_isBoatAttachedToFeederMan) {
			float height = _feederMan->getDerivedHeight();
			_feederMan->makeSpriteAsChild();
			_boat->addRef();
			_boat->detach();
			_boat->attachTo(_feederMan);
			_boat->releaseRef();
			_boat->setPosition(0.0f, height / 2.0f);
			_boat->setScale(_boat->getScaleX() / _feederMan->getScaleX());
			_boat->setPriority(-1);
			_isBoatAttachedToFeederMan = true;
		}
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
		// todo add feed animal animation, food based on model or universal animation
		_part--;
	}
	else if (_part == 10) {
		_feederMan->addTween(Actor::TweenY(-200), 1500)->addDoneCallback(CLOSURE(this, &FeedAnimationProcess::moveToNextPart));
		_canProcess = false;
	}
	else {
		for (int i = 0; i < _farm->getAnimation()->getAnimalsCount(); i++) {
			_farm->getAnimation()->getAnimalsArray()[i]->setJumpSpeedFactor(1.0f);
		}
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
	_feederMan->setPriority(_feederMan->getPriority() + 1);
	_feederMan->setResAnim(tilesResources.getResAnim("human_1"));
	float scale = spectatorsHeight / _feederMan->getHeight();
	_feederMan->setScale(_feederMan->getScaleX() * scale, scale);
	_container->getContent()->addChild(_feederMan);
}

spSprite FeedAnimationProcess::createBoat(float boatWidth) {
	spSprite boat = new Sprite();
	boat->setTouchEnabled(false);
	boat->setAnchor(Vector2(0.5f, 0.5f));

	boat->setResAnim(tilesResources.getResAnim("boat"));
	float scale = boatWidth / boat->getWidth();
	boat->setScale(boat->getScaleX() * scale, scale);
	_container->getContent()->addChild(boat);
	boat->setPriority(_feederMan->getPriority() - 1);

	return boat;
}

void FeedAnimationProcess::moveToNextPart(Event *event) {
	_part++;
	_canProcess = true;
}