#ifndef _SIXTHLEVELFRAME_
#define _SIXTHLEVELFRAME_

#include "CustomFrame.h"
#include "LandingPageFrame.h"
#include "SpriteSpawner.h"
#include "FlashUtils.h"

using namespace FlashUtils;

DECLARE_SMART(SixthLevelFrame, spSixthLevelFrame);

class SixthLevelFrame : public CustomFrame
{
public:
	SixthLevelFrame();
	void selectTransitions();

	Action loop();
protected:
	void onGoBack(Event *event);
	void onShowSliderFrame(Event *event);
	void _postHiding(Event *);
	void _preShowing(Event *);

	void setData();

	void stopPreviousAndPlayNewSound(string soundName);
	void addDraggableSprite(string spriteName, Vector2 anchorPoint, float scaleX, float scaleY, float positionX, float positionY, int priorityZ);
	void addTouchEventListenerIfNeeded(spSprite sprite);
	void onAnimatableObjectTap(Event *event);
private:
	spSoundInstance _previousSoundInstance;
};

#endif