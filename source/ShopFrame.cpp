#include "ShopFrame.h"
#include "SlidingActor.h"
#include "ShopContainer.h"

ShopFrame::ShopFrame() {
	init("LandingPageFrame.xml", true);
	selectTransitions();
}

void ShopFrame::selectTransitions() {
	spTransition transition = new TransitionFade;
	setTransitionIn(transition);
	setTransitionOut(transition);
}

void ShopFrame::_postHiding(Event *) {
	//FlurryAnalytics::instance.onLevelLeaveEvent(_whichLevel.c_str());
	_view->removeChildren();
	_resources.unload();
}

void ShopFrame::_preShowing(Event *) {
	//FlurryAnalytics::instance.onLevelEnterEvent(_whichLevel.c_str());
	selectTransitions();
	_resources.load();
	setData();
}

Action ShopFrame::loop() {
	while (1) {
		Action action = waitAction();
		if (action.id == "back" || action.id == "_btn_back_" || action.id == "close") {
			break;
		}
		/*else if (action.id == "memory") {
			//spChooseMemoryDifficultyFrame chooserFrame = new ChooseMemoryDifficultyFrame();
			//transitionShowFrame(chooserFrame);
		}*/
	}

	return _lastAction;
}

void ShopFrame::setData() {
	spShopContainer shopContainer = new ShopContainer(_view->getSize());
	shopContainer->setData();
	shopContainer->setPosition(_view->getSize() / 2 - shopContainer->getSize() / 2);
	shopContainer->attachTo(_view);
}