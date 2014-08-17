#include "ChooseMemoryDifficultyFrame.h"
#include "MemoryFrame.h"

ChooseMemoryDifficultyFrame::ChooseMemoryDifficultyFrame() {
	init("LandingPageFrame.xml", false);
	selectTransitions();
}

Action ChooseMemoryDifficultyFrame::loop() {
	while (1) {
		Action action = waitAction();
		if (action.id == "back" || action.id == "_btn_back_") {
			break;
		}
		else if (action.id == "close") {
			break;
		}
		else if (action.id == "result") {
			break;
		}
		else if (action.id == "easy" || action.id == "hard" || action.id == "normal") {
			spMemoryFrame memoryFrame = new MemoryFrame(action.id.c_str());
			transitionShowFrame(memoryFrame);
		}
	}

	return _lastAction;
}

void ChooseMemoryDifficultyFrame::selectTransitions() {
	spTransition transition = new TransitionMove;
	setTransitionIn(transition);
	setTransitionOut(transition);
}

void ChooseMemoryDifficultyFrame::_postHiding(Event *) {
	//FlurryAnalytics::instance.onLevelLeaveEvent(_whichLevel.c_str());
	_view->removeChildren();
	_resources.unload();
}

void ChooseMemoryDifficultyFrame::_preShowing(Event *) {
	//FlurryAnalytics::instance.onLevelEnterEvent(_whichLevel.c_str());
	selectTransitions();
	_resources.load();
	setData();
}


void ChooseMemoryDifficultyFrame::setData() {
	float offset = 10.0f;
	spTweenButton button = addButton("easy", "EASY", Vector2(0.0f, 0.0f));
	button->setPosition(_view->getWidth() / 2, _view->getHeight() / 2 - button->getDerivedHeight() - offset); 
	addButton("normal", "NORMAL", Vector2(_view->getWidth() / 2, _view->getHeight() / 2));
	addButton("hard", "HARD", Vector2(_view->getWidth() / 2, _view->getHeight() / 2 + button->getDerivedHeight() + offset));
}