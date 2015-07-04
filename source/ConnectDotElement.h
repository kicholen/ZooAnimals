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
	ConnectDotElement();

	void addText(std::string number);

	void playAnimation();
	void stopAnimation();
protected:
	void createDotSprite();
	void setDotSprite();

private:
	spSprite _dotSprite;
	float _baseScaleX;
};

#endif