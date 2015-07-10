#include "FindShadowField.h"
#include "SharedResources.h"
#include "FlashUtils.h"

FindShadowField::FindShadowField(Vector2 size, int numberOfShadows, std::string mainObjectName) {
	setSize(size);
	_animatedCount = 0;
	_state = fsWaiting;
	createRotatingContainer();

	restart(numberOfShadows, mainObjectName);
}

void FindShadowField::restart(int numberOfShadows, std::string mainObjectName) {
	_wheel->clear();
	fillField(numberOfShadows, mainObjectName);
	animateField();
}

void FindShadowField::fillField(int numberOfShadows, std::string mainObjectName) {
	_mainSprite = createSprite(mainObjectName, mainObjectName, false);
	_mainSprite->setPriority(10);
	_wheel->addMainChild(_mainSprite);

	for (int i = 0; i < numberOfShadows; i++) {
		spSprite shadowSprite = createSprite("1", mainObjectName, true);
		_wheel->addSubChild(shadowSprite);
		shadowSprite->addEventListener(TouchEvent::CLICK, CLOSURE(this, &FindShadowField::onShadowTap));
	}
}

void FindShadowField::animateField() {
	_animatedCount++;
	_state = fsAnimating;
	_wheel->setX(-_wheel->getWidth());
	_wheel->setRotation(FlashUtils::CMath::DegToRad(360.0f));
	_wheel->addTween(Actor::TweenX(getWidth() / 2), 1500, 1, false, 0, Tween::ease_outBack);
	_wheel->addTween(Actor::TweenRotation(FlashUtils::CMath::DegToRad(0.0f)), 1500, 1, false, 0, Tween::ease_outBack)->setDoneCallback(CLOSURE(this, &FindShadowField::animateEndCallback));
}

void FindShadowField::animateEndCallback(Event *event) {
	_animatedCount--;
	if (_animatedCount == 0 ) {
		_state = fsWaiting;
	}
}

void FindShadowField::onShadowTap(Event *event) {
	if (_state != fsWaiting) {
		return;
	}

	spActor actor = safeSpCast<Actor>(event->currentTarget);
	if (actor->getName() == "1") {
		animateProperShadowFound(actor);
	}
	else {
		dispatchShadowWrongEvent();
	}
}

void FindShadowField::animateProperShadowFound(spActor actor) {
	_state = fsCompleted;
	actor->addTween(Actor::TweenScale(Vector2(_mainSprite->getDerivedWidth() / actor->getWidth(), _mainSprite->getDerivedHeight() / actor->getHeight())), 1000, 1, false, 0, Tween::ease_inBack); 
	actor->addTween(Actor::TweenPosition(_mainSprite->getPosition()), 1000);
	_wheel->addTween(Actor::TweenX(_wheel->getWidth() + getRoot()->getWidth()), 1500, 1, false, 1500);
	_wheel->addTween(Actor::TweenRotation(FlashUtils::CMath::DegToRad(-360.0f)), 1500, 1, false, 1500)->setDoneCallback(CLOSURE(this, &FindShadowField::dispatchShadowFoundEvent));
}

void FindShadowField::dispatchShadowFoundEvent(Event *event) {
	FindShadowFieldEvent findEvent(FindShadowFieldEvent::SHADOW_FOUND);
	dispatchEvent(&findEvent);
}

void FindShadowField::dispatchShadowWrongEvent() {
	FindShadowFieldEvent findEvent(FindShadowFieldEvent::SHADOW_WRONG);
	dispatchEvent(&findEvent);
}

spSprite FindShadowField::createSprite(std::string name, std::string spriteName, bool isShadowSprite) {
	spSprite sprite = getChildT<Sprite>(name, oxygine::ep_ignore_error);
	
	if (!sprite) {
		sprite = new Sprite();
		sprite->setAnchor(0.5f, 0.5f);
		sprite->setVisible(true);
	}
	sprite->setName(name);
	sprite->setResAnim(animalsResources.getResAnim(spriteName));
	if (isShadowSprite) {
		sprite->setColor(Color(0, 0, 0));
	}

	return sprite;
}

void FindShadowField::createRotatingContainer() {
	_wheel = new RotatingContainer2d(Vector2(getHeight() * 0.8f, getHeight() * 0.8f));
	_wheel->setAnchor(0.5f, 0.5f);
	_wheel->setPosition(getWidth() / 2, getHeight() / 2);
	_wheel->attachTo(this);
}