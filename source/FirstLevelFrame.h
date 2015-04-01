#ifndef _FIRSTLEVELFRAME_
#define _FIRSTLEVELFRAME_

#include "CustomFrame.h"
#include "LandingPageFrame.h"
#include "SpriteSpawner.h"
#include "FlashUtils.h"

using namespace FlashUtils;

DECLARE_SMART(FirstLevelFrame, spFirstLevelFrame);

class FirstLevelFrame : public CustomFrame
{
public:
	FirstLevelFrame();
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
	
	void onCloudOutOfScreen(Event *event);
private:
	bool _areBarnDoorOpen;
	spSoundInstance _previousSoundInstance; 
};

#endif
