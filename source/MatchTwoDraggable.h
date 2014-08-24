#ifndef _MATCHTWODRAGGABLE_
#define _MATCHTWODRAGGABLE_

#include "oxygine-framework.h"
#include "DraggableSprite.h"

using namespace oxygine;

DECLARE_SMART(MatchTwoDraggable, spMatchTwoDraggable);

class MatchTwoDraggable : public DraggableSprite
{
public:
	MatchTwoDraggable();

	void setBasePosition(Vector2 position);
	Vector2 getBasePosition();
private:
	Vector2 _basePosition;
};

#endif