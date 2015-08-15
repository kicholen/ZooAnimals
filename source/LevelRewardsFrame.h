#ifndef _LEVELREWARDFRAME_
#define _LEVELREWARDSFRAME_

#include "CustomFrame.h"

#define OFFSET 10

DECLARE_SMART(LevelRewardsFrame, spLevelRewardsFrame);

class LevelRewardsFrame : public CustomFrame
{
public:
	LevelRewardsFrame();
	~LevelRewardsFrame();

	Action loop();

	void selectTransitions();
protected:
	void _postHiding(Event *);
	void _preShowing(Event *);

	void setData();

private:
	spSlidingActor _rotatingContainer;
};

#endif