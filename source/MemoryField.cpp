#include "MemoryField.h"
#include "SharedResources.h"
#include "FlashUtils.h"
#include <algorithm>

MemoryField::MemoryField(Point numberOfFields) {
	animated_count = 0;
	droped_count = 0;
	_backSwap = false;
	_numberOfFields = numberOfFields;
	EventCallback cb = CLOSURE(this, &MemoryField::cardClick);
	addEventListener(TouchEvent::TOUCH_DOWN, cb);
	_state = mfWaiting;

	setSize(MEMORY_SIZE_X * _numberOfFields.x, MEMORY_SIZE_Y * _numberOfFields.y);
	/*
	spColorRectSprite background = new ColorRectSprite();
	background->setColor(Color(0, 25, 120));
	background->setSize(getSize());
	background->setAnchor(0.0f, 0.0f);
	background->setPosition(0.0f, 0.0f);
	background->attachTo(this);
	*/
	fillField(true);
	animateField();
}

void MemoryField::fillField(bool isFirstTime) {
	string *buffer = new string[_numberOfFields.x * _numberOfFields.y]; 
	generateRandomCardsArrayByType(buffer);
	random_shuffle(buffer, buffer + _numberOfFields.x * _numberOfFields.y);
	int bufferIndex = 0;

	for (int i = 0; i < _numberOfFields.x; i++) {
		for (int j = _numberOfFields.y - 1; j >= 0; j--) {
			if (isFirstTime) {
				spMemoryCard newCard = new MemoryCard(buffer[bufferIndex]);
				newCard->setPosition(getCellPosition(i, j));
				newCard->index = Point(i,j);
				newCard->setName(FlashUtils::CMath::intToString(i) + FlashUtils::CMath::intToString(j));
				addChild(newCard);
			}
			getChildT<MemoryCard>(FlashUtils::CMath::intToString(i) + FlashUtils::CMath::intToString(j))->set(buffer[bufferIndex]);
			bufferIndex++;
		}
	}

	delete [] buffer;
	buffer = 0;
}

/**
 * Start at random point in array and fill from there fileds
 * Much optimistic, think that one group of animals can fill whole memory slots
*/

void MemoryField::generateRandomCardsArrayByType(string *buffer) {
	int maxLength = sizeof(ALL_ANIMALS)/sizeof(ALL_ANIMALS[0]);
	int random = int(ceill(FlashUtils::CMath::Rand(0, maxLength)));
	
	for (int i = 0; i < _numberOfFields.x * _numberOfFields.y; i++) {
		string copiedName(ALL_ANIMALS[random]);

		buffer[i] = copiedName;
		i += 1;
		buffer[i] = copiedName;

		if (random < maxLength) {
			random++;
		}
		else {
			random = 0;
		}
	}
}

void MemoryField::animateField() {
	_state = mfFalling;
	int count = 0;

	for (int i = 0; i < _numberOfFields.x; i++) {
		count = 0;
		for (int j = _numberOfFields.y - 1; j >= 0; j--) {
			spMemoryCard memoryCard = getChildT<MemoryCard>(FlashUtils::CMath::intToString(i) + FlashUtils::CMath::intToString(j));
			Vector2 pos = memoryCard->getPosition();
			memoryCard->setPosition(Vector2(pos.x, 2 * getPosition().y - count * (MEMORY_SIZE_Y + rand() % 10 + 20)));
			spTween tween = memoryCard->dropTo(pos);
			droped_count++;
			count++;
			tween->addEventListener(TweenEvent::DONE, CLOSURE(this, &MemoryField::dropEndCallback));
		}
	}
}

void MemoryField::dropEndCallback(Event *ev) {
	safeSpCast<MemoryCard>(ev->target)->setState(mcNormal);
	droped_count--;
	if (droped_count == 0 ) {
		_state = mfWaiting;
	}
}

void MemoryField::cardClick(Event *event) {
	TouchEvent *te = safeCast<TouchEvent*>(event);
	Point ind = getCellIndex(te->localPosition);
	spMemoryCard memoryCard = getChildT<MemoryCard>(FlashUtils::CMath::intToString(ind.x) + FlashUtils::CMath::intToString(ind.y));

	if (_state == mfFirstFound) {
		if (memoryCard->getState() == mcNormal) {
			rotateCard(ind, true);
			_secondCardIndex = ind;
			memoryCard->select();
			_state = mfSecondFound;
		}
	}

	if (_state == mfWaiting) {
		if (memoryCard->getState() == mcNormal) {
			rotateCard(ind, true);
			_firstCardIndex = ind;
			memoryCard->select();
			_state = mfFirstFound;
		}
	}
}

int MemoryField::rotateCard(Point ind, bool show) {
	spTween tween = getChildT<MemoryCard>(FlashUtils::CMath::intToString(ind.x) + FlashUtils::CMath::intToString(ind.y))->playAnimation(show);
	if (tween) {
		tween->addEventListener(TweenEvent::DONE, CLOSURE(this, &MemoryField::endAnimationCallback));
		animated_count++;
	}
	return 0;
}

void MemoryField::endAnimationCallback(Event *ev) {
	animated_count--;

	if (animated_count == 0 && _state == mfSecondFound) {
		spMemoryCard firstCard = getChildT<MemoryCard>(FlashUtils::CMath::intToString(_firstCardIndex.x) + FlashUtils::CMath::intToString(_firstCardIndex.y));
		spMemoryCard secondCard = getChildT<MemoryCard>(FlashUtils::CMath::intToString(_secondCardIndex.x) + FlashUtils::CMath::intToString(_secondCardIndex.y));
		
		if (areSelectedCardsPair(firstCard, secondCard)) {
			firstCard->setCardScored();
			secondCard->setCardScored();
			dispatchCardsScored();
			_state = mfWaiting;
		}
		else {
			spTween tween = addTween(TweenDummy(), 500, 1);
			tween->addEventListener(TweenEvent::DONE, CLOSURE(this, &MemoryField::endAnimationCallback));
			animated_count++;
			_state = mfChecking;
		}
	}
	else if (animated_count == 0 && _state == mfChecking) {
		_state = mfBackSwaping;
		spMemoryCard firstCard = getChildT<MemoryCard>(FlashUtils::CMath::intToString(_firstCardIndex.x) + FlashUtils::CMath::intToString(_firstCardIndex.y));
		spMemoryCard secondCard = getChildT<MemoryCard>(FlashUtils::CMath::intToString(_secondCardIndex.x) + FlashUtils::CMath::intToString(_secondCardIndex.y));
		firstCard->unSelect();
		secondCard->unSelect();
		rotateCard(_firstCardIndex, false);
		rotateCard(_secondCardIndex, false);
	}
	else if (animated_count == 0 && _state == mfBackSwaping) {
		_state = mfWaiting;
	}
}

bool MemoryField::areSelectedCardsPair(spMemoryCard first, spMemoryCard second) {
	return first->getId() == second->getId();
}

Point MemoryField::getCellIndex(Vector2 position) {
	return Point(position.x / MEMORY_SIZE_X, position.y / MEMORY_SIZE_Y);
}

Vector2	MemoryField::getCellPosition(int i, int j) {
	return Vector2(i * MEMORY_SIZE_X + MEMORY_SIZE_X / 2, j * MEMORY_SIZE_Y + MEMORY_SIZE_Y / 2);
}

void MemoryField::dispatchCardsScored() {
	MemoryFieldEvent memoryFieldEvent(MemoryFieldEvent::CARDS_SCORED);
	dispatchEvent(&memoryFieldEvent);
}