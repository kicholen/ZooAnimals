#include "WorldMapFrame.h"

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

		}
	}

	return _lastAction;
}

void WorldMapFrame::onContinentSwitched(Event *event) {
	WroldMapView::WroldMapViewEvent *worldMapEvent = safeCast<WroldMapView::WroldMapViewEvent*>(event);
	_animalsContainer->setRegion(worldMapEvent->_name);
}

void WorldMapFrame::setData() {
	_animalsContainer = new RegionAnimalsContainer(Vector2(_view->getWidth() * 0.8f, _view->getHeight() * 0.3f));
	_animalsContainer->setRegion("farm");
	_animalsContainer->attachTo(_view);
	_animalsContainer->setPosition((_view->getWidth() - _animalsContainer->getWidth()) / 2, _view->getHeight() - _animalsContainer->getHeight());

	spWroldMapView worldMap = new WroldMapView(Vector2(_view->getWidth() * 0.8f, _view->getHeight() * 0.7f));
	worldMap->setData();
	worldMap->attachTo(_view);
	worldMap->setPosition((_view->getWidth() - worldMap->getWidth()) / 2, 0.0f);
	worldMap->addEventListener(WroldMapView::WroldMapViewEvent::CONTINENT_SWITCHED, CLOSURE(this, &WorldMapFrame::onContinentSwitched));

	addButton("back", "BACK", Vector2(getRoot()->getWidth() * 0.9f, getRoot()->getHeight() * 0.9f));
}