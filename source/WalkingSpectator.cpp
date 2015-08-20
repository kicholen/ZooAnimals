#include "WalkingSpectator.h"

WalkingSpectator::WalkingSpectator(const VectorArray<Vector2>& trackPoints, uint number) {
	_velocity = Vector2(CMath::Rand(0.2f, 0.5f), CMath::Rand(0.2f, 0.5f));
	setTouchEnabled(false);
	setTouchChildrenEnabled(false);
	_state = wsWaiting;
	_isProperX = false;
	_isProperY = false;
	setAnchor(Vector2(0.5f, 0.5f));
	_number = number;
	revive(trackPoints);
}

WalkingSpectator::WalkingSpectator() {
	_velocity = Vector2(CMath::Rand(0.2f, 0.5f), CMath::Rand(0.2f, 0.5f));
	setTouchEnabled(false);
	setTouchChildrenEnabled(false);
	_state = wsDead;
	_isProperX = false;
	_isProperY = false;
	_number = 0;
	setAnchor(Vector2(0.5f, 0.5f));
}

WalkingSpectator::~WalkingSpectator() {

}

void WalkingSpectator::die() {
	if (_state != wsDead) {
		detach();
		_state = wsDead;

		WalkingSpectatorEvent spectatorEvent(_number);
		dispatchEvent(&spectatorEvent);
	}
}

void WalkingSpectator::revive(const VectorArray<Vector2>& trackPoints) {
	_trackPoints = trackPoints;

	setPosition(-1000.0f, 10000.0f);
	setScale(1.0f);
	setPosition(_trackPoints._vector.back());
	_state = wsWaiting;

	setVelocityByNextPoint();
	checkPositionsByNextPoint();
}

void WalkingSpectator::animateMoveToPosition(const Vector2& position) {
	setFaceAccordingToMovement(position.x);
	addTween(TweenPosition(position), 1000)->addDoneCallback(CLOSURE(this, &WalkingSpectator::onAnimationEnded));
	addTween(TweenRotationDegrees(10), 200, 5, true);
}

void WalkingSpectator::makeSpriteAsChild() {
	spSprite copy = new Sprite();
	copy->setResAnim(getResAnim());
	copy->attachTo(this);
	copy->setAnchor(0.5f, 0.5f);
	setResAnim(0);
}

void WalkingSpectator::doUpdate(const UpdateState &us) {
	if (_state == wsDead) {
		return;
	}
	if (_state == wsWaiting) {
		_state = wsMoving;
		return;
	}

	Vector2 destPosition = _trackPoints._vector.back();
	doVelocity((float)us.dt, destPosition);
	checkPositionsByNextPoint();

	if (_isProperX && _isProperY) {
		_isProperX = false;
		_isProperY = false;
		_trackPoints._vector.pop_back();
		
		if (_trackPoints._vector.empty()) {
			die();
		}
		else {
			setVelocityByNextPoint();
			checkPositionsByNextPoint();
			setFaceAccordingToMovement(_trackPoints._vector.back().x);
		}
	}
}

void WalkingSpectator::doVelocity(float dt, const Vector2& destPosition) {
	if (!_isProperX) {
		float x = getX() + _velocity.x * dt;

		if (_velocity.x > 0.0f) {
			if (x > destPosition.x) {
				x = destPosition.x;
			}
		}
		else {
			if (x < destPosition.x) {
				x = destPosition.x;
			}
		}
		setX(x);
	}

	if (!_isProperY) {
		float y = getY() + _velocity.y * dt;

		if (_velocity.y < 0.0f) {
			if (y < destPosition.y) {
				y = destPosition.y;
			}
		}
		else {
			if (y > destPosition.y) {
				y = destPosition.y;
			}
		}
		setY(y);
	}
}

void WalkingSpectator::setFaceAccordingToMovement(float destX) {
	float scaleX = getScaleX();
	if (destX > getX()) {
		if (scaleX < 0.0f) {
			setScaleX(-scaleX);
		}
	}
	else if (destX < getX()) {
		if (scaleX > 0.0f) {
			setScaleX(-scaleX);
		}
	}
}

void WalkingSpectator::setVelocityByNextPoint() {
	if (!_trackPoints._vector.empty()) {
		if (_trackPoints._vector.back().x > getX()) {
			_velocity.x = _velocity.x < 0.0f ? -_velocity.y : _velocity.y;
		}
		else {
			_velocity.x = _velocity.x > 0.0f ? -_velocity.x : _velocity.x;
		}

		if (_trackPoints._vector.back().y > getY()) {
			_velocity.y = _velocity.y < 0.0f ? -_velocity.y : _velocity.y;
		}
		else {
			_velocity.y = _velocity.y > 0.0f ? -_velocity.y : _velocity.y;
		}
	}
}

void WalkingSpectator::checkPositionsByNextPoint() {
	if (!_isProperX && getX() == _trackPoints._vector.back().x) {
		_isProperX = true;
	}

	if (!_isProperY && getY() == _trackPoints._vector.back().y) {
		_isProperY = true;
	}
}

void WalkingSpectator::onAnimationEnded(Event *event) {
	_number = CMath::random(2, 6);
}