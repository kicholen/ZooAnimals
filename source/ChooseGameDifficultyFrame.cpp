#include "ChooseGameDifficultyFrame.h"

ChooseGameDifficultyFrame::ChooseGameDifficultyFrame() {
	init("LandingPageFrame.xml", false);
	selectTransitions();
}

Action ChooseGameDifficultyFrame::loop() {
	while (1) {
		Action action = waitAction();
		if (action.id == "back" || action.id == "_btn_back_" || action.id == "close") {
			break;
		}
		else if (action.id == "easy" || action.id == "hard" || action.id == "normal") {
			spTransition transitionOut = new TransitionInstant;
			setTransitionOut(transitionOut);
			break;
		}
	}

	return _lastAction;
}

void ChooseGameDifficultyFrame::selectTransitions() {
	spTransition transitionIn = new TransitionMove;
	setTransitionIn(transitionIn);
	spTransition transitionOut = new TransitionMove;
	setTransitionOut(transitionOut);
}

void ChooseGameDifficultyFrame::_postHiding(Event *) {
	//FlurryAnalytics::instance.onLevelLeaveEvent(_whichLevel.c_str());
	_view->removeChildren();
	_resources.unload();
}

void ChooseGameDifficultyFrame::_preShowing(Event *) {
	//FlurryAnalytics::instance.onLevelEnterEvent(_whichLevel.c_str());
	selectTransitions();
	_resources.load();
	setData();
}


void ChooseGameDifficultyFrame::setData() {
	float offset = 10.0f;
	spTweenButton button = addButton("easy", "EASY", Vector2(0.0f, 0.0f));
	button->setPosition(_view->getWidth() / 2, _view->getHeight() / 2 - button->getDerivedHeight() - offset); 
	addButton("normal", "NORMAL", Vector2(_view->getWidth() / 2, _view->getHeight() / 2));
	addButton("hard", "HARD", Vector2(_view->getWidth() / 2, _view->getHeight() / 2 + button->getDerivedHeight() + offset));
}