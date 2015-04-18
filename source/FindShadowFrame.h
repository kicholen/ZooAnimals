#ifndef _FINDSHADOWFRAME_
#define _FINDSHADOWFRAME_

#include "CustomFrame.h"
#include "FlashUtils.h"
#include "FindShadowField.h"

DECLARE_SMART(FindShadowFrame, spFindShadowFrame);

class FindShadowFrame : public CustomFrame
{
public:
	FindShadowFrame(const string& difficulty);
	void selectTransitions();

	Action loop();

protected:
	void _postHiding(Event *);
	void _preShowing(Event *);

	void onFinished(Event *event);

	void setData();
private:
	spFindShadowField _field;
	string _difficulty;
};

#endif