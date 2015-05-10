#ifndef _ITEMBOUGHTPOPUP_
#define _ITEMBOUGHTPOPUP_

#include "CustomFrame.h"
#include "ShopItemModel.h"

DECLARE_SMART(ItemBoughtPopup, spItemBoughtPopup);

class ItemBoughtPopup : public CustomFrame
{
public:
	ItemBoughtPopup(spShopItemModel model);

	void selectTransitions();

	Action loop();

protected:
	void _postHiding(Event *);
	void _preShowing(Event *);

private:
	void setData();

private:
	spShopItemModel _model;
};

#endif