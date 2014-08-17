#ifndef _CHOOSEMEMORYDIFFICULTYFRAME_
#define _CHOOSEMEMORYDIFFICULTYFRAME_

#include "CustomFrame.h"
#include "MemoryField.h"
#include "CounterBoxElement.h"

DECLARE_SMART(ChooseMemoryDifficultyFrame, spChooseMemoryDifficultyFrame);

class ChooseMemoryDifficultyFrame : public CustomFrame
{
public:
	ChooseMemoryDifficultyFrame();
	void selectTransitions();

	Action loop();
protected:
	void _postHiding(Event *);
	void _preShowing(Event *);

	void setData();
};

#endif