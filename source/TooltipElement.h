#ifndef _TOOLTIPELEMENT_
#define _TOOLTIPELEMENT_

#include "oxygine-framework.h"

using namespace oxygine;

DECLARE_SMART(TooltipElement, spTooltipElement);

class TooltipElement : public Actor
{
public:
	TooltipElement(const Vector2& size, const string& background, const string& spriteName, int lockitId = 0);
	~TooltipElement();

	void show();
	void hide(bool shouldDetach);

private:
	void setData(const string& background, const string& spriteName, int lockitId);
	void createBackground(const string& background);
	bool createTextfieldIfNeeded(int lockitId);
	void createSprite(const string& spriteName);

	void onTweenEnded(Event *ev);
private:
	spSprite _background;
	spTextField _textField;
	spSprite _sprite;
};

#endif