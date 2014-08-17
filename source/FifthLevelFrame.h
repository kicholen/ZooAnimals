#ifndef _FIFTHLEVELFRAME_
#define _FIFTHLEVELFRAME_

#include "CustomFrame.h"
#include "LandingPageFrame.h"
#include "SpriteSpawner.h"
#include "FlashUtils.h"

using namespace FlashUtils;

DECLARE_SMART(FifthLevelFrame, spFifthLevelFrame);
class FifthLevelFrame : public CustomFrame
{
public:
	FifthLevelFrame();
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
	bool _wasTailSpreaded;
	spSoundInstance _previousSoundInstance; 
};

#endif