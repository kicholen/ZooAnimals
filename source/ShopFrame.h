#ifndef _SHOPFRAME_
#define _SHOPFRAME_

#include "CustomFrame.h"

DECLARE_SMART(ShopFrame, spShopFrame);

class ShopFrame : public CustomFrame
{
public:
	ShopFrame();

	void selectTransitions();

	Action loop();

protected:
	void _postHiding(Event *);
	void _preShowing(Event *);

private:
	void setData();
};

#endif