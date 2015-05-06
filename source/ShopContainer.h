#ifndef _SHOPCONTAINER_
#define _SHOPCONTAINER_

#include "oxygine-framework.h"
//#include "SharedResources.h"
//#include "FlashUtils.h"

#define OFFSET 4.0f

using namespace oxygine;
//using namespace FlashUtils;

DECLARE_SMART(ShopContainer, spShopContainer);

class ShopContainer : public Actor
{
public:
	ShopContainer(const Vector2& size);
	~ShopContainer();

	void setData();

private:
	void onItemBuy(Event *ev);
};

#endif

