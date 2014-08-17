#include "BuyItemPopup.h"
#include "Settings.h"
#include "TextActor.h"
#include "GooglePlayInAppPurchaseManager.h"

BuyItemPopup::BuyItemPopup() {
	init("popup.xml");
	_updateCallback.p_this = this;
	_updateCallback.p_proxy = CLOSURE(this, &BuyItemPopup::_update).p_proxy;
	selectTransitions();
}

void BuyItemPopup::selectTransitions() {
	spTransition tr[] = {new TransitionScale};
	int c = sizeof(tr) / sizeof(tr[0]);
	setTransitionIn(tr[rand() % c]);
	setTransitionOut(tr[rand() % c]);
}

Action BuyItemPopup::loop() {
	while (1) {
		Action action = waitAction();
		if (action.id == "close") {
			break;
		}
		if (action.id == "buy") {
			GooglePlayInAppPurchaseManager::instance.purchaseProduct("test_product", true);
		}
	}

	return _lastAction;
}

void BuyItemPopup::_postHiding(Event *) {
	_view->removeChildren();
	_resources.unload();
}

void BuyItemPopup::_preShowing(Event *) {
	selectTransitions();
	_resources.load();
	setData();
}

void BuyItemPopup::setData() {
	spBox9Sprite popupBackground;
	popupBackground = new Box9Sprite;
	popupBackground->setAnchor(0.5f, 0.5f);
	popupBackground->setResAnim(gameResources.getResAnim("popup_box9_ad"));
	popupBackground->setSize(_view->getHeight() * 0.8f, _view->getHeight() * 0.8f);
	popupBackground->setPosition(_view->getWidth() / 2, _view->getHeight() / 2);
	popupBackground->setInputEnabled(false);
	popupBackground->setVerticalMode(Box9Sprite::TILING_FULL);
	popupBackground->setHorizontalMode(Box9Sprite::TILING_FULL);
	popupBackground->setGuides(50, 200, 50, 200);
	popupBackground->attachTo(_view);

	spTextActor infoMessage = createTextfield("Would you like to unlock all levels and remove ads?", true, 0, true);
	infoMessage->setFontSize2Scale(20 * _view->getWidth() / 640);
	infoMessage->setSize(popupBackground->getHeight() * 0.9f, popupBackground->getHeight() * 0.7f);
	infoMessage->setPosition(_view->getWidth() / 2, _view->getHeight() * 0.4f);
	_view->addChild(infoMessage);

	float offset = 2.5;
	spTweenButton button = addButton("close", "CANCEL", Vector2(_view->getWidth() / 2, _view->getHeight() / 2));
	button->setBaseScale((popupBackground->getWidth() * 0.35f) / button->getWidth());
	button->setPosition(Vector2(_view->getWidth() / 2 - button->getDerivedWidth() / 2 - offset, _view->getHeight() * 0.7f));
	button = addButton("buy", "BUY",  Vector2(_view->getWidth() / 2 + button->getDerivedWidth() / 2 + offset, _view->getHeight() * 0.7f));
	button->setBaseScale((popupBackground->getWidth() * 0.35f) / button->getWidth());


	spActor updatePostStateActor = new Actor();
	updatePostStateActor->setCallbackDoUpdate(_updateCallback);
	updatePostStateActor->attachTo(_view);
}

void BuyItemPopup::_update(const UpdateState &us) {
	bool canCheckFileAgain = false;

	if (us.time > (_lastCheckTimeMS + 1000)) {
		canCheckFileAgain = true;
		_lastCheckTimeMS = us.time;
	}
	if (canCheckFileAgain && Settings::instance.getValue("unblock_ads").as_int() == 1) {
		WasPurchaseCompletedEvent wasPurchaseCompletedEvent(WasPurchaseCompletedEvent::PURCHASE_COMPLETED);
		dispatchEvent(&wasPurchaseCompletedEvent);
		generateAction("close");
	}
}