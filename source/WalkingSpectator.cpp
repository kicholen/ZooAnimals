#include "WalkingSpectator.h"


WalkingSpectator::WalkingSpectator(const VectorArray<Vector2>& trackPoints, uint number) {
	setTouchEnabled(false);
	setTouchChildrenEnabled(false);
	_number = number;
	revive(trackPoints);
	setAnchor(Vector2(0.5f, 0.5f));
}

WalkingSpectator::~WalkingSpectator() {

}

void WalkingSpectator::die() {
	if (!_dead) {
		detach();
		_dead = true;

		WalkingSpectatorEvent spectatorEvent(_number);
		dispatchEvent(&spectatorEvent);
	}
}

void WalkingSpectator::revive(const VectorArray<Vector2>& trackPoints) {
	_trackPoints = trackPoints;
	_dead = false;

	setScale(1.0f);
	setPosition(_trackPoints._vector.back());
	_trackPoints._vector.pop_back();
	tryToAnimateToNextPosition();
}

void WalkingSpectator::tryToAnimateToNextPosition() {
	removeTweens();
	setRotation(0.0f);
	if (_trackPoints.length() > 0) {
		faceAccordingToMovement(_trackPoints._vector.back().x);
		addTween(createTween(TweenPosition(_trackPoints._vector.back()), 2000))->addDoneCallback(CLOSURE(this, &WalkingSpectator::onTweenEnded));
		if (_trackPoints._vector.back().y != getY()) {
			addTween(createTween(TweenRotation(CMath::DegToRad(5)), 300, -1, true, 0));
		}
		_trackPoints._vector.pop_back();
	}
	else {
		die();
	}
}

void WalkingSpectator::onTweenEnded(Event *ev) {
	tryToAnimateToNextPosition();
}

void WalkingSpectator::faceAccordingToMovement(float destX) {
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