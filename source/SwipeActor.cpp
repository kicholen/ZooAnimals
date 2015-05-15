#include "SwipeActor.h"

namespace oxygine
{
	SwipeActor::SwipeActor(float minSwipeSpeed, float maxFlingDelay) {
		_minSwipeSpeed = minSwipeSpeed;
		_maxFlingDelay = (long)(1000.0f * maxFlingDelay);
		_currentIndex = -1;

		addEventListener(TouchEvent::TOUCH_DOWN, CLOSURE(this, &SwipeActor::onTouchEvent));
		addEventListener(TouchEvent::TOUCH_UP, CLOSURE(this, &SwipeActor::onTouchEvent));
	}

	SwipeActor::~SwipeActor() {

	}

	void SwipeActor::handleEvent(Event *event) {
		Actor::handleEvent(event);
	}

	void SwipeActor::fling(Vector2 velocity) {
		if (fabsf(velocity.x) > fabsf(velocity.y)) {
			if (velocity.x >= _minSwipeSpeed) {
				SwipeEvent sl(SwipeEvent::RIGHT);
				dispatchEvent(&sl);
			}
			else if (velocity.x <= -_minSwipeSpeed) {
				SwipeEvent sl(SwipeEvent::LEFT);
				dispatchEvent(&sl);
			}
		}
		else {
			if (velocity.y >= _minSwipeSpeed) {
				SwipeEvent sl(SwipeEvent::DOWN);
				dispatchEvent(&sl);
			}
			else if (velocity.y <= -_minSwipeSpeed) {
				SwipeEvent sl(SwipeEvent::UP);
				dispatchEvent(&sl);
			}
		}
	}

	// primitive version, we focus only on 1 finger
	void SwipeActor::onTouchEvent(Event *event) {
		TouchEvent *te = safeCast<TouchEvent*>(event);
		timeMS tm = getTimeMS();
		const PointerState *state = te->getPointer();

		if (_currentIndex == -1) {
			_currentIndex = te->index;
		}
		else if (_currentIndex != te->index) {
			return;
		}

		switch (te->type)
		{
		case TouchEvent::TOUCH_DOWN:
			{
				_downPosition = te->localPosition;
				_downTime = tm;
			}
			break;

		case TouchEvent::TOUCH_UP:
			{
				_upPosition = te->localPosition;
				_upTime = tm;
				if (_upTime - _downTime < _maxFlingDelay) {
					fling(calculateVelocity());
				}

				_currentIndex = -1;
			}
			break;
		}
	}

	Vector2 SwipeActor::calculateVelocity() {
		float time = _upTime - _downTime;
		return Vector2((_upPosition.x - _downPosition.x) / time, (_upPosition.y - _downPosition.y) / time);
	}
}