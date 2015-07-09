#ifndef _TOOLTIPELEMENT_
#define _TOOLTIPELEMENT_

#include "oxygine-framework.h"

using namespace oxygine;

DECLARE_SMART(TooltipElement, spTooltipElement);

class TooltipElement : public Actor
{
public:
	TooltipElement(const Vector2& size, const std::string& background, const std::string& spriteName, int lockitId = 0);
	~TooltipElement();

	void showForTime(int ms);
	void show();
	void hide(bool shouldDetach, int delay = 0);

private:
	void setData(const std::string& background, const std::string& spriteName, int lockitId);
	void createBackground(const std::string& background);
	bool createTextfieldIfNeeded(int lockitId);
	void createSprite(const std::string& spriteName);

	void onTweenEnded(Event *ev);
private:
	spSprite _background;
	spTextField _textField;
	spSprite _sprite;
};

#endif