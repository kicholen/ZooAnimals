#ifndef _JUMPOVERFRAME_
#define _JUMPOVERFRAME_

#include "CustomFrame.h"
#include "Box2dContainer.h"

DECLARE_SMART(JumpOverFrame, spJumpOverFrame);

class JumpOverFrame : public CustomFrame
{
public:
	JumpOverFrame(std::string difficulty);
	void selectTransitions();

	Action loop();

protected:
	void _postHiding(Event *);
	void _preShowing(Event *);

	void onFinished(Event *event);
	void onTimesUp(Event *event);

	void setData();

	void showHideDebug();
	void pauseWorld(Event *ev);
	void addQuad();

private:
	spBox2dContainer _world;
	std::string _difficulty;
};

#endif