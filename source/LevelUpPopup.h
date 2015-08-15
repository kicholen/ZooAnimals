#ifndef _LEVELUPPOPUP_
#define _LEVELUPPOPUP_

#include "CustomFrame.h"

DECLARE_SMART(LevelUpPopup, spLevelUpPopup);

class LevelUpPopup : public CustomFrame
{
public:
	LevelUpPopup();
	~LevelUpPopup();
	void selectTransitions();

	Action loop();

protected:
	void _postHiding(Event *);
	void _preShowing(Event *);

private:
	void setData();

	void createPopupBackground();
	void createTitleTextfield();
	void createRewards();

private:
	spSprite _rewardList;
};

#endif