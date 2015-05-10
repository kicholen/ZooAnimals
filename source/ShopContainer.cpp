#include "ShopContainer.h"
#include "ShopManager.h"
#include "ShopItem.h"

ShopContainer::ShopContainer(const Vector2& size) {
	setSize(size);
}

ShopContainer::~ShopContainer() {

}

void ShopContainer::setData() {
	spSlidingActor container = new SlidingActor();
	container->setSize(getSize());
	container->setPriority(-5);

	spColorRectSprite content = new ColorRectSprite();
	content->setColor(Color(0, 0, 22, 120));

	float positionY = getHeight() * 0.3f  / 2.0f + OFFSET;
	float itemHeight = 0.0f;
	const Vector2 size = Vector2(getWidth() * 0.9f, getHeight() * 0.3f);
	itemmap map = ShopManager::instance.getAnimalModels();
	for (itemmap::iterator innerIterator = map.begin(); innerIterator != map.end(); ++innerIterator) {
		spShopItem item = new ShopItem(size, innerIterator->second);
		item->setPosition(getDerivedWidth() / 2.0f, positionY);
		positionY += item->getDerivedHeight() + OFFSET;
		itemHeight = item->getDerivedHeight();
		content->addChild(item);
		item->addEventListener(ShopItem::ShopItemEvent::BUY_ITEM, CLOSURE(this, &ShopContainer::onItemBuy));
	}

	content->setSize(container->getWidth(), positionY - itemHeight / 2.0f);
	
	spColorRectSprite test = new ColorRectSprite();
	test->setSize(content->getSize());
	test->setColor(Color::Aqua);
	test->setPriority(-10);
	content->addChild(test);

	container->setContent(content);
	container->setPosition(getSize() / 2 - container->getSize() / 2);
	container->attachTo(this);
}

void ShopContainer::onItemBuy(Event *ev) {
	dispatchEvent(ev);
}