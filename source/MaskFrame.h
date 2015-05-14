#ifndef _MASKFRAME_
#define _MASKFRAME_

#include "Main.h"
#include "LandingPageFrame.h"
#include "SpriteSpawner.h"
#include "FlashUtils.h"
#include "ParticleEmitter.h"
#include "InputText.h"

using namespace FlashUtils;

DECLARE_SMART(MaskFrame, spMaskFrame);

class MaskFrame : public CustomFrame
{
public:
	MaskFrame();

	void selectTransitions();

	Action loop();
protected:
	void _postHiding(Event *);
	void _preShowing(Event *);
	string getBackground();
	void _clicked(Event *event);
	void onClicked(string id, timeMS duration, timeMS delay);
	void swapAnimation();
	
	void onGoBack(Event *event);
	void onShowSliderFrame(Event *event);
	void onClick(Event *ev);
	void onComplete(Event *ev);

	void setData();

	void addDraggableSprite(string spriteName, Vector2 anchorPoint, float scaleX, float scaleY, float positionX, float positionY, int priorityZ, bool fromEditResources);
	void onSpawnSprite(Event *event);

private:
	int _count;
	spTween _tweenSprite;
	spTween _tweenLetter;
	spSprite _animatedSprite;
	spSprite _animatedLetter;
	spActor _spritesHolder;

	spInputText _inputTextfield;
	spTextActor _currentTextfield;
};

#endif