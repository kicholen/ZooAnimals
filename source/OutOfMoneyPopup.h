#ifndef _OUTOFMONEYPOPUP_
#define _OUTOFMONEYPOPUP_

#include "CustomFrame.h"
#include "ShopItemModel.h"

DECLARE_SMART(OutOfMoneyPopup, spOutOfMoneyPopup);

class OutOfMoneyPopup : public CustomFrame
{
public:
	OutOfMoneyPopup(spShopItemModel model);

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