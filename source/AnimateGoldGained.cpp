#include "AnimateGoldGained.h"
#include "SharedResources.h"
#include "FlashUtils.h"

// maybe convert it to action?
AnimateGoldGained::AnimateGoldGained(spActor parent, int goldAmount, const Vector2& startPosition, const Vector2& endPosition) {
	_parent = parent;
	_goldAmount = goldAmount;
	_startPosition = startPosition;
	_endPosition = endPosition;
	_gold = createGoldSprite(startPosition);
	_gold->attachTo(parent);
	_gold->setVisible(false);
}

AnimateGoldGained::~AnimateGoldGained() {
	if (_gold) {
		_gold->detach();
	}
}

void AnimateGoldGained::process() {
	if (_part == 0) {
		_gold->setVisible(true);
		float randomOffsetX = FlashUtils::CMath::Rand(-_gold->getDerivedWidth() * 2.0f, _gold->getDerivedWidth() * 2.0f);
		_gold->addTween(Actor::TweenX(_gold->getX() + randomOffsetX), 800);
		_gold->addTween(Actor::TweenY(_gold->getY() - _gold->getDerivedHeight() * 2.0f), 800, 1, false, 0, Tween::ease_outBounce);
	}
	else if (_part == 1) {
		
	}
	else {

	}
}

bool AnimateGoldGained::completed() {
	return _completed;
}

spSprite AnimateGoldGained::createGoldSprite(const Vector2& startPosition) {
	spSprite sprite = new Sprite();
	sprite->setResAnim(tilesResources.getResAnim("goldCoin"));
	sprite->setAnchor(0.5f, 0.5f);
	setSpriteScaleBySize(sprite, Vector2(getRoot()->getWidth() * 0.05f * getScaleFactor(), getRoot()->getHeight() * 0.05f * getScaleFactor()));
	sprite->setPosition(startPosition);
	sprite->addEventListener(TouchEvent::CLICK, CLOSURE(this, &AnimateGoldGained::onCoinClicked));

	return sprite;
}

float AnimateGoldGained::getScaleFactor() {
	return 1.0f;
}

void AnimateGoldGained::onCoinClicked(Event *event) {
	_gold->removeTweens();
	_gold->addTween(Actor::TweenPosition(_endPosition), 500, 1, false, 0, Tween::ease_inQuad)->setDoneCallback(CLOSURE(this, &AnimateGoldGained::onAnimationCompleted));
}

void AnimateGoldGained::onAnimationCompleted(Event *event) {
	_gold->setVisible(false);
	_completed = true;
}