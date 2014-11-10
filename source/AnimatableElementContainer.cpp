#include "AnimatableElementContainer.h"
#include "SharedResources.h"

AnimatableElementContainer::AnimatableElementContainer(Vector2 size, float offsetPercent) {
	setSize(size);
	_offsetPercent = offsetPercent;
	_needsUpdateArrayDimensions = false;
	_stackContainersArray._vector.resize(0);
	_childrenArray._vector.resize(0);
	_animationType = aecMove;
}

AnimatableElementContainer::~AnimatableElementContainer() {
	_stackContainersArray._vector.resize(0);
	_childrenArray._vector.resize(0);
}

void AnimatableElementContainer::setAnimationType(AnimatableElementContainerAnimationType animationType) {
	_animationType = animationType;
}

void AnimatableElementContainer::showContainerElements(bool shouldAnimate) {
	for (int i = _childrenArray.length() - 1; i >= 0; i--) {
		if (shouldAnimate) {
			addAnimationTween(_childrenArray[i], true);
		}
		else {
			setAlphaToActor(_childrenArray[i], true);
		}
	}
}

void AnimatableElementContainer::hideContainerElements(bool shouldAnimate) {
	for (int i = _childrenArray.length() - 1; i >= 0; i--) {
		if (shouldAnimate) {
			addAnimationTween(_childrenArray[i], false);
		}
		else {
			setAlphaToActor(_childrenArray[i], false);
		}
	}
}

void AnimatableElementContainer::addChildren(VectorArray<spActor> children) {
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
		spStackContainer stackContainer = new StackContainer(Vector2(getWidth(), getHeight() / yDimension - _offsetPercent / 100.0f * getHeight()), 1);
		stackContainer->setAnchor(0.0f, 0.0f);
		stackContainer->setPosition(Vector2(0.0f, getHeight() / yDimension * i + getHeight() * _offsetPercent / 100.0f / 2));// * (i + 1)));
		
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

		int xDimension = (int)ceil((double)_childrenArray.length() / (double)countSquared);
		int yDimension = (int)ceil(double(_childrenArray.length()) / double(xDimension));

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

void AnimatableElementContainer::addAnimationTween(spActor actor, bool show) {
	if (show) {
		switch (_animationType) {
			case aecAlpha:
				actor->addTween(Actor::TweenAlpha(255), TWEEN_DURATION, 1, false, CMath::random(TWEEN_TIME_MIN, TWEEN_TIME_MAX));
				break;
			case aecMove: {
				setAlphaToActor(actor, true);
				Vector2 actorPosition = actor->getPosition();
				actor->setPosition(Vector2(0.0f, 0.0f));
				actor->addTween(Actor::TweenPosition(actorPosition), TWEEN_DURATION, 1, false, CMath::random(TWEEN_TIME_MIN, TWEEN_TIME_MAX), Tween::ease_inOutBack);
				break;
			}
			case aecScale: {
				setAlphaToActor(actor, true);
				Vector2 actorScale = Vector2(actor->getScaleX(), actor->getScaleY());
				actor->setScale(Vector2(0.0f, 0.0f));
				actor->addTween(Actor::TweenScale(actorScale), TWEEN_DURATION, 1, false, CMath::random(TWEEN_TIME_MIN, TWEEN_TIME_MAX), Tween::ease_inOutBack);
				break;
			}
			default:
				break;
		}
	}
	else {
		actor->addTween(Actor::TweenAlpha(0), TWEEN_DURATION, 1, false, CMath::random(TWEEN_TIME_MIN, TWEEN_TIME_MAX));
	}
}

void AnimatableElementContainer::setAlphaToActor(spActor actor, bool show) {
	if (show) {
		actor->setAlpha(255);
	}
	else {
		actor->setAlpha(0);
	}
}