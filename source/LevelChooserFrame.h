#ifndef _LEVELCHOOSERFRAME_
#define _LEVELCHOOSERFRAME_

#include "Main.h"
#include "CustomFrame.h"
#include "PresentAnimalsFrame.h"

DECLARE_SMART(LevelChooserFrame, spLevelChooserFrame);

class LevelChooserFrame : public CustomFrame
{
public:
	LevelChooserFrame();
	Action loop();

	void setData();

protected:
	void selectTransitions();
	void _postHiding(Event *);
	void _preShowing(Event *);
	void onPurchaseCompleted(Event *);
	void onPostSuccess(Event *);
	void onTweenDispatcherDone(Event *event);

	void addRotatingContainer();
private:
	spSlidingActor _rotatingContainer;
	int _counter;
};

#endif