#include "ShopItemModel.h"


ShopItemModel::ShopItemModel(const string& resource, int lockit, int price) {
	_resource = resource;
	_lockit = lockit;
	_price = price;
}


ShopItemModel::~ShopItemModel() {

}

