#ifndef _LANDINGPAGEFRAME_
#define _LANDINGPAGEFRAME_

#include "CustomFrame.h"
#include "Main.h"
#include "BuyItemPopup.h"
#include "WallPostMessagePopup.h"
#include "SlideFrame.h"
#include "MaskFrame.h"
#include "LevelChooserFrame.h"

DECLARE_SMART(LandingPageFrame, spLandingPageFrame);
class LandingPageFrame : public CustomFrame
{
public:
	LandingPageFrame();

	Action loop();
protected:
	void _postHiding(Event *);
	void _preShowing(Event *);
	void selectTransitions();
	void setData();
	void playLoopedMusic();
	void onMusicDone(Event *event);

private:
	void createGlobalElementsIfDoesntExist();

};

#endif