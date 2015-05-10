#include "ItemBoughtPopup.h"
#include "SharedResources.h"
#include "FlashUtils.h"
#include "LanguageManager.h"

ItemBoughtPopup::ItemBoughtPopup(spShopItemModel model) {
	init("LandingPageFrame.xml", false);
	selectTransitions();
	_model = model;
}

void ItemBoughtPopup::selectTransitions() {
	spTransition transition = new TransitionScale;
	setTransitionIn(transition);
	setTransitionOut(transition);
}

void ItemBoughtPopup::_postHiding(Event *) {
	//FlurryAnalytics::instance.onLevelLeaveEvent(_whichLevel.c_str());
	_view->removeChildren();
	_resources.unload();
}

void ItemBoughtPopup::_preShowing(Event *) {
	//FlurryAnalytics::instance.onLevelEnterEvent(_whichLevel.c_str());
	selectTransitions();
	_resources.load();
	setData();
}

Action ItemBoughtPopup::loop() {
	while (1) {
		Action action = waitAction();
		if (action.id == "back" || action.id == "_btn_back_" || action.id == "close") {
			break;
		}
	}

	return _lastAction;
}

void ItemBoughtPopup::setData() {
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

	spTextField gzMessage = createTextfield(LanguageManager::instance.getText(47), false, true);
	gzMessage->setFontSize2Scale(25 * (int)_view->getWidth() / 640);
	gzMessage->setSize(popupBackground->getHeight() * 0.9f, popupBackground->getHeight() * 0.3f);
	gzMessage->setPosition(_view->getWidth() / 2, _view->getHeight() * 0.2f);
	_view->addChild(gzMessage);

	spSprite itemSprite = new Sprite();
	itemSprite->setAnchor(0.5f, 0.5f);
	itemSprite->setResAnim(animalsResources.getResAnim(_model->resource()));
	itemSprite->setPosition(_view->getWidth() / 2.0f, _view->getHeight() / 2.0f);
	itemSprite->setTouchEnabled(false);
	setActorScaleBySize(itemSprite, Vector2(popupBackground->getWidth() * 0.3f, popupBackground->getHeight() * 0.3f));
	_view->addChild(itemSprite);
	spTextField itemName = createTextfield(LanguageManager::instance.getText(_model->lockit()), false, true);
	itemName->setFontSize2Scale(20 * (int)_view->getWidth() / 640);
	itemName->setSize(popupBackground->getHeight() * 0.9f, popupBackground->getHeight() * 0.3f);
	itemName->setPosition(_view->getWidth() / 2, itemSprite->getY() + itemSprite->getDerivedHeight() / 2.0f + itemName->getTextRect().getHeight() / 2.0f + offset);
	_view->addChild(itemName);

	spTweenButton button = addButton("close", LanguageManager::instance.getText(48), Vector2(_view->getWidth() / 2.0f, _view->getHeight() / 2.0f));
	button->setBaseScale((popupBackground->getWidth() * 0.35f) / button->getWidth());
	button->setPosition(Vector2(_view->getWidth() / 2.0f, itemName->getY() + itemName->getDerivedHeight() / 2.0f + offset));
}