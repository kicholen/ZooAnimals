#include "WorldMapFrame.h"
#include "ZooFrame.h"

#include "WroldMapView.h"

WorldMapFrame::WorldMapFrame() {
	init("LandingPageFrame.xml", true);
	selectTransitions();
}

void WorldMapFrame::selectTransitions() {
	spTransition transition = new TransitionFade;
	setTransitionIn(transition);
	setTransitionOut(transition);
}

void WorldMapFrame::_postHiding(Event *) {
	//FlurryAnalytics::instance.onLevelLeaveEvent(_whichLevel.c_str());
	_view->removeChildren();
	_resources.unload();
}

void WorldMapFrame::_preShowing(Event *) {
	//FlurryAnalytics::instance.onLevelEnterEvent(_whichLevel.c_str());
	selectTransitions();
	_resources.load();
	setData();
}

Action WorldMapFrame::loop() {
	while (1) {
		Action action = waitAction();
		if (action.id == "back" || action.id == "_btn_back_") {
			break;
		}
		else if (action.id == "close") {
			break;
		}
		else if (action.id == "result") {
			spZooFrame zooFrame = new ZooFrame();
			transitionShowFrame(zooFrame);
		}
	}

	return _lastAction;
}

void WorldMapFrame::onFinished(Event *event) {
	//FarmControlPanel::FarmControlPanelEvent *fcpe = safeCast<FarmControlPanel::FarmControlPanelEvent*>(event);
	//generateAction(fcpe->_name);
}

void WorldMapFrame::setData() {
	spWroldMapView worldMap = new WroldMapView(_view->getSize());
	worldMap->setData();
	worldMap->attachTo(_view);

	spTweenButton button = addButton("back", "BACK", Vector2(getRoot()->getWidth() * 0.9f, getRoot()->getHeight() * 0.9f));
	//addButton("store", "store", Vector2(getRoot()->getWidth() * 0.6f, getRoot()->getHeight() * 0.9f));
}