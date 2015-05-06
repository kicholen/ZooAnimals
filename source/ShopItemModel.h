#ifndef _SHOPITEMMODEL_
#define _SHOPITEMMODEL_

#include "oxygine-framework.h"

using namespace oxygine;

DECLARE_SMART(ShopItemModel, spShopItemModel);

class ShopItemModel : public Object
{
public:
	ShopItemModel(const string& resource, int lockit, int price);
	~ShopItemModel();

	const string& resource() { return _resource; }
	int lockit() const { return _lockit; }
	int price() const { return _price; }

private:
	string _resource;
	int _lockit;
	int _price;
};

#endif