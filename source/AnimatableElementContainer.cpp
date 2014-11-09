#include "AnimatableElementContainer.h"
#include "SharedResources.h"

AnimatableElementContainer::AnimatableElementContainer(Vector2 size, float offsetPercent) {
	setSize(size);
	_offsetPercent = offsetPercent;
	_needsUpdateArrayDimensions = false;
	_stackContainersArray._vector.resize(0);
	_childrenArray._vector.resize(0);
}

AnimatableElementContainer::~AnimatableElementContainer() {
	_stackContainersArray._vector.resize(0);
	_childrenArray._vector.resize(0);
}

void AnimatableElementContainer::showContainerElements(bool shouldAnimate) {
	for (int i = _childrenArray.length() - 1; i >= 0; i--) {
		if (shouldAnimate) {
			_childrenArray[i]->addTween(Actor::TweenAlpha(255), TWEEN_DURATION, 1, false, CMath::random(TWEEN_TIME_MIN, TWEEN_TIME_MAX));
		}
		else {
			_childrenArray[i]->setAlpha(255);
		}
	}
}

void AnimatableElementContainer::hideContainerElements(bool shouldAnimate) {
	for (int i = _childrenArray.length() - 1; i >= 0; i--) {
		if (shouldAnimate) {
			_childrenArray[i]->addTween(Actor::TweenAlpha(0), TWEEN_DURATION, 1, false, CMath::random(TWEEN_TIME_MIN, TWEEN_TIME_MAX));
		}
		else {
			_childrenArray[i]->setAlpha(0);
		}
	}
}

void AnimatableElementContainer::addChildren(Array<spActor> children) {
	for (int i = children.length() - 1; i >= 0; i--) {
		_childrenArray.push(children[i]);
	}

	children._vector.resize(0);
	_needsUpdateArrayDimensions = true;

	updateChildren();
}

void AnimatableElementContainer::removeChildren() {
	Actor::removeChildren();
	_stackContainersArray._vector.resize(0);
	_childrenArray._vector.resize(0);
}

void AnimatableElementContainer::updateChildren() {
	for (int i = _stackContainersArray.length() - 1; i >= 0; i--) {
		_stackContainersArray[i]->detach();
	}

	_stackContainersArray._vector.resize(0);

	int xDimension = getProperArrayDimensions().x;
	int yDimension = getProperArrayDimensions().y;

	int childIndex = 0;

	for (int i = 0; i <= yDimension - 1; i++) {
		int childCounterPerStack = 0;
		spStackContainer stackContainer = new StackContainer(Vector2(getWidth(), getHeight() / yDimension - _offsetPercent / 100.0f * getHeight()), StackContainerAlign::scHorizontal);
		stackContainer->setAnchor(0.0f, 0.0f);
		stackContainer->setPosition(Vector2(0.0f, getHeight() / yDimension * i));// + getHeight() * _offsetPercent / 100.0f / 2 * (i + 1)));
		
		_stackContainersArray.push(stackContainer);
		stackContainer->attachTo(this);

		while (childIndex < _childrenArray.length() && childCounterPerStack < xDimension) {
			stackContainer->addChild(_childrenArray[childIndex]);
			childIndex++;
			childCounterPerStack++;
		}
	}
}

Point AnimatableElementContainer::getProperArrayDimensions() {
	if (_needsUpdateArrayDimensions) {
		int countSquared = (int)floorf(scalar::sqrt(_childrenArray.length()));

		int xDimension = ceil((double)_childrenArray.length() / (double)countSquared);
		int yDimension = ceil(double(_childrenArray.length()) / double(xDimension));

		if (getDerivedWidth() > getDerivedHeight()) {
			_properArrayDimensions = Point(xDimension, yDimension);
		}
		else {
			_properArrayDimensions = Point(yDimension, xDimension);
		}

		_needsUpdateArrayDimensions = false;
	}


	return _properArrayDimensions;
}