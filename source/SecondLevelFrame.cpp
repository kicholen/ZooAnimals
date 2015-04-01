#include "SecondLevelFrame.h"
#include "FlurryAnalytics.h"
#include "SoundPlayer.h"
#include "SoundInstance.h"
#include "ResSound.h"

SecondLevelFrame::SecondLevelFrame() {
	init("SecondLevelFrame.xml", true);
	slideFrame->addEventListener(SlideFrame::GoBackToPreviousFrameEvent::GO_BACK, CLOSURE(this, &SecondLevelFrame::onGoBack));

	selectTransitions();
}

void SecondLevelFrame::selectTransitions() {
	spTransition transition = new TransitionQuads;
	setTransitionIn(transition);
	setTransitionOut(transition);
}

void SecondLevelFrame::_postHiding(Event *) {
	FlurryAnalytics::instance.onLevelLeaveEvent("level_2");
	slideFrame->removeEventListener(SlideFrame::GoBackToPreviousFrameEvent::GO_BACK, CLOSURE(this, &SecondLevelFrame::onGoBack));
	_view->removeChildren();
	_resources.unload();
}

void SecondLevelFrame::_preShowing(Event *) {
	FlurryAnalytics::instance.onLevelEnterEvent("level_2");
	selectTransitions();
	_resources.load();
	setData();
}

Action SecondLevelFrame::loop() {
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

void SecondLevelFrame::onGoBack(Event *event) {
	const string &name ="close";
	generateAction(name);
}

void SecondLevelFrame::onShowSliderFrame(Event *event) {
	const string &name ="slideFrame";
	generateAction(name);
}

void SecondLevelFrame::setData() {
	spButton sliderButton = new Button();
	sliderButton->setResAnim(gameResources.getResAnim("back_button"));
	sliderButton->setScale(_content->getWidth() * 8 / 100 / sliderButton->getWidth());
	sliderButton->setPosition(Vector2(_content->getWidth() * 9 / 10 - 12, _content->getHeight() * 95 / 100 - sliderButton->getDerivedHeight()));

	sliderButton->setName("slideFrame");
	sliderButton->addEventListener(TouchEvent::TOUCH_DOWN, CLOSURE(this, &SecondLevelFrame::onShowSliderFrame));
	
	Array<Group*> *groupArray = new Array<Group*>();

	groupArray = SpriteSpawner::getLevelData(2);

	for(int i = 0; i < groupArray->length(); i++) {
		Group &g = *(*groupArray)[i];
		if(g[0] > 0) {
			addDraggableSprite(g.spriteName, Vector2(0.5f, 0.5f), g[0], g[1], g[2], g[3], g[4]);
		}
	}

	Particle::initialize("snow");
	spParticleEmitter particleEmitter = new ParticleEmitter(Vector2(_view->getWidth() * 2 / 10, _view->getWidth() * 8 / 10), Vector2(0.0f, 0.0f), Vector2(-0.01f, 0.01f), Vector2(0.045f, 0.06f), Vector2(4, 12), Vector2(2, 5));
	particleEmitter->setRadius(Vector2(_view->getHeight() / 400, _view->getHeight() / 340));
	particleEmitter->attachTo(_view);

	sliderButton->attachTo(_view);
}

void SecondLevelFrame::addDraggableSprite(string spriteName, Vector2 anchorPoint, float scaleX, float scaleY, float positionX, float positionY, int priorityZ) {
	spSprite sprite = new Sprite;
	sprite->setResAnim(_resources.getResAnim(spriteName));
	sprite->setAnchor(anchorPoint);
	sprite->setScale(scaleX * getRoot()->getWidth() / sprite->getWidth(), scaleY * getRoot()->getHeight() / sprite->getHeight());
	sprite->setPosition(positionX * getRoot()->getWidth(), positionY * getRoot()->getHeight());
	sprite->setName(spriteName);
	if (priorityZ < 0) {
		int rzadDupa = 0;
	}
	sprite->setPriority(priorityZ);
	addTouchEventListenerIfNeeded(sprite);
	_view->addChild(sprite);
}

void SecondLevelFrame::addTouchEventListenerIfNeeded(spSprite sprite) {
	string spriteName = sprite->getName();
	string animatableObjectList[] =  {"fox", "penguin", "penguin_2", "polarbear", "polarbear_fish", "walrus", "wolf", "young_seal", "owl_winter", "hole", "seal_from_hole", "top_hole", "winter_tree", "igloo", "bottom_hole_seal_out", "top_hole_seal_out"};
	int sizeOfStringArray = sizeof(animatableObjectList) / sizeof( animatableObjectList[0]);
	bool shouldMakeUntouchable = true;

	for (int i = 0; i < sizeOfStringArray; i++) {
		if (sprite->getName() == animatableObjectList[i]) {
			sprite->addEventListener(TouchEvent::CLICK, CLOSURE(this, &SecondLevelFrame::onAnimatableObjectTap));
			shouldMakeUntouchable = false;
		}
	}

	if (shouldMakeUntouchable) {
		sprite->setInputEnabled(false);
	}
}

void SecondLevelFrame::onAnimatableObjectTap(Event *event) {
	spActor actor = safeSpCast<Actor>(event->currentTarget);
	string name = event->currentTarget->getName();
	int duration = 1000;
	float scaleMultiplier = 1.2f;

	if (!actor->getFirstTween()) {
		FlurryAnalytics::instance.onAnimatableObjectTapEvent("level_2", name.c_str());

		if (name == "fox") {
			actor->addTween(Actor::TweenScale(actor->getScale() * scaleMultiplier), duration, 1, true);
			stopPreviousAndPlayNewSound(name);
		}
		else if (name == "penguin") {
			spActor penguinActor = _view->getFirstChild();
			while (penguinActor) {
				if (penguinActor->getName() == "penguin" && !penguinActor->getFirstTween()) {
					penguinActor->addTween(Actor::TweenRotation(penguinActor->getRotation() + (float)MATH_PI / 4), duration / 4, 1, true, 0, Tween::ease_inBack);
					penguinActor->addTween(Actor::TweenRotation(penguinActor->getRotation() - (float)MATH_PI / 4), duration / 4, 1, true, duration / 4, Tween::ease_inBack);
				}
				penguinActor = penguinActor->getNextSibling();
			}
			stopPreviousAndPlayNewSound(name);
		}
		else if (name == "penguin_2") {
			spActor penguinActor = _view->getFirstChild();
			while (penguinActor) {
				if (penguinActor->getName() == "penguin_2" && !penguinActor->getFirstTween()) {
					penguinActor->addTween(Actor::TweenRotation(penguinActor->getRotation() + (float)MATH_PI / 4), duration / 2, 1, true, 0, Tween::ease_inBack);
					penguinActor->addTween(Actor::TweenRotation(penguinActor->getRotation() - (float)MATH_PI / 4), duration / 2, 1, true, duration / 2, Tween::ease_inBack);
				}
				penguinActor = penguinActor->getNextSibling();
			}
			stopPreviousAndPlayNewSound("penguin");
		}
		else if (name == "polarbear") {
			actor->addTween(Actor::TweenScale(actor->getScale() * scaleMultiplier), duration / 2, 1, true);
			stopPreviousAndPlayNewSound(name);
		}
		else if (name == "polarbear_fish") {
			actor->addTween(Actor::TweenScale(actor->getScale() * scaleMultiplier), duration / 2, 1, true);
			actor->addTween(Actor::TweenRotation(actor->getRotation() + (float)MATH_PI / 4), duration / 2, 1, true, 0, Tween::ease_inBack);
			actor->addTween(Actor::TweenRotation(actor->getRotation() - (float)MATH_PI / 4), duration / 2, 1, true, duration / 2, Tween::ease_inBack);
			stopPreviousAndPlayNewSound("polarbear");
		}
		else if (name == "walrus") {
			actor->addTween(Actor::TweenRotation(actor->getRotation() + (float)MATH_PI / 4), duration / 4, 1, true, 0, Tween::ease_inBack);
			actor->addTween(Actor::TweenRotation(actor->getRotation() - (float)MATH_PI / 4), duration / 4, 1, true, duration / 4, Tween::ease_inBack);
			stopPreviousAndPlayNewSound(name);
		}
		else if (name == "wolf") {
			actor->addTween(Actor::TweenScale(actor->getScale() * scaleMultiplier), duration / 2, 1, true);
			stopPreviousAndPlayNewSound(name);
		}
		else if (name == "young_seal") {
			spActor sealActor = _view->getFirstChild();
			float minLeft = 0.0f;
			float maxRight = _view->getWidth();
			
			int bottomHoleZPriority = 0;
			int previousSealPriority = 0;
			Vector2 holePosition;
			while (sealActor) {
				if (sealActor->getName() == "young_seal") {
					
					if (sealActor->getX() < maxRight) {
						maxRight = sealActor->getX(); 
					}
					if (sealActor->getX() > minLeft) {
						minLeft = sealActor->getX();
					}
				}
				else if (sealActor->getName() == "top_hole_right") {
					sealActor->getPriority();
				}
				else if (sealActor->getName() == "bottom_hole_right") {
					bottomHoleZPriority = sealActor->getPriority();
					holePosition = sealActor->getPosition();
				}
				sealActor = sealActor->getNextSibling();
			}

			sealActor = _view->getFirstChild();
			while (sealActor) {
				if (sealActor->getName() == "young_seal") {
					if (sealActor->getX() == minLeft && !sealActor->getFirstTween()) {
						sealActor->addTween(Actor::TweenPosition(sealActor->getPosition() + Vector2(-sealActor->getDerivedWidth() / 6, sealActor->getDerivedHeight() / 3)), duration, 1, true);
					}
					else if (sealActor->getX() == maxRight && !sealActor->getFirstTween()) {
						spTween firstTween = createTween(Actor::TweenPosition(holePosition + Vector2(0, -sealActor->getDerivedHeight())), duration / 2);
						sealActor->addTween(firstTween);
						firstTween->addEventListener(TweenEvent::DONE, CLOSURE(this, &SecondLevelFrame::onTweenEndPriorityDown));
						sealActor->addTween(Actor::TweenPosition(holePosition + Vector2(0, -sealActor->getDerivedHeight() / 2)), duration / 2, 1, false, duration / 2, Tween::ease_inOutBack);
						spTween secondTween = createTween(Actor::TweenPosition(holePosition + Vector2(0, -sealActor->getDerivedHeight())), duration / 2, 1, false, duration);
						sealActor->addTween(secondTween);
						secondTween->addEventListener(TweenEvent::DONE, CLOSURE(this, &SecondLevelFrame::onTweenEndPriorityUp));
						sealActor->addTween(Actor::TweenPosition(sealActor->getPosition()), duration / 2, 1, false, duration * 3 / 2);
					}
					else {
						sealActor->addTween(Actor::TweenScale(sealActor->getScale() * scaleMultiplier), duration, 1, true);
					}
				}

				sealActor = sealActor->getNextSibling();
			}
			stopPreviousAndPlayNewSound("seal");
		}
		else if (name == "seal_from_hole" || name == "top_hole_seal_out" || name == "bottom_hole_seal_out") {
			if (name == "seal_from_hole") {
				actor->addTween(Actor::TweenPosition(actor->getPosition() + Vector2(0, -actor->getDerivedHeight() / 2)), duration / 2);
				actor->addTween(Actor::TweenPosition(actor->getPosition()), duration / 2, 1, false, duration * 3 / 2);
			}
			else {
				spActor sealActor = _view->getFirstChild();
				while (sealActor) {
					if (sealActor->getName() == "seal_from_hole" && !sealActor->getFirstTween()) {
						sealActor->addTween(Actor::TweenPosition(sealActor->getPosition() + Vector2(0, -sealActor->getDerivedHeight() / 2)), duration / 2);
						sealActor->addTween(Actor::TweenPosition(sealActor->getPosition()), duration / 2, 1, false, duration * 3 / 2);
					}
					sealActor = sealActor->getNextSibling();
				}
			}
			stopPreviousAndPlayNewSound("seal");
		}
		else if (name == "owl_winter") {
			actor->addTween(Actor::TweenPosition(actor->getPosition() + Vector2(0, -actor->getDerivedHeight())), duration, 1, true);
			stopPreviousAndPlayNewSound(name);
		}
		else if (name == "winter_tree") {
			actor->addTween(Actor::TweenScale(actor->getScale() * scaleMultiplier), duration, 1, true);
		}
		else if (name == "igloo") {
			actor->addTween(Actor::TweenScale(actor->getScale() * scaleMultiplier), duration, 1, true);
		}
		//actor->addTween(Sprite::TweenColor(Color(255,0,0,255)), 300, -1, true);
	}
	//int new_priority = actor->getParent()->getLastChild()->getPriority() + 1;
	//actor->setPriority(new_priority);

}

void SecondLevelFrame::onTweenEndPriorityDown(Event *event) {
	spTween tween= safeSpCast<Tween>(event->currentTarget);
	spActor tweenedActor = tween->getClient();
	tweenedActor->setPriority(tweenedActor->getPriority() - 2);
}

void SecondLevelFrame::onTweenEndPriorityUp(Event *event) {
	spTween tween= safeSpCast<Tween>(event->currentTarget);
	spActor tweenedActor = tween->getClient();
	tweenedActor->setPriority(tweenedActor->getPriority() + 2);
}

void SecondLevelFrame::stopPreviousAndPlayNewSound(string soundName) {
	if (_previousSoundInstance) {
		_previousSoundInstance->stop();
	}
	_previousSoundInstance = sPlayer.play(soundName + "_sound");
}