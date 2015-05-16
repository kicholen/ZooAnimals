#include "CardNavigator.h"

CardNavigator::CardNavigator(int alignType, float offsetBetweenCards) {
	_alignType = alignType == 0 ? cnVertical : cnHorizontal;
	if (_alignType == cnVertical) {
		setAnchor(0.5f, 0.0f);
	}
	else {
		setAnchor(0.0f, 0.5f);
	}
	_state = cnWait;
	_offsetBetweenCards = offsetBetweenCards;
	_childrenCount = 0;
	_currentChildIndex = 1;
}

CardNavigator::~CardNavigator() {

}

void CardNavigator::addCard(spActor actor) {
	addChild(actor);
	setChildPosition(actor);
	_childrenCount++;
}

void CardNavigator::animateToNextCard() {
	if (_currentChildIndex + 1 <= _childrenCount) {
		setPositionByIndex(_currentChildIndex + 1, true);
	}
}

void CardNavigator::animateToPreviousCard() {
	if (_currentChildIndex - 1 > 0) {
		setPositionByIndex(_currentChildIndex - 1, true);
	}
}

void CardNavigator::animateToCardByIndex(int index) {

}

void CardNavigator::setChildPosition(spActor actor) {
	if (_alignType == cnVertical) {
		if (_childrenCount == 0) {
			setWidth(actor->getDerivedWidth());
			setY(getY() - actor->getDerivedHeight() / 2);
		}
		setHeight(getHeight() + actor->getDerivedHeight() + _offsetBetweenCards);

		actor->setPosition(getWidth() / 2.0f, getHeight() - actor->getDerivedHeight() / 2.0f);
	}
	else {
		if (_childrenCount == 0) {
			setHeight(actor->getDerivedHeight());
			setX(getX() - actor->getDerivedWidth() / 2);
		}
		setWidth(getWidth() + actor->getDerivedWidth() + _offsetBetweenCards);

		actor->setPosition(getWidth() - actor->getDerivedWidth() / 2.0f, getHeight() / 2.0f);
	}
}

void CardNavigator::setPositionByIndex(int index, bool shouldAnimate) {
	if (_childrenCount == 0 || _state == cnBlock) {
		return;
	}

	float position = getIndexPosition(index);
	if (_alignType == cnVertical) {
		if (shouldAnimate) {
			addTween(TweenY(position), 300)->addDoneCallback(CLOSURE(this, &CardNavigator::onTweenEnded));
			_state = cnBlock;
		}
		setY(position);
	}
	else {
		if (shouldAnimate) {
			addTween(TweenX(position), 300)->addDoneCallback(CLOSURE(this, &CardNavigator::onTweenEnded));
			_state = cnBlock;
		}
		setX(position);
	}

	_currentChildIndex = index;
}

float CardNavigator::getIndexPosition(int index) {
	int diff = index - _currentChildIndex;
	float position = 0.0f;
	if (_alignType == cnVertical) {
		position = getY() - diff * getHeight() / _childrenCount;
	}
	else {
		position = getX() - diff * getWidth() / _childrenCount;
	}

	return position;
}

void CardNavigator::onTweenEnded(Event *ev) {
	_state = cnWait;
}