#include "ShopManager.h"
#include "Content.h"
#include "MoneyManager.h"
#include "AnimalsManager.h"

ShopManager ShopManager::instance;

ShopManager::ShopManager() {

}

ShopManager::~ShopManager() {
	for (std::map<int, spShopItemModel>::iterator innerIterator = _animalItemMap.begin(); innerIterator != _animalItemMap.end(); ++innerIterator) {
		innerIterator->second = 0;
	}
	_animalItemMap.clear();
}

void ShopManager::init(const std::string& version) {
	createShopItemsFromContent();
}

void ShopManager::createShopItemsFromContent() {
	createMap(_animalItemMap, "animals");
}

void ShopManager::createMap(itemmap &map, const std::string& shopType) {
	pugi::xml_node animalShop = Content::instance.getShopFirstChildNode(shopType);

	while (!animalShop.empty()) {
		int index = 0;
		int lockit = 0;
		const char* resource;
		int price = 0;
		const char* region = animalShop.name();
		pugi::xml_node itemShop = animalShop.first_child();

		while (!itemShop.empty()) {
			pugi::xml_attribute attribute = itemShop.first_attribute();

			while (!attribute.empty()) {
				const char *name = attribute.name();

				if (!strcmp(name, "index")) {
					index = attribute.as_int();
				}
				else if (!strcmp(name, "lockit")) {
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
			spShopItemModel model = new ShopItemModel(index, resource, region, lockit, price);

			map.insert(std::make_pair(index, model));

			itemShop = itemShop.next_sibling();
		}

		animalShop = animalShop.next_sibling();
	}
}

const itemmap& ShopManager::getAnimalModels() const {
	return _animalItemMap;
}

spShopItemModel ShopManager::getItemByIndex(int index) {
	if (_animalItemMap.count(index) > 0) {
		return _animalItemMap[index];
	}
	if (_itemsMap.count(index) > 0) {
		return _itemsMap[index];
	}
	if (_mixMap.count(index) > 0) {
		return _mixMap[index];
	}

	OX_ASSERT(false);
	return new ShopItemModel(0, "", "", 0, 0);
}

ShopManagerBuyResult ShopManager::buyItemByMoney(spShopItemModel model) {
	if (MoneyManager::instance.getMoney() > model->price()) {
		MoneyManager::instance.decreaseMoneyOnItemBought(model->price());
		AnimalsManager::instance.increaseAnimalCount(model->region(), model->resource(), 1);
		return smBought;
	}
	else {
		return smNotEnoughtMoney;
	}
}


// todo
void ShopManager::updateShopItemsFromBackendAsync() {

}