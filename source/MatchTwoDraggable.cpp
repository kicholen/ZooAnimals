#include "MatchTwoDraggable.h"


MatchTwoDraggable::MatchTwoDraggable() {

}

void MatchTwoDraggable::setBasePosition(Vector2 position) {
	_basePosition = position;
}

Vector2 MatchTwoDraggable::getBasePosition() {
	return _basePosition;
}