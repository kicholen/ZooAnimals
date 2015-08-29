#include "AnimateGoldAction.h"
#include "SharedResources.h"
#include "FlashUtils.h"
#include "MoneyManager.h"

AnimateGoldAction::AnimateGoldAction(spActor parent, int goldAmount, const Vector2& startPosition, const Vector2& endPosition) {
	_parent = parent;
	_goldAmount = goldAmount;
	_startPosition = startPosition;
	_endPosition = endPosition;
	_gold = createGoldSprite(startPosition);
	_gold->attachTo(parent);

	float randomOffsetX = FlashUtils::CMath::Rand(-_gold->getDerivedWidth() * 2.0f, _gold->getDerivedWidth() * 2.0f);
	float scale = _gold->getScaleX();

	_gold->addTween(Actor::TweenX(_gold->getX() + randomOffsetX), 800, 1, false, 510);
	_gold->addTween(Actor::TweenY(_gold->getY() + _gold->getDerivedHeight() * 2.0f), 800, 1, false, 510, Tween::ease_outBounce);

	_gold->addTween(Actor::TweenX(_gold->getX() + randomOffsetX * 0.2f), 500, 1, false, 0, Tween::ease_inSin);
	_gold->addTween(Actor::TweenY(_gold->getY() - _gold->getDerivedHeight()), 500, 1, false, 0, Tween::ease_inSin);

	_gold->setScale(scale * 0.2f);
	_gold->addTween(Actor::TweenScale(scale), 500, 1, false, 0, Tween::ease_inSin);
}

AnimateGoldAction::~AnimateGoldAction()
{
}

void AnimateGoldAction::doAction() {
	
}

void AnimateGoldAction::complete() {
	MoneyManager::instance.addMoney(_goldAmount);
	if (_gold) {
		_gold->detach();
	}
	detach();
}

spSprite AnimateGoldAction::createGoldSprite(const Vector2& startPosition) {
	spSprite sprite = new Sprite();
	sprite->setResAnim(tilesResources.getResAnim("goldCoin"));
	sprite->setAnchor(0.5f, 0.5f);
	setSpriteScaleBySize(sprite, Vector2(getRoot()->getWidth() * 0.05f * getScaleFactor(), getRoot()->getHeight() * 0.05f * getScaleFactor()));
	sprite->setPosition(startPosition);
	sprite->addEventListener(TouchEvent::CLICK, CLOSURE(this, &AnimateGoldAction::onCoinClicked));
	sprite->setPriority(30000);

	return sprite;
}

float AnimateGoldAction::getScaleFactor() {
	if (false) {
		return 0.8f;
	}
	else if (true) {
		return 1.0f;
	}
	else if (false) {
		return 1.2f;
	}
}

void AnimateGoldAction::onCoinClicked(Event *event) {
	Vector2 position = convert_local2stage(_gold, Vector2(_gold->getDerivedWidth() / 2.0f, _gold->getDerivedHeight() / 2.0f));
	_gold->removeTweens();
	_gold->addRef();
	_gold->detach();
	_gold->attachTo(getStage());
	_gold->releaseRef();
	_gold->setPosition(position);
	_gold->addTween(Actor::TweenPosition(_endPosition), 500, 1, false, 0, Tween::ease_inSin)->setDoneCallback(CLOSURE(this, &AnimateGoldAction::onAnimationCompleted));
}

void AnimateGoldAction::onAnimationCompleted(Event *event) {
	complete();
}