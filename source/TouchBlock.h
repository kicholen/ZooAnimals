#ifndef _TOUCHBLOCK_
#define _TOUCHBLOCK_

#include "oxygine-framework.h"

using namespace oxygine;

DECLARE_SMART(TouchBlock, spTouchBlock);

typedef enum { qsShowed, qsHidden, qsAnimating } QuadState;
// this class is a little bit overkill
class TouchBlock : public Actor
{
public:
	TouchBlock();
	TouchBlock(const Vector2& size);
	~TouchBlock();

	void changeState();

private:
	void show();
	void hide();

	void createTouchQuadIfDoesntExist();
	
	void onShowEnded(Event *ev);
	void onHideEnded(Event *ev);
private:
	spColorRectSprite _touchQuad;
	Vector2 _size;

	QuadState _state;
};

#endif