#ifndef _SHOPITEMMODEL_
#define _SHOPITEMMODEL_

#include "oxygine-framework.h"

using namespace oxygine;

DECLARE_SMART(ShopItemModel, spShopItemModel);

class ShopItemModel : public Object
{
public:
	ShopItemModel(int index, const string& resource, const string& region, int lockit, int price);
	~ShopItemModel();

	/* resource == name */
	const string& resource() { return _resource; }
	const string& region() { return _region; }
	int lockit() const { return _lockit; }
	int price() const { return _price; }
	int index() const { return _index;  }
private:
	string _resource;
	string _region;
	int _lockit;
	int _price;
	int _index;
};

#endif