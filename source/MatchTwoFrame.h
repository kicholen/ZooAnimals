#ifndef _MATCHTWOFRAME_
#define _MATCHTWOFRAME_

#include "CustomFrame.h"
#include "FlashUtils.h"
#include "MatchTwoField.h"

DECLARE_SMART(MatchTwoFrame, spMatchTwoFrame);

class MatchTwoFrame : public CustomFrame
{
public:
	MatchTwoFrame(void);
	void selectTransitions();

	Action loop();

protected:
	void _postHiding(Event *);
	void _preShowing(Event *);

	void onFinished(Event *event);

	void setData();
private:
	spMatchTwoField _field;

	//spSoundInstance _previousSoundInstance; 
};

#endif