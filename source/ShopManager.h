#ifndef _SHOPMANAGER_
#define _SHOPMANAGER_

#include "FlashUtils.h"
#include <map>
#include "ShopItemModel.h"

using namespace FlashUtils;

typedef map<int, spShopItemModel> itemmap;

/*  Stores ShopItemModels
**  - any animal can wear any wearable, content so much
*/
class ShopManager : public EventDispatcher
{
public:
	static ShopManager instance;

	// events probably wont be needed here, call every other manager seperately in order to update ?
	class ShopEvent : public Event
	{
	public:
		enum EV
		{
			ITEM_BOUGHT = makefourcc('S', 'E', 'I', 'B'),
		};

		ShopEvent(EV ev, int id_) : Event(ev), id(id_) {}

		int id;
	};

	ShopManager();
	~ShopManager();

	void init(const string& version);

	const itemmap& getAnimalModels() const;

private:
	void createShopItemsFromContent();
	void updateShopItemsFromBackendAsync();

	void createMap(itemmap &map, const string& shopType);

private:
	itemmap _animalItemMap;
	itemmap _itemsMap;
	itemmap _mixMap;
};

#endif