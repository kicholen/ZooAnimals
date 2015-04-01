#ifndef _FOURTHLEVELFRAME_
#define _FOURTHLEVELFRAME_

#include "CustomFrame.h"
#include "LandingPageFrame.h"
#include "SpriteSpawner.h"
#include "FlashUtils.h"

using namespace FlashUtils;

DECLARE_SMART(FourthLevelFrame, spFourthLevelFrame);

class FourthLevelFrame : public CustomFrame
{
public:
	FourthLevelFrame ();
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
	void onSharkOutOfScreen(Event *event);
private:
	bool wasWhaleAnimated;
	bool wasKrzakAnimated;
	spSoundInstance _previousSoundInstance;

	float _sharkBaseScaleY;
};

#endif