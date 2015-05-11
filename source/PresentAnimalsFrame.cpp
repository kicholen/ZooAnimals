#include "PresentAnimalsFrame.h"
#include "FlurryAnalytics.h"
#include "SoundPlayer.h"
#include "SoundInstance.h"
#include "ResSound.h"

PresentAnimalsFrame::PresentAnimalsFrame(const char* whichLevel) {
	init(whichLevel, true);
	_whichLevel = whichLevel;
//	slideFrame->addEventListener(SlideFrame::GoBackToPreviousFrameEvent::GO_BACK, CLOSURE(this, &PresentAnimalsFrame::onGoBack));

	selectTransitions();
}

void PresentAnimalsFrame::selectTransitions() {
	spTransition transition = new TransitionQuads;
	setTransitionIn(transition);
	setTransitionOut(transition);
}

void PresentAnimalsFrame::_postHiding(Event *) {
	FlurryAnalytics::instance.onLevelLeaveEvent(_whichLevel.c_str());
//	slideFrame->removeEventListener(SlideFrame::GoBackToPreviousFrameEvent::GO_BACK, CLOSURE(this, &PresentAnimalsFrame::onGoBack));
	_view->removeChildren();
	_resources.unload();

	_whichLevel.clear();
}

void PresentAnimalsFrame::_preShowing(Event *) {
	FlurryAnalytics::instance.onLevelEnterEvent(_whichLevel.c_str());
	selectTransitions();
	_resources.load();
	setData();
}

Action PresentAnimalsFrame::loop() {
	while (1) {
		Action action = waitAction();
		if (action.id == "slideFrame" || action.id == "_btn_back_") {
//			transitionShowFrameAsDialog(slideFrame);
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

void PresentAnimalsFrame::onGoBack(Event *event) {
	const string &name ="close";
	generateAction(name);
}

void PresentAnimalsFrame::onShowSliderFrame(Event *event) {
	const string &name ="slideFrame";
	generateAction(name);
}

void PresentAnimalsFrame::setData() {
	spButton sliderButton = new Button();
	sliderButton->setResAnim(gameResources.getResAnim("back_button"));
	sliderButton->setScale(_content->getWidth() * 8 / 100 / sliderButton->getWidth());
	sliderButton->setPosition(Vector2(_content->getWidth() * 9 / 10 - 12, _content->getHeight() * 95 / 100 - sliderButton->getDerivedHeight()));

	sliderButton->setName("slideFrame");
	sliderButton->addEventListener(TouchEvent::TOUCH_DOWN, CLOSURE(this, &PresentAnimalsFrame::onShowSliderFrame));
	
	VectorArray<Group*> *groupArray = new VectorArray<Group*>();

	int levelToLoad = getLevelToLoad();

	groupArray = SpriteSpawner::getLevelData(1);
	// we move position for every next animal whole screen to right, for every left animal, whole screen to left
	// also we block swipe until previous one ends
	//for(int i = 0; i < groupArray->length(); i++) {
	//	Group &g = *(*groupArray)[i];
	//	if(g[0] > 0) {
	//		addDraggableSprite(g.spriteName, Vector2(0.5f, 0.5f), g[0], g[1], g[2], g[3], g[4]);
	//	}
	//}

	sliderButton->attachTo(_view);
}

void PresentAnimalsFrame::stopPreviousAndPlayNewSound(string soundName) {
	if (_previousSoundInstance) {
		_previousSoundInstance->stop();
	}
	_previousSoundInstance = sPlayer.play(soundName + "_sound");
}

int PresentAnimalsFrame::getLevelToLoad() {
	int returnValue = 0;

	if (_whichLevel == "level_1") {
		returnValue = 1;
	}
	else if(_whichLevel == "level_2") {
		returnValue = 2;
	}
	else if(_whichLevel == "level_3") {
		returnValue = 3;
	}
	else if(_whichLevel == "level_4") {
		returnValue = 4;
	}
	else if(_whichLevel == "level_5") {
		returnValue = 5;
	}

	return returnValue;
}