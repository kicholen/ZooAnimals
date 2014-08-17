#include "ThridLevelFrame.h"
#include "FlurryAnalytics.h"
#include "SoundPlayer.h"
#include "SoundInstance.h"
#include "ResSound.h"

ThridLevelFrame::ThridLevelFrame() {
	init("ThirdLevelFrame.xml", true);
	slideFrame->addEventListener(SlideFrame::GoBackToPreviousFrameEvent::GO_BACK, CLOSURE(this, &ThridLevelFrame::onGoBack));

	selectTransitions();
}

void ThridLevelFrame::selectTransitions() {
	spTransition transition = new TransitionQuads;
	setTransitionIn(transition);
	setTransitionOut(transition);
}

void ThridLevelFrame::_postHiding(Event *) {
	FlurryAnalytics::instance.onLevelLeaveEvent("level_3");
	slideFrame->removeEventListener(SlideFrame::GoBackToPreviousFrameEvent::GO_BACK, CLOSURE(this, &ThridLevelFrame::onGoBack));
	_view->removeChildren();
	_resources.unload();
}

void ThridLevelFrame::_preShowing(Event *) {
	FlurryAnalytics::instance.onLevelEnterEvent("level_3");
	selectTransitions();
	_resources.load();
	setData();
}

Action ThridLevelFrame::loop() {
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

void ThridLevelFrame::onGoBack(Event *event) {
	const string &name ="close";
	generateAction(name);
}

void ThridLevelFrame::onShowSliderFrame(Event *event) {
	const string &name ="slideFrame";
	generateAction(name);
}

void ThridLevelFrame::setData() {
	spButton sliderButton = new Button();
	sliderButton->setResAnim(gameResources.getResAnim("back_button"));
	sliderButton->setScale(_content->getWidth() * 8 / 100 / sliderButton->getWidth());
	sliderButton->setPosition(Vector2(_content->getWidth() * 9 / 10 - 12, _content->getHeight() * 95 / 100 - sliderButton->getDerivedHeight()));

	sliderButton->setName("slideFrame");
	sliderButton->addEventListener(TouchEvent::TOUCH_DOWN, CLOSURE(this, &ThridLevelFrame::onShowSliderFrame));
	
	Array<Group*> *groupArray = new Array<Group*>();

	groupArray = SpriteSpawner::getLevelData(3);

	for(int i = 0; i < groupArray->length(); i++) {
		Group &g = *(*groupArray)[i];
		if(g[0] > 0) {
			addDraggableSprite(g.spriteName, Vector2(0.5f, 0.5f), g[0], g[1], g[2], g[3], g[4]);
		}
	}
	sliderButton->attachTo(_view);
}

void ThridLevelFrame::addDraggableSprite(string spriteName, Vector2 anchorPoint, float scaleX, float scaleY, float positionX, float positionY, int priorityZ) {
	spSprite sprite = new Sprite;
	sprite->setResAnim(_resources.getResAnim(spriteName));
	sprite->setAnchor(anchorPoint);
	sprite->setScale(scaleX * getRoot()->getWidth() / sprite->getWidth(), scaleY * getRoot()->getHeight() / sprite->getHeight());
	sprite->setPosition(positionX * getRoot()->getWidth(), positionY * getRoot()->getHeight());
	sprite->setName(spriteName);
	if (priorityZ != 0) {
		int rzadDupa = 0;
	}
	sprite->setPriority(priorityZ);
	addTouchEventListenerIfNeeded(sprite);
	_view->addChild(sprite);
}

void ThridLevelFrame::addTouchEventListenerIfNeeded(spSprite sprite) {
	string spriteName = sprite->getName();
	string animatableObjectList[] =  {"elephant", "girafee", "gnu", "leopard", "rhino", "zebra", "lion"};
	int sizeOfStringArray = sizeof(animatableObjectList) / sizeof( animatableObjectList[0]);
	bool shouldMakeUntouchable = true;

	for (int i = 0; i < sizeOfStringArray; i++) {
		if (sprite->getName() == animatableObjectList[i]) {
			sprite->addEventListener(TouchEvent::CLICK, CLOSURE(this, &ThridLevelFrame::onAnimatableObjectTap));
			shouldMakeUntouchable = false;
		}
	}

	if (shouldMakeUntouchable) {
		sprite->setInputEnabled(false);
	}
}

void ThridLevelFrame::onAnimatableObjectTap(Event *event) {
	spActor actor = safeSpCast<Actor>(event->currentTarget);
	string name = event->currentTarget->getName();
	int duration = 1000;
	float scaleMultiplier = 1.2f;

	if (!actor->getFirstTween()) {
		FlurryAnalytics::instance.onAnimatableObjectTapEvent("level_3", name.c_str());

		if (name == "elephant") {
			actor->addTween(Actor::TweenScaleX(actor->getScaleX() * scaleMultiplier), duration / 2, 1, true);
			stopPreviousAndPlayNewSound(name);
		}
		else if (name == "girafee") {
			actor->addTween(Actor::TweenScaleY(actor->getScaleY() * scaleMultiplier), duration / 2, 1, true);
			stopPreviousAndPlayNewSound("giraffe");
		}
		else if (name == "gnu") {
			spActor gnuActor = _view->getFirstChild();
			int countGnu = 0;
			while (gnuActor) {
				if (gnuActor->getName() == "gnu" && !gnuActor->getFirstTween()) {
					countGnu += 1;
					if (countGnu < 2) {
						gnuActor->addTween(Actor::TweenRotation(gnuActor->getRotation() + (float)MATH_PI / 4), duration / 2, 1, true, 0, Tween::ease_inBack);
						gnuActor->addTween(Actor::TweenRotation(gnuActor->getRotation() - (float)MATH_PI / 4), duration / 2, 1, true, duration / 2, Tween::ease_inBack);
					}
					else {
						gnuActor->addTween(Actor::TweenRotation(gnuActor->getRotation() + (float)MATH_PI / 4), duration / 2, 1, true, duration, Tween::ease_inBack);
						gnuActor->addTween(Actor::TweenRotation(gnuActor->getRotation() - (float)MATH_PI / 4), duration / 2, 1, true, duration * 3 / 2, Tween::ease_inBack);
					}
				}
				gnuActor = gnuActor->getNextSibling();
			}
			stopPreviousAndPlayNewSound(name);
		}
		else if (name == "leopard") {
			spActor zebraActor = _view->getFirstChild();
			Vector2 zebraPosition;
			bool foundZebra = false;
			while (zebraActor) {
				if (zebraActor->getName() == "zebra" && !zebraActor->getFirstTween()) {
					zebraPosition = zebraActor->getPosition();
					foundZebra = true;

					zebraActor->addTween(Actor::TweenRotation(actor->getRotation() + (float)MATH_PI / 4), duration / 4, 1, true, 0, Tween::ease_inBack);
					zebraActor->addTween(Actor::TweenRotation(actor->getRotation() - (float)MATH_PI / 4), duration / 4, 1, true, duration / 4, Tween::ease_inBack);
					zebraActor->addTween(Actor::TweenX(zebraActor->getX() + - zebraActor->getDerivedWidth()), duration / 3, 1, false);
					zebraActor->addTween(Actor::TweenY(zebraActor->getY() + zebraActor->getDerivedHeight()), duration / 4, 1, false);
					zebraActor->addTween(Actor::TweenX(zebraActor->getX()), duration / 3, 1, false, 3 * duration / 2);
					zebraActor->addTween(Actor::TweenY(zebraActor->getY()), duration / 4, 1, false, 3 * duration / 2);
				}
				zebraActor = zebraActor->getNextSibling();
			}
			if (foundZebra) {
				actor->addTween(Actor::TweenPosition(zebraPosition), duration / 2, 1, false);
				actor->addTween(Actor::TweenPosition(actor->getPosition()), duration / 2, 1, false, duration * 3 / 4);
				actor->addTween(Actor::TweenScale(actor->getScale() * scaleMultiplier), duration / 2, 1, true);
			}
			else {
				actor->addTween(Actor::TweenPosition(actor->getPosition() + Vector2(0, -actor->getDerivedHeight())), duration / 2, 1, true);
			}
			stopPreviousAndPlayNewSound(name);
		}
		else if (name == "rhino") {
			actor->addTween(Actor::TweenPosition(actor->getPosition() + Vector2(-actor->getDerivedWidth(), 0)), duration, 1, true);
			actor->addTween(Actor::TweenScale(actor->getScale() * scaleMultiplier), duration, 1, true);
			stopPreviousAndPlayNewSound(name);
		}
		else if (name == "zebra") {
			spActor zebraActor = _view->getFirstChild();
			while (zebraActor) {
				if (zebraActor->getName() == "zebra" && !zebraActor->getFirstTween()) {
					zebraActor->addTween(Actor::TweenRotation(zebraActor->getRotation() + (float)MATH_PI / 4), duration / 4, 1, true, 0, Tween::ease_inBack);
					zebraActor->addTween(Actor::TweenRotation(zebraActor->getRotation() - (float)MATH_PI / 4), duration / 4, 1, true, duration / 4, Tween::ease_inBack);
				}
				zebraActor = zebraActor->getNextSibling();
			}
			stopPreviousAndPlayNewSound(name);
		}
		else if (name == "lion") {
			actor->addTween(Actor::TweenScale(actor->getScale() * scaleMultiplier), duration / 2, 1, true);
			stopPreviousAndPlayNewSound(name);
		}
	}
}

void ThridLevelFrame::stopPreviousAndPlayNewSound(string soundName) {
	if (_previousSoundInstance) {
		_previousSoundInstance->stop();
	}
	_previousSoundInstance = sPlayer.play(soundName + "_sound");
}