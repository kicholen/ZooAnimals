#ifndef _SEVENTHLEVELFRAME_
#define _SEVENTHLEVELFRAME_

#include "CustomFrame.h"
#include "LandingPageFrame.h"
#include "SpriteSpawner.h"
#include "FlashUtils.h"

using namespace FlashUtils;

DECLARE_SMART(SeventhLevelFrame, spSeventhLevelFrame);

class SeventhLevelFrame : public CustomFrame
{
public:
	SeventhLevelFrame(void);
	void selectTransitions();

	Action loop();
protected:
	void onGoBack(Event *event);
	void onShowSliderFrame(Event *event);
	void _postHiding(Event *);
	void _preShowing(Event *);

	void setData();

	void addDraggableSprite(string spriteName, Vector2 anchorPoint, float scaleX, float scaleY, float positionX, float positionY, int priorityZ);
};

#endif