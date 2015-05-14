#include "PresentAnimalsFrame.h"
#include "FlurryAnalytics.h"
#include "SoundPlayer.h"
#include "SoundInstance.h"
#include "ResSound.h"
#include "AnimalCardElement.h"

PresentAnimalsFrame::PresentAnimalsFrame(spAnimalModel model) {
	init("LandingPageFrame.xml", false);

	_model = model;
	selectTransitions();
}

void PresentAnimalsFrame::selectTransitions() {
	spTransition transition = new TransitionMove;
	setTransitionIn(transition);
	setTransitionOut(transition);
}

void PresentAnimalsFrame::_postHiding(Event *) {
	//FlurryAnalytics::instance.onLevelLeaveEvent(_whichLevel.c_str());
	_view->removeChildren();
	_resources.unload();
}

void PresentAnimalsFrame::_preShowing(Event *) {
	//FlurryAnalytics::instance.onLevelEnterEvent(_whichLevel.c_str());
	selectTransitions();
	_resources.load();
	setData();
}

Action PresentAnimalsFrame::loop() {
	while (1) {
		Action action = waitAction();
		if (action.id == "_btn_back_") {
			break;
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

void PresentAnimalsFrame::setData() {
	spAnimalCardElement animalCard = new AnimalCardElement(Vector2(_view->getHeight() * 0.6f, _view->getHeight() * 0.8f), _model);
	animalCard->setPosition(_view->getSize() / 2.0f);
	_view->addChild(animalCard);
	// we move position for every next animal whole screen to right, for every left animal, whole screen to left
	// also we block swipe until previous one ends
	//for(int i = 0; i < groupArray->length(); i++) {
	//	Group &g = *(*groupArray)[i];
	//	if(g[0] > 0) {
	//		addDraggableSprite(g.spriteName, Vector2(0.5f, 0.5f), g[0], g[1], g[2], g[3], g[4]);
	//	}
	//}
}