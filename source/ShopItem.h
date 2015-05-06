#ifndef _SHOPITEM_
#define _SHOPITEM_

#include "oxygine-framework.h"
#include "ShopItemModel.h"

using namespace oxygine;

DECLARE_SMART(ShopItem, spShopItem);

class ShopItem : public Actor
{
public:
	class ShopItemEvent : public Event
	{
	public:
		enum EV
		{
			BUY_ITEM = makefourcc('S', 'I', 'B', 'I'),
		};

		ShopItemEvent(EV ev, int itemId_) :Event(ev), itemId(itemId_) {}

		int itemId;
	};

	ShopItem(const Vector2& size, spShopItemModel model);
	~ShopItem();

private:
	void createBackground();
	void createItemSprite(spShopItemModel model);
	void createBuyButton();

	void dispatchBuyItemEvent(Event *ev);
private:
	spSprite _itemSprite;
	int _itemId;
};

#endif