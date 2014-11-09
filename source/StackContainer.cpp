#include "StackContainer.h"
#include "SharedResources.h"

StackContainer::StackContainer(Vector2 size, StackContainerAlign alignType) {
	setData(size, alignType);
}

/*
* Made for overriding.
*/
StackContainer::StackContainer() {

}

void StackContainer::setData(Vector2 size, StackContainerAlign alignType) {
	setSize(size);
	_childrenCount = 0;
	_alignType = alignType;
	_needsUpdateChildSize = false;
}

void StackContainer::addChild(spActor actor) {
	addChild(actor.get());
}

void StackContainer::addChild(Actor *actor) {
	Actor::addChild(actor);

	_childrenCount++;
	_needsUpdateChildSize = true;

	updateChildren();
}

void StackContainer::removeChild(spActor actor) {
	Actor::removeChild(actor);

	_childrenCount--;
	_needsUpdateChildSize = true;

	updateChildren();
}

void StackContainer::removeChildren() {
	Actor::removeChildren();

	_childrenCount = 0;
}

void StackContainer::updateChildren() {
	spActor child = getFirstChild();
	int count = 1;

	while (child) {
		setChildPositionAndScale(child, count);
		count++;
		child = child->getNextSibling();
	}
}

void StackContainer::setChildPositionAndScale(spActor actor, int count) {
	setActorScaleBySize(actor, getProperChildSize());
	actor->setAnchor(0.5f, 0.5f);
	actor->setPosition(getProperChildPosition(count));
}

Vector2 StackContainer::getProperChildSize() {
	if (_needsUpdateChildSize) {
		if (_alignType == scVertical) {
			float maxWidth = getWidth();
			float maxHeight = getHeight() / (float)_childrenCount;

			_properChildSize = Vector2(maxWidth, maxHeight);
		}
		else {
			float maxWidth = getWidth() / (float)_childrenCount;
			float maxHeight = getHeight();

			_properChildSize = Vector2(maxWidth, maxHeight);
		}

		_needsUpdateChildSize = false;
	}

	return _properChildSize;
}

Vector2 StackContainer::getProperChildPosition(int count) {
	if (_alignType == scVertical) {
		float xPosition = getWidth() / 2;
		float yPosition = getHeight() / (float)_childrenCount * ((float)count - 0.5f);

		return Vector2(xPosition, yPosition);
	}
	else {
		float xPosition = getWidth() / (float)_childrenCount * ((float)count - 0.5f);
		float yPosition = getHeight() / 2;

		return Vector2(xPosition, yPosition);
	}
}