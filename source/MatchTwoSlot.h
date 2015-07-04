#ifndef _MATCHTWOSLOT_
#define _MATCHTWOSLOT_

#include "oxygine-framework.h"
#include "DraggableSprite.h"

using namespace oxygine;

#define MATCH_SLOT_SIZE_X 400
#define MATCH_SLOT_SIZE_Y 200

typedef enum {mtsWaiting, mtsAnimating, mtsCompleted} MatchTwoSlotState;

DECLARE_SMART(MatchTwoSlot, spMatchTwoSlot);

class MatchTwoSlot : public Actor
{
public:
	MatchTwoSlot();

	void setSprite(std::string spriteName);
	void setDragBounds(Vector2 position, Vector2 size);
	void fitToSlot(spSprite sprite);
	Vector2 getBasketPosition();
private:
	void createPairSprite();
	void createBasket();
	void createBackground();
private:
	MatchTwoSlotState _state;
	spSprite _basket;
	spSprite _pairSprite;

	spDraggableSprite _draggableSprite;
};

#endif