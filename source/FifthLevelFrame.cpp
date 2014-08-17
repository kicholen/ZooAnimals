#include "FifthLevelFrame.h"
#include "FlurryAnalytics.h"
#include "SoundPlayer.h"
#include "SoundInstance.h"
#include "ResSound.h"

FifthLevelFrame::FifthLevelFrame() {
	init("FifthLevelFrame.xml", true);
	slideFrame->addEventListener(SlideFrame::GoBackToPreviousFrameEvent::GO_BACK, CLOSURE(this, &FifthLevelFrame::onGoBack));

	selectTransitions();
}

void FifthLevelFrame::selectTransitions() {
	spTransition transition = new TransitionQuads;
	setTransitionIn(transition);
	setTransitionOut(transition);
}

void FifthLevelFrame::_postHiding(Event *) {
	FlurryAnalytics::instance.onLevelLeaveEvent("level_5");
	slideFrame->removeEventListener(SlideFrame::GoBackToPreviousFrameEvent::GO_BACK, CLOSURE(this, &FifthLevelFrame::onGoBack));
	_view->removeChildren();
	_resources.unload();
}

void FifthLevelFrame::_preShowing(Event *) {
	FlurryAnalytics::instance.onLevelEnterEvent("level_5");
	selectTransitions();
	_resources.load();
	setData();
}

Action FifthLevelFrame::loop(){
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

void FifthLevelFrame::onGoBack(Event *event) {
	const string &name ="close";
	generateAction(name);
}

void FifthLevelFrame::onShowSliderFrame(Event *event) {
	const string &name ="slideFrame";
	generateAction(name);
}

void FifthLevelFrame::setData() {
	_wasTailSpreaded = false;
	spButton sliderButton = new Button();
	sliderButton->setResAnim(gameResources.getResAnim("back_button"));
	sliderButton->setScale(_content->getWidth() * 8 / 100 / sliderButton->getWidth());
	sliderButton->setPosition(Vector2(_content->getWidth() * 9 / 10 - 12, _content->getHeight() * 95 / 100 - sliderButton->getDerivedHeight()));

	sliderButton->setName("slideFrame");
	sliderButton->addEventListener(TouchEvent::TOUCH_DOWN, CLOSURE(this, &FifthLevelFrame::onShowSliderFrame));
	
	Array<Group*> *groupArray = new Array<Group*>();

	groupArray = SpriteSpawner::getLevelData(5);

	for(int i = 0; i < groupArray->length(); i++) {
		Group &g = *(*groupArray)[i];
		if(g[0] > 0) {
			addDraggableSprite(g.spriteName, Vector2(0.5f, 0.5f), g[0], g[1], g[2], g[3], g[4]);
		}
	}
	Particle::initialize("sparkle");
	spParticleEmitter particleEmitter = new ParticleEmitter(Vector2(_view->getWidth() * 1 / 10, _view->getWidth() * 9 / 10), Vector2(_view->getHeight() * 1 / 10, _view->getHeight() * 9 / 10), Vector2(-0.05, 0.05), Vector2(-0.05, 0.05), Vector2(3, 5), Vector2(4, 6));
	particleEmitter->setRadius(Vector2(_view->getHeight() / 600, _view->getHeight() / 550));
	particleEmitter->pushColor(0x003400, true);
	particleEmitter->pushColor(0xfffbe5);
	particleEmitter->pushColor(0x77b283);
	particleEmitter->attachTo(_view);
	sliderButton->attachTo(_view);
}

void FifthLevelFrame::addDraggableSprite(string spriteName, Vector2 anchorPoint, float scaleX, float scaleY, float positionX, float positionY, int priorityZ) {
	spSprite sprite = new Sprite;
	sprite->setResAnim(_resources.getResAnim(spriteName));
	sprite->setAnchor(anchorPoint);
	sprite->setScale(scaleX * getRoot()->getWidth() / sprite->getWidth(), scaleY * getRoot()->getHeight() / sprite->getHeight());
	sprite->setPosition(positionX * getRoot()->getWidth(), positionY * getRoot()->getHeight());
	sprite->setName(spriteName);
	sprite->setPriority(priorityZ);
	addTouchEventListenerIfNeeded(sprite);
	_view->addChild(sprite);
}

void FifthLevelFrame::addTouchEventListenerIfNeeded(spSprite sprite) {
	string spriteName = sprite->getName();
	string animatableObjectList[] =  {"yak", "tiger", "snake", "peacock", "panda", "monkey", "monkey_2", "parrot_2", "parrot", "mountain", "elephant"};
	int sizeOfStringArray = sizeof(animatableObjectList) / sizeof( animatableObjectList[0]);
	bool shouldMakeUntouchable = true;

	for (int i = 0; i < sizeOfStringArray; i++) {
		if (sprite->getName() == animatableObjectList[i]) {
			sprite->addEventListener(TouchEvent::CLICK, CLOSURE(this, &FifthLevelFrame::onAnimatableObjectTap));
			shouldMakeUntouchable = false;
		}
	}

	if (shouldMakeUntouchable) {
		sprite->setInputEnabled(false);
	}
}

void FifthLevelFrame::onAnimatableObjectTap(Event *event) {
	spActor actor = safeSpCast<Actor>(event->currentTarget);
	string name = event->currentTarget->getName();
	int duration = 1000;
	float scaleMultiplier = 1.2f;
	float scaleLowerer = 0.8f;

	if (!actor->getFirstTween()) {
		FlurryAnalytics::instance.onAnimatableObjectTapEvent("level_5", name.c_str());

		if (name == "yak") {
			actor->addTween(Actor::TweenRotation(actor->getRotation() + (float)MATH_PI / 4), duration / 4, 1, true, 0, Tween::ease_inExpo);
			actor->addTween(Actor::TweenRotation(actor->getRotation() - (float)MATH_PI / 4), duration / 4, 1, true, duration / 4, Tween::ease_inExpo);
			stopPreviousAndPlayNewSound(name);
		}
		else if (name == "tiger") {
			spActor monkey = _view->getChild("monkey_2");
			if (!monkey->getFirstTween()) {
				Vector2 kotelPosition = monkey->getPosition();
				monkey->addTween(Actor::TweenPosition(monkey->getPosition() + Vector2(0, -monkey->getDerivedHeight())), duration, 1, true);
				monkey->addTween(Actor::TweenRotation(monkey->getRotation() - (float)MATH_PI * 2), duration, 1, true);

				monkey = _view->getChild("monkey");
				if (!monkey->getFirstTween()) {
					monkey->addTween(Actor::TweenPosition(monkey->getPosition() + Vector2(0, -monkey->getDerivedHeight())), duration / 2, 1, true);
					monkey->addTween(Actor::TweenRotation(monkey->getRotation() + (float)MATH_PI * 2), duration / 2, 1, true);
				}

				actor->addTween(Actor::TweenPosition(kotelPosition), duration / 2, 1, true);
				actor->addTween(Actor::TweenScale(actor->getScale() * scaleMultiplier), duration / 2, 1, true);
			}
			else {
				actor->addTween(Actor::TweenPosition(actor->getPosition() + Vector2(0, -actor->getDerivedHeight())), duration / 2, 1, true);
				actor->addTween(Actor::TweenScale(actor->getScale() * scaleMultiplier), duration / 2, 1, true);
			}
			stopPreviousAndPlayNewSound(name);
		}
		else if (name == "snake") {
			actor->addTween(Actor::TweenRotation(actor->getRotation() + (float)MATH_PI * 2), duration, 1);
			stopPreviousAndPlayNewSound(name);
		}
		else if (name == "peacock") {
			actor->addTween(Actor::TweenPosition(actor->getPosition() + Vector2(0, -actor->getDerivedHeight())), duration / 2, 1, true);
			actor->addTween(Actor::TweenScale(actor->getScale() * scaleMultiplier), duration / 2, 1, true);
			
			spSprite spriteActor = safeSpCast<Sprite>(event->currentTarget);
			if (_wasTailSpreaded) {
				spriteActor->setAnimFrame(_resources.getResAnim("peacock"), 0, 0);
				_wasTailSpreaded = false;
			}
			else {
				spriteActor->setAnimFrame(_resources.getResAnim("peacock"), 1, 0);
				_wasTailSpreaded = true;
			}
			stopPreviousAndPlayNewSound(name);
		}
		else if (name == "panda") {
			actor->addTween(Actor::TweenScale(actor->getScale() * scaleMultiplier), duration / 2, 1, true);
			actor->addTween(Actor::TweenPosition(actor->getPosition() + Vector2(actor->getDerivedWidth(), 0)), duration / 2);
			actor->addTween(Actor::TweenPosition(actor->getPosition()), duration / 2, 1, false, duration * 2);
			stopPreviousAndPlayNewSound(name);
		}
		else if (name == "monkey") {
			actor->addTween(Actor::TweenPosition(actor->getPosition() + Vector2(0, -actor->getDerivedHeight())), duration / 2, 1, true);
			actor->addTween(Actor::TweenRotation(actor->getRotation() + (float)MATH_PI * 2), duration / 2, 1, true);
			stopPreviousAndPlayNewSound(name);
		}
		else if (name == "monkey_2") {
			actor->addTween(Actor::TweenPosition(actor->getPosition() + Vector2(0, -actor->getDerivedHeight())), duration, 1, true);
			actor->addTween(Actor::TweenRotation(actor->getRotation() - (float)MATH_PI * 2), duration, 1, true);
			stopPreviousAndPlayNewSound("monkey");
		}
		else if (name == "parrot_2") {
			spActor anotherParrot = _view->getChild("parrot");
			anotherParrot->addTween(Actor::TweenPosition(anotherParrot->getPosition() + Vector2(0, -anotherParrot->getDerivedHeight())), duration / 2, 1, true,  duration / 2);
			anotherParrot->addTween(Actor::TweenScale(anotherParrot->getScale() * scaleLowerer), duration / 2, 1, true,  duration / 2);
			actor->addTween(Actor::TweenPosition(actor->getPosition() + Vector2(0, -actor->getDerivedHeight())), duration / 2, 1, true);
			actor->addTween(Actor::TweenScale(actor->getScale() * scaleLowerer), duration / 2, 1, true);
			stopPreviousAndPlayNewSound("parrot");
		}
		else if (name == "parrot") {
			spActor anotherParrot = _view->getChild("parrot_2");
			anotherParrot->addTween(Actor::TweenPosition(anotherParrot->getPosition() + Vector2(0, -anotherParrot->getDerivedHeight())), duration / 2, 1, true,  duration / 2);
			anotherParrot->addTween(Actor::TweenScale(anotherParrot->getScale() * scaleLowerer), duration / 2, 1, true,  duration / 2);
			actor->addTween(Actor::TweenPosition(actor->getPosition() + Vector2(0, -actor->getDerivedHeight())), duration / 2, 1, true);
			actor->addTween(Actor::TweenScale(actor->getScale() * scaleLowerer), duration / 2, 1, true);
			stopPreviousAndPlayNewSound(name);
		}
		else if (name == "mountain") {
			actor->addTween(Actor::TweenScale(actor->getScale() * scaleLowerer), duration / 2, 1, true);
		}
		else if (name == "elephant") {
			actor->addTween(Actor::TweenRotation(actor->getRotation() + (float)MATH_PI / 4), duration / 4, 1, true, 0, Tween::ease_inBack);
			actor->addTween(Actor::TweenRotation(actor->getRotation() - (float)MATH_PI / 4), duration / 4, 1, true, duration / 4, Tween::ease_inBack);
			actor->addTween(Actor::TweenPosition(actor->getPosition() + Vector2(actor->getDerivedWidth(), 0)), duration, 1, true);
			stopPreviousAndPlayNewSound(name);
		}
	}
}

void FifthLevelFrame::stopPreviousAndPlayNewSound(string soundName) {
	if (_previousSoundInstance) {
		_previousSoundInstance->stop();
	}
	_previousSoundInstance = sPlayer.play(soundName + "_sound");
}