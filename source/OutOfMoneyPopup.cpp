#include "OutOfMoneyPopup.h"
#include "SharedResources.h"

OutOfMoneyPopup::OutOfMoneyPopup(spShopItemModel model) {
	init("LandingPageFrame.xml", true);
	selectTransitions();
	_model = model;
}

void OutOfMoneyPopup::selectTransitions() {
	spTransition transition = new TransitionFade;
	setTransitionIn(transition);
	setTransitionOut(transition);
}

void OutOfMoneyPopup::_postHiding(Event *) {
	//FlurryAnalytics::instance.onLevelLeaveEvent(_whichLevel.c_str());
	_view->removeChildren();
	_resources.unload();
}

void OutOfMoneyPopup::_preShowing(Event *) {
	//FlurryAnalytics::instance.onLevelEnterEvent(_whichLevel.c_str());
	selectTransitions();
	_resources.load();
	setData();
}

Action OutOfMoneyPopup::loop() {
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

//void OutOfMoneyPopup::onItemBuy(Event *ev) {
	//ShopItem::ShopItemEvent *shopEvent = safeCast<ShopItem::ShopItemEvent*>(ev);

	//	ShopManager::instance.
//}

void OutOfMoneyPopup::setData() {
	float offset = 5.0f;

	spBox9Sprite popupBackground;
	popupBackground = new Box9Sprite;
	popupBackground->setAnchor(0.5f, 0.5f);
	popupBackground->setResAnim(gameResources.getResAnim("popup_box9_ad"));
	popupBackground->setSize(_view->getHeight() * 0.8f, _view->getHeight() * 0.8f);
	popupBackground->setPosition(_view->getWidth() / 2, _view->getHeight() / 2);
	popupBackground->setTouchEnabled(false);
	popupBackground->setVerticalMode(Box9Sprite::STRETCHING);
	popupBackground->setHorizontalMode(Box9Sprite::STRETCHING);
	popupBackground->setGuides(50, 200, 50, 200);
	popupBackground->attachTo(_view);

	spTextField gzMessage = createTextfield("*Play games to get more money!*", false, true);
	gzMessage->setFontSize2Scale(25 * (int)_view->getWidth() / 640);
	gzMessage->setSize(popupBackground->getHeight() * 0.9f, popupBackground->getHeight() * 0.3f);
	gzMessage->setPosition(_view->getWidth() / 2, _view->getHeight() * 0.2f);
	_view->addChild(gzMessage);
}