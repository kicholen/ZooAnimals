#ifndef _ACHIEVEMENTFRAME_
#define _ACHIEVEMENTFRAME_

#define OFFSET_ACHIEVEMENT 20

#include "CustomFrame.h"

DECLARE_SMART(AchievementFrame, spAchievementFrame);

class AchievementFrame : public CustomFrame
{
public:
	AchievementFrame();
	~AchievementFrame();
	void selectTransitions();

	Action loop();

protected:
	void _postHiding(Event *);
	void _preShowing(Event *);

private:
	void setData();

	spTextField createTitleTextfield();
	void createPopupBackground();
	void createAchievementsList(spTextField title);
};

#endif