#include "RotatingContainer2d.h"
#include "initActor.h"
#include "FlashUtils.h"

RotatingContainer2d::RotatingContainer2d(Vector2 size) : _mainChildScale(0.5f), _centerSubChildrenAt(0.75f), _needsRevalidate(false) {
	setSize(size);
	_wheel = initActor(new Actor,
		arg_position = Vector2(getSize() / 2),
		arg_anchor = Vector2(0.5f, 0.5f),
		arg_size = size,
		arg_attachTo = this);
}

RotatingContainer2d::~RotatingContainer2d() {

}

void RotatingContainer2d::clear() {
	_wheel->removeChildren();
	if (_mainChild != NULL) {
		_mainChild->detach();
	}
}

void RotatingContainer2d::addMainChild(spActor child) {
	_mainChild = child;
	_mainChild->setAnchor(0.5f, 0.5f);
	_mainChild->setPosition(getWidth() / 2, getHeight() / 2);
	scaleChild(_mainChild, _mainChildScale);
	_mainChild->attachTo(this);
	_needsRevalidate = true;
}

void RotatingContainer2d::addSubChild(spActor child) {
	_wheel->addChild(child);
	_needsRevalidate = true;
}

void RotatingContainer2d::invalidateAll() {
	spActor child = _wheel->getFirstChild();
	int numChildren = 0;
	
	while (child) {
		numChildren++;
		child = child->getNextSibling();
	}

	Vector2 center = _wheel->getSize() / 2;

	float angleOffset = 360 / numChildren;

	float childDistanceFromCenter = center.x * _centerSubChildrenAt;

	child = _wheel->getFirstChild();

	float startAngle = 0.0f;
	while (child) {
		Vector2 position = getSubChildPosition(FlashUtils::CMath::DegToRad(startAngle), center, childDistanceFromCenter);
		scaleChild(child, _mainChildScale * 0.75f);
		child->setPosition(position);

		startAngle += angleOffset;
		child = child->getNextSibling();
	}

	_needsRevalidate = false;
}

Vector2 RotatingContainer2d::getSubChildPosition(float angle, Vector2 center, float distanceFromCenter) {
	return Vector2(center + Vector2(sin(angle) * distanceFromCenter, cos(angle) * distanceFromCenter));
}

void RotatingContainer2d::doUpdate(const UpdateState &us) {
	if (!_wheel) {
		return;
	}
	if (_needsRevalidate) {
		invalidateAll();
	}
}

void RotatingContainer2d::scaleChild(spActor sprite, float scaleInParent) {
	float scale = 1.0f;
	if (sprite->getWidth() > sprite->getHeight()) {
		scale = getWidth() * scaleInParent / sprite->getWidth();
	}
	else {
		scale = getHeight() * scaleInParent / sprite->getHeight();
	}
	sprite->setScale(scale);
}