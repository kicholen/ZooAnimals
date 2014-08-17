#include "FurthLevelFrame.h"
#include "FlurryAnalytics.h"
#include "SoundPlayer.h"
#include "SoundInstance.h"
#include "ResSound.h"

FourthLevelFrame::FourthLevelFrame() {
	init("FourthLevelFrame.xml", true);
	slideFrame->addEventListener(SlideFrame::GoBackToPreviousFrameEvent::GO_BACK, CLOSURE(this, &FourthLevelFrame::onGoBack));

	selectTransitions();
}

void FourthLevelFrame::selectTransitions() {
	spTransition transition = new TransitionQuads;
	setTransitionIn(transition);
	setTransitionOut(transition);
}

void FourthLevelFrame::_postHiding(Event *) {
	FlurryAnalytics::instance.onLevelLeaveEvent("level_4");
	slideFrame->removeEventListener(SlideFrame::GoBackToPreviousFrameEvent::GO_BACK, CLOSURE(this, &FourthLevelFrame::onGoBack));
	_view->removeChildren();
	_resources.unload();
}

void FourthLevelFrame::_preShowing(Event *) {
	FlurryAnalytics::instance.onLevelEnterEvent("level_4");
	selectTransitions();
	_resources.load();
	setData();
}

Action FourthLevelFrame::loop() {
	while (1) {
		Action action = waitAction();
		if (action.id == "slideFrame" || action.id == "_btn_back_") {
			transitionShowFrameAsDialog(slideFrame);
		}
		else if (action.id == "back") {
			break;
		}
		else if (action.id == "close") {
			break;
		}
	}

	return _lastAction;
}

void FourthLevelFrame::onGoBack(Event *event) {
	const string &name ="close";
	generateAction(name);
}

void FourthLevelFrame::onShowSliderFrame(Event *event) {
	const string &name ="slideFrame";
	generateAction(name);
}

void FourthLevelFrame::setData() {
	spButton sliderButton = new Button();
	sliderButton->setResAnim(gameResources.getResAnim("back_button"));
	sliderButton->setScale(_content->getWidth() * 8 / 100 / sliderButton->getWidth());
	sliderButton->setPosition(Vector2(_content->getWidth() * 9 / 10 - 12, _content->getHeight() * 95 / 100 - sliderButton->getDerivedHeight()));

	sliderButton->setName("slideFrame");
	sliderButton->addEventListener(TouchEvent::TOUCH_DOWN, CLOSURE(this, &FourthLevelFrame::onShowSliderFrame));
	
	Array<Group*> *groupArray = new Array<Group*>();

	groupArray = SpriteSpawner::getLevelData(4);

	for(int i = 0; i < groupArray->length(); i++) {
		Group &g = *(*groupArray)[i];
		if(g[0] > 0) {
			addDraggableSprite(g.spriteName, Vector2(0.5f, 0.5f), g[0], g[1], g[2], g[3], g[4]);
		}
	}
	Particle::initialize("test_particle");
	spParticleEmitter particleEmitter = new ParticleEmitter(Vector2(_view->getWidth() / 2, _view->getWidth() / 2), Vector2(_view->getHeight() * 9 / 10, _view->getHeight() * 9 / 10), Vector2(-0.005, 0.005), Vector2(-0.1, -0.05), Vector2(3, 5), Vector2(2, 5));
	particleEmitter->setRadius(Vector2(_view->getHeight() / 600, _view->getHeight() / 550));
	particleEmitter->attachTo(_view);

	spParticleEmitter particleEmitter_2 = new ParticleEmitter(Vector2(_view->getWidth() * 2 / 10, _view->getWidth() * 2 / 10), Vector2(_view->getHeight() * 8 / 10,  _view->getHeight() * 8 / 10), Vector2(-0.01, 0.01), Vector2(-0.05, -0.025), Vector2(2, 4), Vector2(2, 5));
	particleEmitter->setRadius(Vector2(_view->getHeight() / 550, _view->getHeight() / 400));
	particleEmitter_2->attachTo(_view);

	spParticleEmitter particleEmitter_3 = new ParticleEmitter(Vector2(_view->getWidth() * 7 / 10, _view->getWidth() * 7 / 10), Vector2(_view->getHeight() * 7 / 10,  _view->getHeight() * 7 / 10), Vector2(-0.01, 0.01), Vector2(-0.05, -0.025), Vector2(2, 4), Vector2(2, 5));
	particleEmitter->setRadius(Vector2(_view->getHeight() / 550, _view->getHeight() / 400));
	particleEmitter_3->attachTo(_view);

	sliderButton->attachTo(_view);
	wasWhaleAnimated = false;
	wasKrzakAnimated = false;
}

void FourthLevelFrame::addDraggableSprite(string spriteName, Vector2 anchorPoint, float scaleX, float scaleY, float positionX, float positionY, int priorityZ) {
	spSprite sprite = new Sprite;
	sprite->setResAnim(_resources.getResAnim(spriteName));
	sprite->setAnchor(anchorPoint);
	sprite->setScale(scaleX * getRoot()->getWidth() / sprite->getWidth(), scaleY * getRoot()->getHeight() / sprite->getHeight());
	sprite->setPosition(positionX * getRoot()->getWidth(), positionY * getRoot()->getHeight());
	sprite->setName(spriteName);
	sprite->setPriority(priorityZ);
	addTouchEventListenerIfNeeded(sprite);
	if (spriteName == "shark") {
		int randomTime = FlashUtils::CMath::random(8000, 15000);
		spTween cloudTween = createTween(Actor::TweenX(sprite->getX() + (-sprite->getX() - sprite->getDerivedWidth() / 2)), randomTime, 1, false);
		sprite->addTween(Actor::TweenY(sprite->getY() - sprite->getDerivedHeight() * 0.6f), randomTime * 0.2, 1, false, randomTime * 0.1f);
		sprite->addTween(Actor::TweenY(sprite->getY()), randomTime * 0.2, 1, false, randomTime * 0.7f);
		sprite->addTween(cloudTween);
		_sharkBaseScaleY = sprite->getScaleY();
		cloudTween->addEventListener(TweenEvent::DONE, CLOSURE(this, &FourthLevelFrame::onSharkOutOfScreen));	
	}
	_view->addChild(sprite);
}

void FourthLevelFrame::addTouchEventListenerIfNeeded(spSprite sprite) {
	string spriteName = sprite->getName();
	string animatableObjectList[] =  {"crab", "dolphin", "krzak", "octopus", "whale", "shark"};
	int sizeOfStringArray = sizeof(animatableObjectList) / sizeof( animatableObjectList[0]);
	bool shouldMakeUntouchable = true;

	for (int i = 0; i < sizeOfStringArray; i++) {
		if (sprite->getName() == animatableObjectList[i]) {
			sprite->addEventListener(TouchEvent::CLICK, CLOSURE(this, &FourthLevelFrame::onAnimatableObjectTap));
			shouldMakeUntouchable = false;
		}
	}

	if (shouldMakeUntouchable) {
		sprite->setInputEnabled(false);
	}
}

void FourthLevelFrame::onAnimatableObjectTap(Event *event) {
	spActor actor = safeSpCast<Actor>(event->currentTarget);
	string name = event->currentTarget->getName();
	int duration = 1000;
	float scaleMultiplier = 1.2f;
	float scaleNotMultiWow = 0.8f;

	if (name == "shark") {
		FlurryAnalytics::instance.onAnimatableObjectTapEvent("level_1", name.c_str());

		spTween currentTween = actor->getFirstTween();
		bool shouldTween = true;
		while (currentTween) {
			if (currentTween->getName() == "removable") {
				shouldTween = false;
			}
			currentTween = currentTween->getNextSibling();
		}
		if (shouldTween) {
			spTween sharkTween = createTween(Actor::TweenScaleY(-actor->getScaleY()), duration, 1, true);
			sharkTween->setName("removable");
			actor->addTween(sharkTween);
			stopPreviousAndPlayNewSound(name);
		}
	}
	if (!actor->getFirstTween()) {
		FlurryAnalytics::instance.onAnimatableObjectTapEvent("level_4", name.c_str());

		if (name == "crab") {
			actor->addTween(Actor::TweenRotation(actor->getRotation() + (float)MATH_PI / 4), duration / 4, 1, true, 0, Tween::ease_inBack);
			actor->addTween(Actor::TweenPosition(actor->getPosition() + Vector2(-(actor->getDerivedWidth() * 2), 0)), duration, 1, true);
			spActor crabActor = _view->getFirstChild();
			while (crabActor) {
				if (crabActor->getName() == "crab" && crabActor != actor && !crabActor->getFirstTween()) {
					crabActor->addTween(Actor::TweenRotation(crabActor->getRotation() + (float)MATH_PI / 4), duration / 4, 1, true, duration, Tween::ease_inBack);
					crabActor->addTween(Actor::TweenPosition(crabActor->getPosition() + Vector2(-(actor->getDerivedWidth() * 2), 0)), duration * 5 / 4, 1, true, duration);
				}
				crabActor = crabActor->getNextSibling();
			}
			stopPreviousAndPlayNewSound(name);
		}
		else if (name == "dolphin") {
			actor->addTween(Actor::TweenRotation(actor->getRotation() + (float)MATH_PI * 2), duration * 2, 1, true);
			actor->addTween(Actor::TweenPosition(actor->getPosition() + Vector2(-actor->getDerivedWidth(), -actor->getDerivedHeight() / 2)), duration * 2, 1, true);
			stopPreviousAndPlayNewSound(name);
		}
		else if (name == "krzak") {
			actor->addTween(Actor::TweenScale(actor->getScale() * scaleNotMultiWow), duration * 2, 1, true);
			spActor catchFish = _view->getFirstChild();
			while (catchFish) {
				if (catchFish->getName() == "fish_1") {
					catchFish->addTween(Actor::TweenPosition(catchFish->getPosition() + Vector2(-catchFish->getDerivedWidth() * 2, 0)), duration, 1, true);
					catchFish->addTween(Actor::TweenScaleX(-catchFish->getScaleX()), duration, 1, false, 0);
				}
				if (catchFish->getName() == "fish_2") {
					catchFish->addTween(Actor::TweenPosition(catchFish->getPosition() + Vector2(-catchFish->getDerivedWidth() * 2, catchFish->getDerivedHeight())), duration, 1, true);
					catchFish->addTween(Actor::TweenScaleX(-catchFish->getScaleX()), duration, 1, false, 0);
				}
				if (catchFish->getName() == "fish_3") {
					catchFish->addTween(Actor::TweenPosition(catchFish->getPosition() + Vector2(-catchFish->getDerivedWidth() * 3, -catchFish->getDerivedHeight() * 2)), duration, 1, true);
					catchFish->addTween(Actor::TweenScaleX(-catchFish->getScaleX()), duration, 1, false, 0);
				}
				catchFish = catchFish->getNextSibling();
			}
			stopPreviousAndPlayNewSound("fish");
		}
		else if (name == "octopus") {
			actor->addTween(Actor::TweenRotation(actor->getRotation() + (float)MATH_PI * 2), duration, 1);
			stopPreviousAndPlayNewSound(name);
		}
		else if (name == "whale") {
			actor->addTween(Actor::TweenPosition(actor->getPosition() + Vector2(-actor->getDerivedWidth(), 0)), duration, 1, true);
			actor->addTween(Actor::TweenScaleX(-actor->getScaleX()), duration, 1, false, 0);
			stopPreviousAndPlayNewSound(name);
		}
	}
}

void FourthLevelFrame::stopPreviousAndPlayNewSound(string soundName) {
	if (_previousSoundInstance) {
		_previousSoundInstance->stop();
	}
	_previousSoundInstance = sPlayer.play(soundName + "_sound");
}

void FourthLevelFrame::onSharkOutOfScreen(Event *event) {
	spTween tween= safeSpCast<Tween>(event->currentTarget);
	spActor tweenedActor = tween->getClient();

	spTween currentTween = tweenedActor->getFirstTween();
	bool shouldTween = true;
	while (currentTween) {
		if (currentTween->getName() == "removable") {
			tweenedActor->removeTween(currentTween);
			break;
		}
		currentTween = currentTween->getNextSibling();
	}
	tweenedActor->setScaleY(_sharkBaseScaleY);
	tweenedActor->setX(_view->getWidth() + tweenedActor->getDerivedWidth() / 2);
	int randomTime = FlashUtils::CMath::random(18000, 30000);
	tweenedActor->addTween(Actor::TweenY(tweenedActor->getY() + tweenedActor->getDerivedHeight()), randomTime * 0.1, 1, false);
	tweenedActor->addTween(Actor::TweenY(tweenedActor->getY()), randomTime * 0.1, 1, false, randomTime * 0.3f);
	
	tweenedActor->addTween(Actor::TweenY(tweenedActor->getY() - tweenedActor->getDerivedHeight() * 0.6f), randomTime * 0.1, 1, false, randomTime * 0.5f);
	tweenedActor->addTween(Actor::TweenY(tweenedActor->getY()), randomTime * 0.1, 1, false, randomTime * 0.9f);
	
	//tweenedActor->addTween(Actor::TweenY(tweenedActor->getY()), randomTime * 0.2, 1, false, randomTime * 0.7f);
	tweenedActor->addTween(Actor::TweenX(tweenedActor->getX() +(-tweenedActor->getX() - tweenedActor->getDerivedWidth() / 2)), randomTime, 1, false)->addEventListener(TweenEvent::DONE, CLOSURE(this, &FourthLevelFrame::onSharkOutOfScreen));
}