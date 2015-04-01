#ifndef _CONNECTDOTSFRAME_
#define _CONNECTDOTSFRAME_

#include "CustomFrame.h"
#include "FlashUtils.h"
#include "ConnectDotsField.h"
#include "SharedResources.h"
#include "CounterBoxElement.h"

DECLARE_SMART(ConnectDotsFrame, spConnectDotsFrame);

class ConnectDotsFrame : public CustomFrame
{
public:
	ConnectDotsFrame();
	void selectTransitions();

	Action loop();

protected:
	void _postHiding(Event *);
	void _preShowing(Event *);

	void onFinished(Event *event);
	void onTimesUp(Event *event);

	void setData();
private:
	spConnectDotsField _field;
	int _previousAnimal;
	int _totalScore;
	spCounterBoxElement _counterBox;
	//spSoundInstance _previousSoundInstance; 
};

#endif