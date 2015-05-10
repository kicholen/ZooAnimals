#include "ShopItem.h"
#include "SharedResources.h"

ShopItem::ShopItem(const Vector2& size, spShopItemModel model) {
	setTouchEnabled(false);
	setAnchor(0.5f, 0.5f);
	setSize(size);
	setTouchEnabled(false);
	createBackground();
	createItemSprite(model);
	createBuyButton();
	_itemId = model->index();
}

ShopItem::~ShopItem() {

}

void ShopItem::createItemSprite(spShopItemModel model) {
	_itemSprite = new Sprite();
	_itemSprite->setAnchor(0.5f, 0.5f);
	_itemSprite->setResAnim(animalsResources.getResAnim(model->resource()));
	_itemSprite->setPosition(getWidth() * 0.33f, getHeight() / 2);
	_itemSprite->setTouchEnabled(false);
	setActorScaleBySize(_itemSprite, Vector2(getWidth() * 0.4f, getHeight() * 0.8f));
	addChild(_itemSprite);
}

void ShopItem::createBackground() {
	spBox9Sprite cardBackground = new Box9Sprite;
	cardBackground->setHorizontalMode(Box9Sprite::STRETCHING);
	cardBackground->setVerticalMode(Box9Sprite::STRETCHING);
	cardBackground->setAnchor(0.5f, 0.5f);
	cardBackground->setResAnim(gameResources.getResAnim("greyBox9"));
	cardBackground->setSize(getSize());
	cardBackground->setPosition(getWidth() / 2, getHeight() / 2);
	cardBackground->setGuides(9, 20, 9, 15);
	cardBackground->attachTo(this);
	cardBackground->setPriority(-1);
	cardBackground->setTouchEnabled(false);
}

void ShopItem::createBuyButton() {
	spTweenButton button = createButton("BUY", "BUY");
	button->setPosition(getWidth() - button->getWidth() / 2 - 5.0f, getHeight() - button->getHeight() / 2 - 5.0f);
	button->addEventListener(TouchEvent::CLICK, CLOSURE(this, &ShopItem::dispatchBuyItemEvent));
	addChild(button);
}

void ShopItem::dispatchBuyItemEvent(Event *ev) {
	ShopItemEvent shopItemEven(ShopItemEvent::BUY_ITEM, _itemId);
	dispatchEvent(&shopItemEven);
}