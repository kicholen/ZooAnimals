#ifndef _MEMORYFRAME_
#define _MEMORYFRAME_

#include "CustomFrame.h"
#include "SpriteSpawner.h"
#include "FlashUtils.h"
#include "LandingPageFrame.h"
#include "MemoryField.h"
#include "CounterBoxElement.h"

DECLARE_SMART(MemoryFrame, spMemoryFrame);

class MemoryFrame : public CustomFrame
{
public:
	MemoryFrame(const char* whichLevel);
	void selectTransitions();

	Action loop();
protected:
	void onGoBack(Event *event);
	void onShowSliderFrame(Event *event);
	void onTimesUp(Event *event);
	void onCardsScored(Event *event);

	void _postHiding(Event *);
	void _preShowing(Event *);

	void setData();

	void stopPreviousAndPlayNewSound(string soundName);
	int getLevelToLoad();

private:
	spCounterBoxElement _counterBox;
	spMemoryField _field;

	int _currentLevelCardsScored;
	int _maxCards;
	int _totalScore;
	int _levelScoreMultiplier;
	int _level;
	//spSoundInstance _previousSoundInstance; 
	string _whichLevel;
};

#endif