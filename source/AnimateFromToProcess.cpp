#include "AnimateFromToProcess.h"
#include "SharedResources.h"

AnimateFromToProcess::AnimateFromToProcess(const std::string& resource, spActor parent, const Vector2& from, const Vector2& to) {
	_sprite = createSprite(resource);
	_sprite->setPosition(from);
	parent->addChild(_sprite);
	_to = to;
	_canProcess = true;
	_completed = false;
}

AnimateFromToProcess::~AnimateFromToProcess() {
	_sprite->detach();
}

void AnimateFromToProcess::process() {
	if (_part == 0) {
		_sprite->addTween(Actor::TweenX(_to.x), 1500, 1, false, 500, Tween::ease_outBack)->setDoneCallback(CLOSURE(this, &AnimateFromToProcess::onAnimationCompleted));
		_sprite->addTween(Actor::TweenY(_to.y), 1000, 1, false, 500, Tween::ease_outBack);
	}
	else if (_part == 1) {
		_completed = true;
	}
}

bool AnimateFromToProcess::completed() {
	return _completed;
}

spSprite AnimateFromToProcess::createSprite(const std::string& resource) {
	spSprite sprite = new Sprite();
	sprite->setResAnim(gameResources.getResAnim(resource));
	sprite->setAnchor(0.5f, 0.5f);
	setSpriteScaleBySize(sprite, Vector2(getRoot()->getWidth() * 0.05f, getRoot()->getHeight() * 0.05f));

	return sprite;
}

void AnimateFromToProcess::onAnimationCompleted(Event *event) {
	_part = 1;
}