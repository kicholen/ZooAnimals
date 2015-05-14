#ifndef _PRESENTANIMALSFRAME_
#define _PRESENTANIMALSFRAME_

#include "CustomFrame.h"
#include "LandingPageFrame.h"
#include "SpriteSpawner.h"
#include "FlashUtils.h"
#include "AnimalModel.h"

using namespace FlashUtils;

DECLARE_SMART(PresentAnimalsFrame, spPresentAnimalsFrame);

class PresentAnimalsFrame : public CustomFrame
{
public:
	PresentAnimalsFrame(spAnimalModel model);
	void selectTransitions();

	Action loop();
protected:
	void onGoBack(Event *event);
	void onShowSliderFrame(Event *event);
	void _postHiding(Event *);
	void _preShowing(Event *);

	void setData();

private:
	spAnimalModel _model;
};

#endif