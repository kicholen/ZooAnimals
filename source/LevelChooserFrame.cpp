#include "LevelChooserFrame.h"
#include "RotatingContainer.h"
#include "FlurryAnalytics.h"
#include "FlurryAds.h"
#include "GooglePlayInAppPurchaseManager.h"
#include "Settings.h"
#include "ShaderSprite.h"
#include "ChooseGameDifficultyFrame.h"

#include "MatchTwoFrame.h"

#include "ConnectDotsFrame.h"

#include "FindShadowFrame.h"

#include "ZooFrame.h"

LevelChooserFrame::LevelChooserFrame() {
	init("LevelChooserFrame.xml", true);
	_counter = 0;
	selectTransitions();
}

void LevelChooserFrame::selectTransitions() {
	spTransition transition = new TransitionMove;
	setTransitionIn(transition);
	setTransitionOut(transition);
}

Action LevelChooserFrame::loop() {
	while (1) {
		Action action = waitAction();
		if (action.id == "close") {
			break;
		}
		else if (action.id == "back") {
			break;
		}
		else if (action.id == "memory" || action.id == "dots" || action.id == "shadow" || action.id == "match" || action.id == "pop" || action.id == "discover") {
			spChooseGameDifficultyFrame chooserFrame = new ChooseGameDifficultyFrame();
			transitionShowFrame(chooserFrame);
		}
		else if (action.id == "zoo_frame") {
//			spZooFrame zooFrame = new ZooFrame();
//			transitionShowFrame(zooFrame);
		}
	}

	return _lastAction;
}

void LevelChooserFrame::_postHiding(Event *) {
	_view->removeChildren();
	
	_resources.unload();
}

void LevelChooserFrame::_preShowing(Event *) {
	_counter++;
	selectTransitions();
	setData();
	_resources.load();
}

void LevelChooserFrame::setData() {
	addButton("back", "BACK", Vector2(_view->getWidth() / 2,  _content->getHeight() * 0.9f ));
	spTweenButton button = addButton("memory", "memory", Vector2(_view->getWidth() / 2,  _content->getHeight() * 0.2f ));
	addButton("connect_dots", "connect_dots", Vector2(_view->getWidth() / 2,  _content->getHeight() * 0.2f + button->getDerivedHeight() + 10));
	addButton("zoo_frame", "zoo_frame", Vector2(_view->getWidth() / 2,  _content->getHeight() * 0.2f + (button->getDerivedHeight() + 10) * 2));
	addButton("find_shadow", "find_shadow", Vector2(_view->getWidth() / 2,  _content->getHeight() * 0.2f + (button->getDerivedHeight() + 10) * 3));
	addButton("match_two", "match_two", Vector2(_view->getWidth() / 2,  _content->getHeight() * 0.2f + (button->getDerivedHeight() + 10) * 4));

	/*spTextActor text = createTextfield("In which place do you want to play?", true, 0, true);
	text->setFontSize2Scale(40 * _view->getWidth() / 640);
	text->setSize(_view->getWidth() * 0.75f, _view->getHeight() * 0.15f);
	text->setPosition(_view->getWidth() / 2, _view->getHeight() * 0.09f);
	text->setColor(Color(35, 145, 245));
	text->attachTo(_view);*/
}