#include "AnimalInFarmElement.h"
#include "SharedResources.h"
#include "FlashUtils.h"

using namespace FlashUtils;

AnimalInFarmElement::AnimalInFarmElement(string spriteName, Vector2 size) {
	_id = spriteName;
	_state = aifCreating;
	setSize(size);

	_calculateNewPointCounter = 0;
	_randomPointOnEdge = Vector2(CMath::random(0.0f, getWidth()), CMath::random(0.0f, getHeight()));
	_shouldCalculateNewPoint = true;
	_nextJumpDelay = 0.0f;
	setInputEnabled(false);
	setAnchor(0.0f, 0.0f);
	createAnimalSprite();
	createShadowSprite();
}

AnimalInFarmElement::~AnimalInFarmElement() {

}

void AnimalInFarmElement::createAnimalSprite() {
	_animalSprite = new Sprite();
	_animalSprite->setPriority(0);
	setAnimalSprite(_id);
	addChild(_animalSprite);
	_state = aifWaiting;
}

void AnimalInFarmElement::createShadowSprite() {
	_shadowSprite = new Sprite();
	_shadowSprite->setAnchor(0.5f, 0.5f);
	_shadowSprite->setPriority(_animalSprite->getPriority() - 1);
	_shadowSprite->setColor(Color(0, 0, 0));
	_shadowSprite->setResAnim(animalsResources.getResAnim(_id));
	_shadowSprite->setPosition(Vector2(_animalSprite->getX(), _animalSprite->getY() - _animalSprite->getDerivedHeight() / 2));
	addChild(_shadowSprite);
	setSpriteScaleBySize(_shadowSprite, Vector2(ANIMAL_IN_FARM_SIZE_X / 2, ANIMAL_IN_FARM_SIZE_Y / 2));
}

void AnimalInFarmElement::setAnimalSprite(string id) {
	_id = id;
	_animalSprite->setResAnim(animalsResources.getResAnim(id));
	setAlpha(255);
	_animalSprite->setAnchor(0.5f, 0.5f);
	_animalSprite->setPosition(CMath::random(0, getWidth()), CMath::random(0, getHeight()));
	
	setSpriteScaleBySize(_animalSprite, Vector2(ANIMAL_IN_FARM_SIZE_X, ANIMAL_IN_FARM_SIZE_Y));
}

void AnimalInFarmElement::animateJump(Vector2 position, bool isRandom) {
	if (_state == aifJumping) {
		return;
	}
	if (isRandom) {
		drawDebugLineAndJumpTo(calculateJumpPosition(getRandomPointOnRectangleEdge()));
	}
	else {
		_shouldCalculateNewPoint = true;
		drawDebugLineAndJumpTo(calculateJumpPosition(position));
	}
}

float AnimalInFarmElement::getShadowY() {
	if (_state != aifCreating) {
		return _shadowSprite->getY();
	}
}

Vector2 AnimalInFarmElement::getRandomPointOnRectangleEdge() {
	if (_shouldCalculateNewPoint) {
		_shouldCalculateNewPoint = false;
		
		int random = CMath::random(0, 5);
		switch (random) {
		case 0:
			_randomPointOnEdge = Vector2(0.0f, CMath::random(0.0f, getHeight()));
			break;
		case 1:
			_randomPointOnEdge = Vector2(getWidth(), CMath::random(0.0f, getHeight()));
			break;
		case 2:
			_randomPointOnEdge = Vector2(CMath::random(0.0f, getWidth()), 0.0f);
			break;
		case 3:
			_randomPointOnEdge = Vector2(CMath::random(0.0f, getWidth()), getHeight());
			break;
		default:
			_randomPointOnEdge = Vector2(CMath::random(0.0f, getWidth()), CMath::random(0.0f, getHeight()));
			break;
		}
	}
	return _randomPointOnEdge;
}

Vector2 AnimalInFarmElement::calculateJumpPosition(Vector2 position) {
	position = Vector2(position.x, -position.y);
	Vector2 animalPosition = Vector2(_animalSprite->getX(), -_animalSprite->getY());

	Vector2 normalizedVector = Vector2(animalPosition.x - position.x, animalPosition.y - position.y);
	normalizedVector.normalizeTo(JUMP_RANGE);
	Vector2 calculatedVector = Vector2(_animalSprite->getX() - normalizedVector.x, _animalSprite->getY() + normalizedVector.y);
	
	return checkAndChangePointIfNeeded(calculatedVector);
}

Vector2 AnimalInFarmElement::checkAndChangePointIfNeeded(Vector2 point) {
	RectF rect = RectF(getPosition(), getSize());
	Vector2 animalPosition = Vector2(_animalSprite->getX(), -_animalSprite->getY());

	if (rect.pointIn(point)) {
		if (_calculateNewPointCounter > 5) {
			_calculateNewPointCounter = 0;
			_shouldCalculateNewPoint = true;
		}
		else {
			_calculateNewPointCounter++;
		}
		return point;
	}
	else {
		_calculateNewPointCounter = 0;
		_shouldCalculateNewPoint = true;
		Vector2 centerVector = Vector2(animalPosition.x - rect.getCenter().x, animalPosition.y - rect.getCenter().y);
		centerVector.normalizeTo(JUMP_RANGE);
		float x, y;
		if (_animalSprite->getX() - centerVector.x <= getSize().x && _animalSprite->getX() - centerVector.x >= 0.0f) {
			x = _animalSprite->getX() - centerVector.x;
		}
		else {
			x = _animalSprite->getX() + centerVector.x;
		}

		if (_animalSprite->getY() - centerVector.y <= getSize().y && _animalSprite->getY() - centerVector.y >= 0.0f) {
			y = _animalSprite->getY() - centerVector.y;
		}
		else {
			y = _animalSprite->getY() + centerVector.y;
		}
		return Vector2(x, y);
	}
}

/**
*	I assume that passed point is always proper.
*	Function for drawing debug line.
*/
void AnimalInFarmElement::drawDebugLineAndJumpTo(Vector2 destinedPosition) {
	// set debug line hooked vertex
	/*if (_debugJumpLine.get() == NULL) {
		_debugJumpLine = new ColorRectSprite();
		addChild(_debugJumpLine);
	}
	_debugJumpLine->setVisible(true);
	_debugJumpLine->setColor(Color(10, 150, 22));
	_debugJumpLine->setAnchor(0.0f, 0.5f);
	_debugJumpLine->setSize(2.0f, 2.0f);
	_debugJumpLine->setPosition(_animalSprite->getPosition());
	
	// find line between Current and DestinedPosition  - debug
	float distance = FlashUtils::CMath::calculateDistance(_debugJumpLine->getPosition(), destinedPosition);
	_debugJumpLine->setWidth(distance);
	float angle = CMath::calculateAngle(_debugJumpLine->getPosition(), destinedPosition);
	_debugJumpLine->setRotation(angle);
	*/
	jumpToPosition(destinedPosition);
}

void AnimalInFarmElement::jumpToPosition(Vector2 position) {
	// shadow tween position
	_state = aifJumping;
	spTweenQueue queueTween = new TweenQueue();
	queueTween->add(TweenPosition(_animalSprite->getPosition() + (position - _animalSprite->getPosition()) / 2 - Vector2(0.0f, JUMP_HEIGHT)), JUMP_TIME / 2);	
	queueTween->add(TweenPosition(Vector2(position.x, position.y - _animalSprite->getDerivedHeight() / 2)), JUMP_TIME / 2);
	
	spTweenQueue queueTweenShadow = new TweenQueue();
	queueTweenShadow->add(TweenPosition(_shadowSprite->getPosition() + (position - _shadowSprite->getPosition()) / 2), JUMP_TIME / 2);	
	queueTweenShadow->add(TweenPosition((position)), JUMP_TIME / 2);

	_shadowSprite->addTween(queueTweenShadow);
	_animalSprite->addTween(queueTween)->setDoneCallback(CLOSURE(this, &AnimalInFarmElement::onJumpEnded));
}

void AnimalInFarmElement::animateAppear() {

}

void AnimalInFarmElement::animateDisappear() {

}

void AnimalInFarmElement::onJumpEnded(Event *event) {
	_state = aifWaiting;
	_nextJumpDelay = CMath::random(JUMP_DELAY_MIN, JUMP_DELAY_MAX);
}

void AnimalInFarmElement::doUpdate(const UpdateState &us) {
	if (_nextJumpDelay > 0.0f) {
		_nextJumpDelay -= us.dt;
	}
	else if (_state != aifJumping){
		animateJump(Vector2(), true);
	}
}