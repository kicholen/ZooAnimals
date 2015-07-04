#include "ShopItemModel.h"


ShopItemModel::ShopItemModel(int index, const std::string& resource, const std::string& region, int lockit, int price) {
	_index = index;
	_resource = resource;
	_lockit = lockit;
	_price = price;
	_region = region;
}


ShopItemModel::~ShopItemModel() {

}

