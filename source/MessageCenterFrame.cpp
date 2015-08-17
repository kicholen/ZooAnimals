#include "MessageCenterFrame.h"
#include "MessageCenterManager.h"

MessageCenterFrame::MessageCenterFrame() {
	init("LandingPageFrame.xml", true);
	selectTransitions();
}

MessageCenterFrame::~MessageCenterFrame() {

}

void MessageCenterFrame::selectTransitions() {
	spTransition transition = new TransitionFade;
	setTransitionIn(transition);
	setTransitionOut(transition);
}

void MessageCenterFrame::_postHiding(Event *) {
	//FlurryAnalytics::instance.onLevelLeaveEvent(_whichLevel.c_str());
	_view->removeChildren();
	_resources.unload();
}

void MessageCenterFrame::_preShowing(Event *) {
	//FlurryAnalytics::instance.onLevelEnterEvent(_whichLevel.c_str());
	selectTransitions();
	_resources.load();
	setData();
}


Action MessageCenterFrame::loop() {
	while (1) {
		Action action = waitAction();
		if (action.id == "back" || action.id == "_btn_back_") {
			break;
		}
	}

	return _lastAction;
}

void MessageCenterFrame::setData() {
	const std::vector<spMessageModel>& messages = MessageCenterManager::instance.getMessages();

	for (uint i = 0; i < messages.size(); i++) {
		//createMessageItem();
	}
}

spMessageItem MessageCenterFrame::createMessageItem(spMessageModel model) {
	spMessageItem item = new MessageItem(Vector2(100, 100), model);

	return item;
}