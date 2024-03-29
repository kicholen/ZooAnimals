#include "MatchTwoField.h"
#include "SharedResources.h"
#include "pugixml/pugixml.hpp"
#include "Content.h"

MatchTwoField::MatchTwoField(Vector2 size, uint numberOfMatches, uint pairsFromContent) : _numberOfMatches(0) {
	setSize(size);
	addEventListener(TouchEvent::TOUCH_UP, CLOSURE(this, &MatchTwoField::touchUp));

	restart(numberOfMatches, pairsFromContent);
}

MatchTwoField::~MatchTwoField() {
	_isSpotTaken._vector.resize(0);
}

void MatchTwoField::restart(uint numberOfMatches, uint pairsFromContent) {
	createSpotTakenVector(numberOfMatches);
	_animatedCount = 0;
	_state = mtAnimating;
	_previousNumberOfMatches = _numberOfMatches;
	_numberOfMatches = numberOfMatches;
	hidePreviousMatchesIfNecessary();
	_numberOfMatchesFound = 0;
	fillField(pairsFromContent);
}

void MatchTwoField::fillField(uint pairsFromContent) {
	int bufferIndex = 1;
	pugi::xml_node levelNode = Content::instance.getMatchTwoLevelNode(pairsFromContent).first_child();

	for(uint i = 0; i < _numberOfMatches; i++) {
		std::string name = FlashUtils::CMath::intToString(bufferIndex);
		spMatchTwoSlot slot = getChildT<MatchTwoSlot>(name + "_basket", oxygine::ep_ignore_error);
		if (!slot) {
			slot = new MatchTwoSlot();
			slot->setAnchor(0.5f, 0.5f);
			slot->setName(name + "_basket");
			slot->setPriority(-1);
			addChild(slot);
		}
		_animatedCount++;
		pugi::xml_attribute firstAttribute = levelNode.first_attribute();
		slot->setSprite(firstAttribute.as_string());
		levelNode = levelNode.next_sibling();
		firstAttribute = levelNode.first_attribute();
		slot->setVisible(true);
		slot->setPosition(getSlotPosition(bufferIndex));
		Vector2 slotScale = Vector2(getSlotSize().x / MATCH_SLOT_SIZE_X, getSlotSize().y / MATCH_SLOT_SIZE_Y);
		slot->setScale(slotScale);
		slot->setX(getWidth() + slot->getDerivedWidth());
		spTween tween = slot->addTween(Sprite::TweenX(getSlotPosition(bufferIndex).x), 250, 1, false, 75 * bufferIndex, Tween::ease_inOutBack);
		tween->setDoneCallback(CLOSURE(this, &MatchTwoField::animationEndCallback));
		
		createDraggableSprite(slot, name, slotScale, firstAttribute.as_string());
		bufferIndex++;

		levelNode = levelNode.next_sibling();
	}
}

void MatchTwoField::animationEndCallback(Event *event) {
	_animatedCount--;

	if (_animatedCount == 0) {
		_state = mtWaiting;
	}
}

Vector2	MatchTwoField::getSlotPosition(int i) {
	float yPosition = getHeight() / _numberOfMatches * (float(i) - 0.5f);

	return Vector2(getWidth() - getSlotSize().x / 2, yPosition);
}

Vector2 MatchTwoField::getSlotSize() {
	float ratio = MATCH_SLOT_SIZE_X / MATCH_SLOT_SIZE_Y;
	
	if (getWidth() / 3 / ratio * _numberOfMatches > getHeight() * 0.95f) {
		float height = getHeight() / _numberOfMatches * 0.9f;
		return Vector2(height * ratio, height);
	}
	else {
		return Vector2(getWidth() / 3, getWidth() / 3 / ratio);
	}
}

void MatchTwoField::createDraggableSprite(spMatchTwoSlot slot, std::string name, Vector2 slotScale, std::string spriteName) {
	spMatchTwoDraggable draggableSprite = getChildT<MatchTwoDraggable>(name, oxygine::ep_ignore_error);
	
	if (!draggableSprite) {
		draggableSprite = new MatchTwoDraggable();
		draggableSprite->setAnchor(Vector2(0.5f, 0.5f));
		draggableSprite->setName(name);
		draggableSprite->attachTo(this);
		draggableSprite->setDragBounds(getPosition(), getSize());
		draggableSprite->addEventListener(TouchEvent::TOUCH_DOWN, CLOSURE(this, &MatchTwoField::spriteTouchDown));
		draggableSprite->addEventListener(TouchEvent::TOUCH_UP, CLOSURE(this, &MatchTwoField::spriteTouchUp));
	}
	draggableSprite->setVisible(true);
	draggableSprite->setPriority(1);
	draggableSprite->setTouchEnabled(true);
	draggableSprite->setResAnim(animalsResources.getResAnim(spriteName));
	Vector2 basePosition = Vector2(getWidth() * 0.2f, getSlotPosition(getRandomFreeSpot()).y);
	draggableSprite->setBasePosition(basePosition);
	draggableSprite->setX(basePosition.x);
	draggableSprite->setY(basePosition.y);
	slot->fitToSlot(draggableSprite);
	draggableSprite->setScale(Vector2(draggableSprite->getScaleX() * slotScale.x, draggableSprite->getScaleY() * slotScale.y));
}

void MatchTwoField::spriteTouchDown(Event *event) {
	if (_state == mtDragging) {
		return;
	}
	_dragging = safeSpCast<MatchTwoDraggable>(event->currentTarget);
	_baseDraggingScale = _dragging->getScale();
	_dragging->setScale(_baseDraggingScale * 1.1f);
	_dragging->setPriority(_dragging->getPriority() + 1);
	_state = mtDragging;
}

void MatchTwoField::spriteTouchUp(Event *event) {
	// unblock objects touch
	_dragging = safeSpCast<MatchTwoDraggable>(event->currentTarget);
	_dragging->setPriority(_dragging->getPriority() - 1);
	_state = mtDragging;
}

void MatchTwoField::touchUp(Event *event) {
	if (!_dragging) {
		return;
	}

	if (event->currentTarget.get() != this) {
		return;
	}

	//if (_state != mtDragging) {
	//	return;
	//}

	spMatchTwoSlot slot = getChildT<MatchTwoSlot>(_dragging->getName() + "_basket");
	_state = mtAnimating;

	Vector2 localPosition = safeCast<TouchEvent*>(event)->localPosition;
	if (RectF(slot->getPosition() - slot->getSize() / 2, slot->getSize()).pointIn(Point(int(localPosition.x), int(localPosition.y)))) {
		onProperSlotFound(slot->getBasketPosition());
	}
	else {
		setTouchEnabled(false);
		setTouchChildrenEnabled(false);
		_dragging->addTween(Actor::TweenScale(_baseDraggingScale), 300);
		_dragging->addTween(Actor::TweenPosition(_dragging->getBasePosition()), 300, 1, false, 0, Tween::ease_outBack)->setDoneCallback(CLOSURE(this, &MatchTwoField::onBackAnimationFinished));
	}
	_dragging = 0;
}

void MatchTwoField::onBackAnimationFinished(Event *event) {
	setTouchChildrenEnabled(true);
	setTouchEnabled(true);
	_state = mtWaiting;
}

void MatchTwoField::onProperSlotFound(Vector2 animateTo) {
	//spSprite clonedDragSprite = _dragging->clone();
	//_dragging->detach();
	//addChild(clonedDragSprite);
	_dragging->addTween(Actor::TweenScale(_baseDraggingScale), 300);
	_dragging->addTween(Actor::TweenPosition(animateTo), 300, 1, false, 0, Tween::ease_outBack)->setDoneCallback(CLOSURE(this, &MatchTwoField::onSlotFoundAnimationCompleted));
	_dragging->setTouchEnabled(false);
}

void MatchTwoField::onSlotFoundAnimationCompleted(Event *event) {
	_numberOfMatchesFound++;

	if (_numberOfMatchesFound == _numberOfMatches) {
		MatchTwoFieldEvent finishedEvent(MatchTwoFieldEvent::FINISHED);
		dispatchEvent(&finishedEvent);
	}
}

void MatchTwoField::hidePreviousMatchesIfNecessary() {
	if (_previousNumberOfMatches > _numberOfMatches) {
		for (uint i = _numberOfMatches + 1; i <= _previousNumberOfMatches; i++) {
			std::string name = FlashUtils::CMath::intToString(i);
			spMatchTwoDraggable draggableSprite = getChildT<MatchTwoDraggable>(name);
			draggableSprite->setVisible(false);
			spMatchTwoSlot slot = getChildT<MatchTwoSlot>(name + "_basket");
			slot->setVisible(false);
		}
	}
}

void MatchTwoField::createSpotTakenVector(uint numberOfMatches) {
	_isSpotTaken._vector.resize(0);
	_isSpotTaken._vector.reserve(numberOfMatches);
	for (uint i = 0; i < numberOfMatches; i++) {
		_isSpotTaken._vector.push_back(false);
	}
}

uint MatchTwoField::getRandomFreeSpot() {
	bool untilFound = true;
	int freeSpot = 0;
	int preformanceCounter = 0;
	int randomCounter = CMath::random(2, _numberOfMatches * 3);

	while (untilFound) {
		for (int i = 0; i < _isSpotTaken.length(); i++) {
			preformanceCounter++;

			if (!_isSpotTaken[i]) {
				randomCounter--;
				if (randomCounter == 0) {
					freeSpot = i;
					untilFound = false;
					_isSpotTaken._vector.erase(_isSpotTaken._vector.begin() + i);
					_isSpotTaken._vector.insert(_isSpotTaken._vector.begin() + i, 1, true);
				}
			}
		}
	}
	return freeSpot + 1;
}