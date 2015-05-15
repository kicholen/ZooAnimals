#include "CardNavigator.h"

CardNavigator::CardNavigator(int alignType, float offsetBetweenCards) {
	_alignType = alignType == 0 ? cnVertical : cnHorizontal;
	if (_alignType == cnVertical) {
		setAnchor(0.5f, 0.0f);
	}
	else {
		setAnchor(0.0f, 0.5f);
	}
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
		animateToIndex(_currentChildIndex + 1);
	}
}

void CardNavigator::animateToPreviousCard() {
	if (_currentChildIndex - 1 > 0) {
		animateToIndex(_currentChildIndex - 1);
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
		setHeight(getHeight() + actor->getDerivedHeight());

		actor->setPosition(getWidth() / 2.0f, getHeight() - actor->getDerivedHeight() / 2.0f);
	}
	else {
		if (_childrenCount == 0) {
			setHeight(actor->getDerivedHeight());
			setX(getX() - actor->getDerivedWidth() / 2);
		}
		setWidth(getWidth() + actor->getDerivedWidth());

		actor->setPosition(getWidth() - actor->getDerivedWidth() / 2.0f, getHeight() / 2.0f);
	}
}

void CardNavigator::animateToIndex(int index) {
	if (_childrenCount == 0) {
		return;
	}

	int diff = index - _currentChildIndex;// std::max(_currentChildIndex, index) - std::min(_currentChildIndex, index);
	if (_alignType == cnVertical) {
		setY(getY() - diff * getHeight() / _childrenCount);
	}
	else {
		setX(getX() - diff * getWidth() / _childrenCount);
	}

	_currentChildIndex = index;
}