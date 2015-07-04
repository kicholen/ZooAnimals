#include "MatchTwoSlot.h"
#include "SharedResources.h"

MatchTwoSlot::MatchTwoSlot() {
	setSize(MATCH_SLOT_SIZE_X, MATCH_SLOT_SIZE_Y);
	setAnchor(0.5f, 0.5f);

	/*spColorRectSprite bg = new ColorRectSprite();
	bg->setColor(Color(120, 0, 0));
	bg->setSize(getSize());
	bg->setAnchor(0.0f, 0.0f);
	bg->setPosition(0.0f, 0.0f);
	bg->setPriority(-1000);
	addChild(bg);*/

	createBackground();
	createPairSprite();
	createBasket();

	_state = mtsWaiting;
}

void MatchTwoSlot::setSprite(std::string spriteName) {
	_pairSprite->setResAnim(animalsResources.getResAnim(spriteName));
}

Vector2 MatchTwoSlot::getBasketPosition() {
	return local2global(_basket->getPosition());
}

void MatchTwoSlot::createBackground() {
	spBox9Sprite cardBackground = new Box9Sprite;
	cardBackground->setAnchor(0.5f, 0.5f);
	cardBackground->setResAnim(gameResources.getResAnim("memory_card_box9_a"));
	cardBackground->setSize(getSize());
	cardBackground->setPosition(getWidth() / 2, getHeight() / 2);
	cardBackground->setGuides(36, 144, 36, 144);
	cardBackground->attachTo(this);
	cardBackground->setPriority(-11);
	cardBackground->setName("cat");
}

void MatchTwoSlot::createPairSprite() {
	_pairSprite = initActor(new Sprite,
		arg_resAnim = animalsResources.getResAnim("cat"),
		arg_name = "pair_sprite",
		arg_anchor = Vector2(0.5f, 0.5f),
		arg_x = getWidth() * 0.75f,
		arg_y = getHeight() / 2,
		arg_attachTo = this);
	fitToSlot(_pairSprite);
}

void MatchTwoSlot::createBasket() {
	_basket = initActor(new Sprite,
		arg_name = "basket",
		arg_anchor = Vector2(0.5f, 0.5f),
		arg_attachTo = this,
		arg_size = Vector2(MATCH_SLOT_SIZE_X / 2, MATCH_SLOT_SIZE_Y),
		arg_x = getWidth() * 0.25f,
		arg_priority = -10,
		arg_y = getHeight() / 2);
}

void MatchTwoSlot::fitToSlot(spSprite sprite) {
	float scaleInParent = 0.8f;
	float scale = 1.0f;
	if (sprite->getWidth() > sprite->getHeight()) {
		scale = scaleInParent * MATCH_SLOT_SIZE_Y / sprite->getWidth();
	}
	else {
		scale = scaleInParent * MATCH_SLOT_SIZE_Y / sprite->getHeight();
	}
	sprite->setScale(scale);
}