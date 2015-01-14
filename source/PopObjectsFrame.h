#ifndef _POPOBJECTSFRAME_
#define _POPOBJECTSFRAME_

#include "CustomFrame.h"
#include "ParticleEmitter.h"

DECLARE_SMART(PopObjectsFrame, spPopObjectsFrame);

class PopObjectsFrame : public CustomFrame
{
public:
	PopObjectsFrame();
	~PopObjectsFrame();
	void selectTransitions();

	Action loop();

protected:
	void _postHiding(Event *);
	void _preShowing(Event *);

private:
	void onFinished(Event *event);

	void setData();

	//spPopObjectsField _popField;
};

#endif