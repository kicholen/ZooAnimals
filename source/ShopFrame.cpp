#include "ShopFrame.h"
#include "SlidingActor.h"

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
	spSlidingActor container = new SlidingActor();
	container->setSize(_view->getWidth(), _view->getHeight());
	container->setPriority(-5);

	spColorRectSprite content = new ColorRectSprite();
	content->setSize(container->getWidth(), container->getHeight() * 2.0f);
	content->setColor(Color(0, 0, 22, 120));

	spColorRectSprite test = new ColorRectSprite();
	test->setSize(container->getWidth(), container->getHeight());
	test->setColor(Color::Aqua);

	content->addChild(test);
	container->setContent(content);
	container->setPosition(_view->getSize() / 2 - container->getSize() / 2);
	container->attachTo(_view);
}