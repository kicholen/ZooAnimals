#include "SixthLevelFrame.h"
#include "FlurryAnalytics.h"
#include "SoundPlayer.h"
#include "SoundInstance.h"
#include "ResSound.h"

SixthLevelFrame::SixthLevelFrame() {
	init("SixthLevelFrame.xml", true);
	slideFrame->addEventListener(SlideFrame::GoBackToPreviousFrameEvent::GO_BACK, CLOSURE(this, &SixthLevelFrame::onGoBack));

	selectTransitions();
}

void SixthLevelFrame::selectTransitions() {
	spTransition transition = new TransitionQuads;
	setTransitionIn(transition);
	setTransitionOut(transition);
}

void SixthLevelFrame::_postHiding(Event *) {
	FlurryAnalytics::instance.onLevelLeaveEvent("level_6");
	slideFrame->removeEventListener(SlideFrame::GoBackToPreviousFrameEvent::GO_BACK, CLOSURE(this, &SixthLevelFrame::onGoBack));
	_view->removeChildren();
	_resources.unload();
}

void SixthLevelFrame::_preShowing(Event *) {
	FlurryAnalytics::instance.onLevelEnterEvent("level_6");
	selectTransitions();
	_resources.load();
	setData();
}

Action SixthLevelFrame::loop() {
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

void SixthLevelFrame::onGoBack(Event *event) {
	const string &name ="close";
	generateAction(name);
}

void SixthLevelFrame::onShowSliderFrame(Event *event) {
	const string &name ="slideFrame";
	generateAction(name);
}

void SixthLevelFrame::setData() {
	spButton sliderButton = new Button();
	sliderButton->setResAnim(gameResources.getResAnim("back_button"));
	sliderButton->setScale(_content->getWidth() * 8 / 100 / sliderButton->getWidth());
	sliderButton->setPosition(Vector2(_content->getWidth() * 9 / 10 - 12, _content->getHeight() * 95 / 100 - sliderButton->getDerivedHeight()));

	sliderButton->setName("slideFrame");
	sliderButton->addEventListener(TouchEvent::TOUCH_DOWN, CLOSURE(this, &SixthLevelFrame::onShowSliderFrame));
	
	Array<Group*> *groupArray = new Array<Group*>();

	groupArray = SpriteSpawner::getLevelData(6);

	for(int i = 0; i < groupArray->length(); i++) {
		Group &g = *(*groupArray)[i];
		if(g[0] > 0) {
			addDraggableSprite(g.spriteName, Vector2(0.5f, 0.5f), g[0], g[1], g[2], g[3], g[4]);
		}
	}
	sliderButton->attachTo(_view);
}

void SixthLevelFrame::addDraggableSprite(string spriteName, Vector2 anchorPoint, float scaleX, float scaleY, float positionX, float positionY, int priorityZ) {
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

void SixthLevelFrame::addTouchEventListenerIfNeeded(spSprite sprite) {
	string spriteName = sprite->getName();
	string animatableObjectList[] =  {"crocodile", "kangaroo", "kiwi", "koala", "ostrich", "tealplatypus", "turtle", "dingo", "tree", "cactus", "sun"};
	int sizeOfStringArray = sizeof(animatableObjectList) / sizeof( animatableObjectList[0]);
	bool shouldMakeUntouchable = true;

	for (int i = 0; i < sizeOfStringArray; i++) {
		if (sprite->getName() == animatableObjectList[i]) {
			sprite->addEventListener(TouchEvent::CLICK, CLOSURE(this, &SixthLevelFrame::onAnimatableObjectTap));
			shouldMakeUntouchable = false;
		}
	}

	if (shouldMakeUntouchable) {
		sprite->setInputEnabled(false);
	}
}

void SixthLevelFrame::onAnimatableObjectTap(Event *event) {
	spActor actor = safeSpCast<Actor>(event->currentTarget);
	string name = event->currentTarget->getName();
	int duration = 1000;
	float scaleMultiplier = 1.2f;
	float scaleLowerer = 0.8f;

	if (!actor->getFirstTween()) {
		FlurryAnalytics::instance.onAnimatableObjectTapEvent("level_6", name.c_str());

		if (name == "crocodile") {
			actor->addTween(Actor::TweenScaleX(-actor->getScaleX()), duration, 1, true);
			stopPreviousAndPlayNewSound(name);
		}
		else if (name == "kangaroo") {
			actor->addTween(Actor::TweenRotation(actor->getRotation() + (float)MATH_PI / 4), duration / 4, 1, true, 0, Tween::ease_inBack);
			actor->addTween(Actor::TweenRotation(actor->getRotation() - (float)MATH_PI / 4), duration / 4, 1, true, duration / 4, Tween::ease_inBack);
			actor->addTween(Actor::TweenPosition(actor->getPosition() + Vector2(actor->getDerivedWidth(), 0)), duration, 1, true);
			stopPreviousAndPlayNewSound(name);
		}
		else if (name == "kiwi") {
			actor->addTween(Actor::TweenRotation(actor->getRotation() + (float)MATH_PI / 4), duration / 4, 1, true, 0, Tween::ease_inBack);
			actor->addTween(Actor::TweenRotation(actor->getRotation() - (float)MATH_PI / 4), duration / 4, 1, true, duration / 4, Tween::ease_inBack);
			stopPreviousAndPlayNewSound(name);
		}
		else if (name == "koala") {
			actor->addTween(Actor::TweenScale(actor->getScale() * scaleMultiplier), duration / 2, 1, true);
			actor->addTween(Actor::TweenPosition(actor->getPosition() + Vector2(actor->getDerivedWidth(), 0)), duration / 2, 1, true);
			stopPreviousAndPlayNewSound(name);
		}
		else if (name == "ostrich") {
			actor->addTween(Actor::TweenPosition(actor->getPosition() + Vector2(0, -actor->getDerivedHeight())), duration / 2, 1, true);
			actor->addTween(Actor::TweenScale(actor->getScale() * scaleMultiplier), duration / 2, 1, true);
			stopPreviousAndPlayNewSound("emu");
		}
		else if (name == "tealplatypus") {
			actor->addTween(Actor::TweenRotation(actor->getRotation() + (float)MATH_PI / 4), duration / 4, 1, true, 0, Tween::ease_inBack);
			actor->addTween(Actor::TweenRotation(actor->getRotation() - (float)MATH_PI / 4), duration / 4, 1, true, duration / 4, Tween::ease_inBack);
			stopPreviousAndPlayNewSound("platypus");
		}
		else if (name == "turtle") {
			actor->addTween(Actor::TweenScale(actor->getScale() * scaleLowerer), duration / 2, 1, true);
			actor->addTween(Sprite::TweenColor(Color(19, 109, 186, 180)), duration / 2, 1, true);
			stopPreviousAndPlayNewSound(name);
		}
		else if (name == "dingo") {
			actor->addTween(Actor::TweenPosition(actor->getPosition() + Vector2(0, -actor->getDerivedHeight())), duration / 2, 1, true);
			actor->addTween(Actor::TweenScale(actor->getScale() * scaleMultiplier), duration / 2, 1, true);
			stopPreviousAndPlayNewSound(name);
		}
		else if (name == "cactus") {
			actor->addTween(Actor::TweenScaleY(actor->getScaleY() * scaleMultiplier), duration / 2, 1, true, 0);
		}
		else if (name == "sun") {
			actor->addTween(Actor::TweenRotation(actor->getRotation() + (float)MATH_PI / 4), duration / 4, 1, true, 0, Tween::ease_inBack);
			actor->addTween(Actor::TweenScale(actor->getScale() * scaleMultiplier), duration / 4, 1, true);
		}
	}
}

void SixthLevelFrame::stopPreviousAndPlayNewSound(string soundName) {
	if (_previousSoundInstance) {
		_previousSoundInstance->stop();
	}
	_previousSoundInstance = sPlayer.play(soundName + "_sound");
}