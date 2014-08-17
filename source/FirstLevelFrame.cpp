#include "FirstLevelFrame.h"
#include "FlurryAnalytics.h"
#include "SoundPlayer.h"
#include "SoundInstance.h"
#include "ResSound.h"

FirstLevelFrame::FirstLevelFrame() {
	init("FirstLevelFrame.xml", true);
	slideFrame->addEventListener(SlideFrame::GoBackToPreviousFrameEvent::GO_BACK, CLOSURE(this, &FirstLevelFrame::onGoBack));

	selectTransitions();
}

void FirstLevelFrame::selectTransitions() {
	spTransition transition = new TransitionQuads;
	setTransitionIn(transition);
	setTransitionOut(transition);
}

void FirstLevelFrame::_postHiding(Event *) {
	FlurryAnalytics::instance.onLevelLeaveEvent("level_1");
	slideFrame->removeEventListener(SlideFrame::GoBackToPreviousFrameEvent::GO_BACK, CLOSURE(this, &FirstLevelFrame::onGoBack));
	_view->removeChildren();
	_resources.unload();
}

void FirstLevelFrame::_preShowing(Event *) {
	FlurryAnalytics::instance.onLevelEnterEvent("level_1");
	selectTransitions();
	_resources.load();
	setData();
}

Action FirstLevelFrame::loop() {
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

void FirstLevelFrame::onGoBack(Event *event) {
	const string &name ="close";
	generateAction(name);
}

void FirstLevelFrame::onShowSliderFrame(Event *event) {
	const string &name ="slideFrame";
	generateAction(name);
}

void FirstLevelFrame::setData() {
	_areBarnDoorOpen = false;
	spButton sliderButton = new Button();
	sliderButton->setResAnim(gameResources.getResAnim("back_button"));
	sliderButton->setScale(_content->getWidth() * 8 / 100 / sliderButton->getWidth());
	sliderButton->setPosition(Vector2(_content->getWidth() * 9 / 10 - 12, _content->getHeight() * 95 / 100 - sliderButton->getDerivedHeight()));

	sliderButton->setName("slideFrame");
	sliderButton->addEventListener(TouchEvent::TOUCH_DOWN, CLOSURE(this, &FirstLevelFrame::onShowSliderFrame));
	
	Array<Group*> *groupArray = new Array<Group*>();

	groupArray = SpriteSpawner::getLevelData(1);

	for(int i = 0; i < groupArray->length(); i++) {
		Group &g = *(*groupArray)[i];
		if(g[0] > 0) {
			if (g.spriteName == "house") {
				Particle::initialize("snow");
				float spriteWidth = g[0] * _view->getWidth() * (0.874 - 0.5f);
				float spriteHeight = g[1] * _view->getHeight() * (0.5f - 0.056);
				float spritePositionX = g[2] * _view->getWidth() + spriteWidth;
				float spritePositionY = g[3] * _view->getHeight() - spriteHeight;

				spParticleEmitter smokeEmitter = new ParticleEmitter(Vector2(spritePositionX, spritePositionX), Vector2(spritePositionY, spritePositionY), Vector2(-0.005, 0.005), Vector2(-0.1, -0.05), Vector2(3, 5), Vector2(2, 5));
				smokeEmitter->pushColor(0x808080, true);
				smokeEmitter->setRadius(Vector2(_view->getHeight() / 400, _view->getHeight() / 340));
				smokeEmitter->attachTo(_view);
			}
			addDraggableSprite(g.spriteName, Vector2(0.5f, 0.5f), g[0], g[1], g[2], g[3], g[4]);
		}
	}

	sliderButton->attachTo(_view);
}

void FirstLevelFrame::addDraggableSprite(string spriteName, Vector2 anchorPoint, float scaleX, float scaleY, float positionX, float positionY, int priorityZ) {
	spSprite sprite = new Sprite;
	sprite->setResAnim(_resources.getResAnim(spriteName));
	sprite->setAnchor(anchorPoint);
	sprite->setScale(scaleX * getRoot()->getWidth() / sprite->getWidth(), scaleY * getRoot()->getHeight() / sprite->getHeight());
	sprite->setPosition(positionX * getRoot()->getWidth(), positionY * getRoot()->getHeight());
	sprite->setName(spriteName);
	sprite->setPriority(priorityZ);
	addTouchEventListenerIfNeeded(sprite);
	if (spriteName == "cloud") {
		int randomTime = FlashUtils::CMath::random(5000, 10000);
		spTween cloudTween = createTween(Actor::TweenPosition(sprite->getPosition() + Vector2(-sprite->getX() - sprite->getDerivedWidth() / 2, 0)), randomTime, 1, false);
		sprite->addTween(cloudTween);
		cloudTween->addEventListener(TweenEvent::DONE, CLOSURE(this, &FirstLevelFrame::onCloudOutOfScreen));	
	}
	_view->addChild(sprite);
}

void FirstLevelFrame::addTouchEventListenerIfNeeded(spSprite sprite) {
	string spriteName = sprite->getName();
	string animatableObjectList[] =  {"bee", "cat", "cloud", "cow", "dog", "duck", "goat", "horse", "house", "sheep", "snail", "sun", "tractor", "squirell", "owl", "mouse", "corn", "zboze", "flowers", "barn_2_door"};
	int sizeOfStringArray = sizeof(animatableObjectList) / sizeof( animatableObjectList[0]);
	bool shouldMakeUntouchable = true;

	for (int i = 0; i < sizeOfStringArray; i++) {
		if (sprite->getName() == animatableObjectList[i]) {
			sprite->addEventListener(TouchEvent::CLICK, CLOSURE(this, &FirstLevelFrame::onAnimatableObjectTap));
			shouldMakeUntouchable = false;
		}
	}

	if (shouldMakeUntouchable) {
		sprite->setInputEnabled(false);
	}
}

void FirstLevelFrame::onAnimatableObjectTap(Event *event) {
	spActor actor = safeSpCast<Actor>(event->currentTarget);
	string name = event->currentTarget->getName();
	int duration = 1000;
	float scaleMultiplier = 1.2f;

	if (name == "cloud") {
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
			spTween cloudTween = createTween(Actor::TweenRotation(actor->getRotation() + (float)MATH_PI * 2), duration * 2, 1, false);
			cloudTween->setName("removable");
			actor->addTween(cloudTween);
		}
	}
	else if (!actor->getFirstTween()) {
		FlurryAnalytics::instance.onAnimatableObjectTapEvent("level_1", name.c_str());

		if (name == "bee" || name == "flowers") {
			spActor beeActor = _view->getFirstChild();
			while (beeActor) {
				if (beeActor->getName() == "bee" && !beeActor->getFirstTween()) {
					spTweenQueue queue = new TweenQueue();			
					queue->add(Actor::TweenRotation(beeActor->getRotation() + (float)MATH_PI * 2.0f), duration / 2, 1, false);
					queue->add(Actor::TweenScale(beeActor->getScale() * scaleMultiplier), duration, 1, true);
					queue->add(Actor::TweenPosition(beeActor->getPosition() + Vector2(- FlashUtils::CMath::random(1, 3) * beeActor->getDerivedWidth(), - FlashUtils::CMath::random(1, 3) * beeActor->getDerivedHeight())), duration, 1, true);
					beeActor->addTween(queue);
				}
				beeActor = beeActor->getNextSibling();
			}
			stopPreviousAndPlayNewSound("bee");
		}
		else if (name == "cat") {
			actor->addTween(Actor::TweenRotation(actor->getRotation() + (float)MATH_PI * 2), duration * 2, 1, true);
			stopPreviousAndPlayNewSound(name);
		}
		else if (name == "cow") {
			actor->addTween(Actor::TweenScale(actor->getScale() * scaleMultiplier), duration / 2, 1, true);
			stopPreviousAndPlayNewSound(name);
		}
		else if (name == "dog") {
			spActor kotel = _view->getChild("cat");
			if (!kotel->getFirstTween()) {
				Vector2 kotelPosition = kotel->getPosition();
				kotel->addTween(Actor::TweenScale(actor->getScale() * scaleMultiplier), duration / 2, 1, true);
				kotel->addTween(Actor::TweenRotation(actor->getRotation() + (float)MATH_PI / 4), duration / 4, 1, true, 0, Tween::ease_inBack);
				kotel->addTween(Actor::TweenRotation(actor->getRotation() - (float)MATH_PI / 4), duration / 4, 1, true, duration / 4, Tween::ease_inBack);
				kotel->addTween(Actor::TweenPosition(kotelPosition + Vector2(- kotel->getDerivedWidth(), 2 * kotel->getDerivedHeight() / 3)), duration / 4, 1, false);
				kotel->addTween(Actor::TweenPosition(kotelPosition), duration / 3, 1, false, duration / 2);

				actor->addTween(Actor::TweenPosition(kotelPosition), duration / 2, 1, true);
				actor->addTween(Actor::TweenScale(actor->getScale() * scaleMultiplier), duration / 2, 1, true);
			}
			else {
				actor->addTween(Actor::TweenPosition(actor->getPosition() + Vector2(0, -actor->getDerivedHeight())), duration / 2, 1, true);
				actor->addTween(Actor::TweenScale(actor->getScale() * scaleMultiplier), duration / 2, 1, true);
			}
			stopPreviousAndPlayNewSound(name);
		}
		else if (name == "duck") {
			spActor duckActor = _view->getFirstChild();
			while (duckActor) {
				if (duckActor->getName() == "duck" && !duckActor->getFirstTween()) {
					duckActor->addTween(Actor::TweenRotation(actor->getRotation() + (float)MATH_PI / 4), duration / 2, 1, true, 0, Tween::ease_inBack);
					duckActor->addTween(Actor::TweenRotation(actor->getRotation() - (float)MATH_PI / 4), duration / 2, 1, true, duration / 2, Tween::ease_inBack);
					duckActor->addTween(Actor::TweenPosition(actor->getPosition() + Vector2(0, duckActor->getDerivedHeight())), duration, 1, true);
					duckActor->addTween(Actor::TweenRotation(actor->getRotation() + (float)MATH_PI / 4), duration / 2, 1, true,  duration, Tween::ease_inBack);
					duckActor->addTween(Actor::TweenRotation(actor->getRotation() - (float)MATH_PI / 4), duration / 2, 1, true,  duration * 3 / 2, Tween::ease_inBack);
				}
				duckActor = duckActor->getNextSibling();
			}
			stopPreviousAndPlayNewSound(name);
		}
		else if (name == "goat") {
			actor->addTween(Actor::TweenScale(actor->getScale() * scaleMultiplier), duration / 2, 1, true);
			actor->addTween(Actor::TweenRotation(actor->getRotation() + (float)MATH_PI / 4), duration / 4, 1, true, 0, Tween::ease_inBack);
			actor->addTween(Actor::TweenRotation(actor->getRotation() - (float)MATH_PI / 4), duration / 4, 1, true, duration / 4, Tween::ease_inBack);
			actor->addTween(Actor::TweenPosition(actor->getPosition() + Vector2(0, actor->getDerivedHeight())), duration / 2, 1, true);
			stopPreviousAndPlayNewSound(name);
		}
		else if (name == "horse") {
			actor->addTween(Actor::TweenScale(actor->getScale() * scaleMultiplier), duration / 2, 1, true);
			stopPreviousAndPlayNewSound("donkey");
		}
		else if (name == "sheep") {
			actor->addTween(Actor::TweenScale(actor->getScale() * scaleMultiplier), duration / 2, 1, true);
			stopPreviousAndPlayNewSound(name);
		}
		else if (name == "snail") {
			actor->addTween(Actor::TweenPosition(actor->getPosition() + Vector2(actor->getDerivedWidth() * 2, 0)), duration / 2, 1);
			actor->addTween(Actor::TweenPosition(actor->getPosition() + Vector2(-actor->getDerivedWidth() * 2, 0)), duration / 2, 1, false, duration / 2);
			actor->addTween(Actor::TweenScaleX(-actor->getScaleX()), duration / 2, 1, false, 0);
			stopPreviousAndPlayNewSound(name);
		}
		else if (name == "sun") {
			actor->addTween(Actor::TweenScale(actor->getScale() * scaleMultiplier), duration, 1, true);
		}
		else if (name == "tractor") {
			actor->addTween(Actor::TweenRotation(actor->getRotation() + (float)MATH_PI / 4), duration / 4, 1, true, 0, Tween::ease_inBack);
			actor->addTween(Actor::TweenPosition(actor->getPosition() + Vector2(-(actor->getDerivedWidth() * 2), 0)), duration, 1, true);
			
			spActor mouseActor = _view->getFirstChild();
			while (mouseActor) {
				if (mouseActor->getName() == "mouse" && !mouseActor->getFirstTween()) {
					mouseActor->addTween(Actor::TweenRotation(mouseActor->getRotation() - (float)MATH_PI / 4), duration, 1, true, 0, Tween::ease_inBack);
					mouseActor->addTween(Actor::TweenPosition(mouseActor->getPosition() + Vector2(0, mouseActor->getDerivedWidth() * 3)), duration, 1, true);
				}
				mouseActor = mouseActor->getNextSibling();
			}
			stopPreviousAndPlayNewSound(name);
		}
		else if (name == "squirell") {
			actor->addTween(Actor::TweenRotation(actor->getRotation() + (float)MATH_PI / 4), duration / 4, 1, true, 0, Tween::ease_inBack);
			actor->addTween(Actor::TweenRotation(actor->getRotation() - (float)MATH_PI / 4), duration / 4, 1, true, duration / 4, Tween::ease_inBack);
			actor->addTween(Actor::TweenPosition(actor->getPosition() + Vector2(actor->getDerivedWidth(), 0)), duration, 1, true);
			stopPreviousAndPlayNewSound("squirell");
		}
		else if (name == "owl") {
			actor->addTween(Actor::TweenPosition(actor->getPosition() + Vector2(0, -actor->getDerivedHeight())), duration, 1, true);
			stopPreviousAndPlayNewSound(name);
		}
		else if (name == "corn" || name == "zboze") {
			actor->addTween(Actor::TweenScaleX(actor->getScaleX() * scaleMultiplier), duration / 2, 1, true);
		}
		else if (name == "barn_2_door") {
			if (_areBarnDoorOpen) {
				_areBarnDoorOpen = false;
				actor->addTween(Actor::TweenPosition(actor->getPosition() + Vector2(-actor->getDerivedWidth(), 0)), duration, 1, false);
			}
			else {
				_areBarnDoorOpen = true;
				actor->addTween(Actor::TweenPosition(actor->getPosition() + Vector2(actor->getDerivedWidth(), 0)), duration, 1, false);
			}
		}
		//actor->addTween(Sprite::TweenColor(Color(255,0,0,255)), 300, -1, true);
	}
	//int new_priority = actor->getParent()->getLastChild()->getPriority() + 1;
	//actor->setPriority(new_priority);

}

void FirstLevelFrame::onCloudOutOfScreen(Event *event) {
	spTween tween= safeSpCast<Tween>(event->currentTarget);
	spActor tweenedActor = tween->getClient();
	tweenedActor->setX(_view->getWidth() + tweenedActor->getDerivedWidth() / 2);
	tweenedActor->addTween(Actor::TweenPosition(tweenedActor->getPosition() + Vector2(-tweenedActor->getX() - tweenedActor->getDerivedWidth() / 2, 0)), FlashUtils::CMath::random(10000, 20000), -1, false);
}

void FirstLevelFrame::stopPreviousAndPlayNewSound(string soundName) {
	if (_previousSoundInstance) {
		_previousSoundInstance->stop();
	}
	_previousSoundInstance = sPlayer.play(soundName + "_sound");
}
