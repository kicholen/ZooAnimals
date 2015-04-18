#include "ChooseGameDifficultyFrame.h"
#include "MemoryFrame.h"
#include "MatchTwoFrame.h"
#include "ConnectDotsFrame.h"
#include "FindShadowFrame.h"
#include "PopObjectsFrame.h"
#include "DiscoverImageFrame.h"
#include "AnimalsManager.h"

ChooseGameDifficultyFrame::ChooseGameDifficultyFrame(const string& gameName) {
	init("LandingPageFrame.xml", false);
	selectTransitions();
	_gameName = gameName;
}

Action ChooseGameDifficultyFrame::loop() {
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
			startGame(action.id);
		}
	}

	return _lastAction;
}

void ChooseGameDifficultyFrame::startGame(const string& difficulty) {
	if (_gameName == "memory") {
		spMemoryFrame memoryFrame = new MemoryFrame(difficulty);
		transitionShowFrame(memoryFrame);
	}
	else if (_gameName == "dots") {
		spConnectDotsFrame connectFrame = new ConnectDotsFrame(difficulty);
		transitionShowFrame(connectFrame);
	}
	else if (_gameName == "shadow") {
		spFindShadowFrame findShadow = new FindShadowFrame(difficulty);
		transitionShowFrame(findShadow);
	}
	else if (_gameName == "match") {
		spMatchTwoFrame matchTwo = new MatchTwoFrame(difficulty);
		transitionShowFrame(matchTwo);
	}
	else if (_gameName == "pop") {
		spPopObjectsFrame pop = new PopObjectsFrame(difficulty);
		transitionShowFrame(pop);
	}
	else if (_gameName == "discover") {
		spDiscoverImageFrame disc = new DiscoverImageFrame(difficulty);
		transitionShowFrame(disc);
	}
}

void ChooseGameDifficultyFrame::selectTransitions() {
	spTransition transition = new TransitionMove;
	setTransitionIn(transition);
	setTransitionOut(transition);
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