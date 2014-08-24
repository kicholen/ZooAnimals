#ifndef _CONNECTDOTELEMENT_
#define _CONNECTDOTELEMENT_

#include "oxygine-framework.h"

#define DOT_SIZE_X 10
#define DOT_SIZE_Y 10

using namespace oxygine;

DECLARE_SMART(ConnectDotElement, spConnectDotElement);

class ConnectDotElement : public Actor
{
public:
	ConnectDotElement(string number);

	void setBaseScale(Vector2 scale);

	void playAnimation();
	void stopAnimation();
protected:
	void createDotSprite();
	void setDotSprite();
	void createText(string number);

private:
	spSprite _dotSprite;
	Vector2 _baseScale;
};

#endif