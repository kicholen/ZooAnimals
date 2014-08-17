#ifndef _DRAGGABLESPRITE_
#define _DRAGGABLESPRITE_

#include "Sprite.h"
#include "Draggable.h"

using namespace oxygine;

DECLARE_SMART(DraggableSprite, spDraggableSprite);

class DraggableSprite : public Sprite
{
public:
	DraggableSprite();

private:
	Draggable drag;
};

#endif