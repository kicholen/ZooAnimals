#include "ShopManager.h"
#include "Content.h"

ShopManager ShopManager::instance;

ShopManager::ShopManager() {

}

ShopManager::~ShopManager() {
	for (map<int, spShopItemModel>::iterator innerIterator = _animalItemMap.begin(); innerIterator != _animalItemMap.end(); ++innerIterator) {
		innerIterator->second->releaseRef();
	}
	_animalItemMap.clear();
}

void ShopManager::init(const string& version) {
	createShopItemsFromContent();
}


void ShopManager::createShopItemsFromContent() {
	createMap(_animalItemMap, "animals");

	//	_itemsMap = 
}

void ShopManager::createMap(itemmap &map, const string& shopType) {
	pugi::xml_node itemShop = Content::instance.getShopFirstChildNode(shopType);
	int index = 0;
	int lockit;
	const char* resource;
	int price;

	while (!itemShop.empty()) {
		pugi::xml_attribute attribute = itemShop.first_attribute();
		
		while (!attribute.empty()) {
			const char *name = attribute.name();

			if (!strcmp(name, "lockit")) {
				lockit = attribute.as_int();
			}
			else if (!strcmp(name, "resource")) {
				resource = attribute.as_string();
			}
			else if (!strcmp(name, "price")) {
				price = attribute.as_int();
			}
		
			attribute = attribute.next_attribute();
		}
		spShopItemModel model = new ShopItemModel(resource, lockit, price);
		
		map.insert(make_pair(index, model));
		index += 1;

		itemShop = itemShop.next_sibling();
	}
}

const itemmap& ShopManager::getAnimalModels() const {
	return _animalItemMap;
}

// todo
void ShopManager::updateShopItemsFromBackendAsync() {

}