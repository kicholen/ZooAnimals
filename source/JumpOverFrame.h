#ifndef _JUMPOVERFRAME_
#define _JUMPOVERFRAME_

#include "CustomFrame.h"

DECLARE_SMART(JumpOverFrame, spJumpOverFrame);

class JumpOverFrame : public CustomFrame
{
public:
	JumpOverFrame();
	void selectTransitions();

	Action loop();

protected:
	void _postHiding(Event *);
	void _preShowing(Event *);

	void onFinished(Event *event);
	void onTimesUp(Event *event);

	void setData();

	void showHideDebug();
	void pauseWorld();
	void addQuad();
};

#endif