#ifndef _SHOPCONTAINER_
#define _SHOPCONTAINER_

#include "oxygine-framework.h"
//#include "SharedResources.h"
#include "RotatingContainer.h"
//#include "FlashUtils.h"

using namespace oxygine;
//using namespace FlashUtils;

DECLARE_SMART(ShopContainer, spShopContainer);

class ShopContainer : public RotatingContainer
{
public:
	ShopContainer();
	~ShopContainer();
};

#endif

