#include "ShopFrame.h"
#include "SlidingActor.h"
#include "ShopContainer.h"
#include "ShopItem.h"
#include "MoneyManager.h"
#include "ShopManager.h"
#include "OutOfMoneyPopup.h"
#include "ItemBoughtPopup.h"

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
		else if (action.id == "out_of_money") {
			spOutOfMoneyPopup outOfMoneyPopup = new OutOfMoneyPopup(_model);
			transitionShowFrameAsDialog(outOfMoneyPopup);
		}
		else if (action.id == "buy") {
			spItemBoughtPopup itemBoughtPopup = new ItemBoughtPopup(_model);
			transitionShowFrameAsDialog(itemBoughtPopup);
		}
	}

	return _lastAction;
}

void ShopFrame::onItemBuy(Event *ev) {
	ShopItem::ShopItemEvent *shopEvent = safeCast<ShopItem::ShopItemEvent*>(ev);
	
	_model = ShopManager::instance.getItemByIndex(shopEvent->itemId);

	ShopManagerBuyResult result = ShopManager::instance.buyItemByMoney(_model);

	if (result == smBought) {
		generateAction("buy");
	}
	else {
		generateAction("out_of_money");
	}
}

void ShopFrame::setData() {
	spShopContainer shopContainer = new ShopContainer(_view->getSize());
	shopContainer->setData();
	shopContainer->setPosition(_view->getSize() / 2 - shopContainer->getSize() / 2);
	shopContainer->attachTo(_view);
	shopContainer->addEventListener(ShopItem::ShopItemEvent::BUY_ITEM, CLOSURE(this, &ShopFrame::onItemBuy));
}

bool ShopFrame::canItemBeBought(int price) {
	return MoneyManager::instance.getMoney() > price;
}