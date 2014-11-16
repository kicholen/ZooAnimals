#include "SwipeTrailElement.h"
#include "SharedResources.h"

SwipeTrailElement::SwipeTrailElement(int maxInputPoints) {
	_initialDistance = 10;
	_minDistance = 20;
	_pressed = false;
	_touchPoints.setCapacity(maxInputPoints);
	_pointsSimplifier = new ResolverRadialChaikin();
	_swipePointsGenerator = new SwipePointsGenerator(30.0f, 8.5f);
	resolve();

	addEventListener(TouchEvent::TOUCH_DOWN, CLOSURE(this, &SwipeTrailElement::onTouchEvent));
	addEventListener(TouchEvent::TOUCH_UP, CLOSURE(this, &SwipeTrailElement::onTouchEvent));
}

SwipeTrailElement::~SwipeTrailElement() {
	_pointsSimplifier = 0;
	_swipePointsGenerator = 0;
	_triangles.clear();
	_simplifiedPoints.clear();
}

void SwipeTrailElement::doUpdate(const UpdateState &us) {
	_triangles.clear();
	
	if (_simplifiedPoints.length() < 2) {
		return;
	}

	_batchSize = generate(_simplifiedPoints, 1);
	int b = generate(_simplifiedPoints, -1);
}

void SwipeTrailElement::onTouchEvent(Event *event) {
	TouchEvent *te = safeCast<TouchEvent*>(event);
	switch(te->type)
	{
	case TouchEvent::TOUCH_DOWN:
		{
			startDrag(te->localPosition);
		}
		break;
	case TouchEvent::TOUCH_UP:
		{
			_pressed = false;
			_lastPoint = Vector2();
			removeEventListener(TouchEvent::MOVE, CLOSURE(this, &SwipeTrailElement::onTouchEvent));

			for (int i = 0; i < _simplifiedPoints.length(); i++) {
				oxygine::log::message(FlashUtils::CMath::stringFormat("<<<<<<< %f, %f \n", _simplifiedPoints[i].x, _simplifiedPoints[i].y).c_str());
			}
		}
		break;

	case TouchEvent::MOVE:
		{
			onMove(te->localPosition);
		}
		break;
	}
}

void SwipeTrailElement::startDrag(Vector2 localPosition) {
	_pressed = true;
	_touchPoints.clear();
	_lastPoint = localPosition;
	_touchPoints.push(_lastPoint);
	resolve();
	addEventListener(TouchEvent::MOVE, CLOSURE(this, &SwipeTrailElement::onTouchEvent));
}

void SwipeTrailElement::onMove(Vector2 localPosition) {
	// We push only points that exceeded < MIN_LENGTH
	if (!_pressed || _lastPoint.x == 0.0f && _lastPoint.y == 0.0f) {
		return;
	}
	float dx = localPosition.x - _lastPoint.x;
	float dy = localPosition.y - _lastPoint.y;
	float length = scalar::sqrt((dx * dx + dy * dy));

	if ((length < _minDistance) && (_touchPoints.length() > 1 || length < _initialDistance)) {
		return;
	}

	_touchPoints.push(localPosition);
	_lastPoint = localPosition;
		
	//simplify new line
	resolve();
}

void SwipeTrailElement::resolve() {
	_pointsSimplifier->resolve(_touchPoints, _simplifiedPoints);
}

int SwipeTrailElement::generate(DequeArray<Vector2>& input, int mult) {
	return _swipePointsGenerator->generate(input, _triangles, mult);
}