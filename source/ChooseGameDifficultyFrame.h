#ifndef _CHOOSEGAMEDIFFICULTYFRAME_
#define _CHOOSEGAMEDIFIICULTYFRAME_

#include "CustomFrame.h"
#include "MemoryField.h"
#include "CounterBoxElement.h"

DECLARE_SMART(ChooseGameDifficultyFrame, spChooseGameDifficultyFrame);

class ChooseGameDifficultyFrame : public CustomFrame
{
public:
	ChooseGameDifficultyFrame(const string& gameName);
	void selectTransitions();

	Action loop();
protected:
	void _postHiding(Event *);
	void _preShowing(Event *);

	void setData();

	void startGame(const string& difficulty);
private:
	string _gameName;
};

#endif