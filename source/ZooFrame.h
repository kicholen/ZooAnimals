#ifndef _ZOOFRAME_
#define _ZOOFRAME_

#include "CustomFrame.h"
#include "FlashUtils.h"
#include "AnimalFarmField.h"
#include "RotatingContainer.h"

DECLARE_SMART(ZooFrame, spZooFrame);

class ZooFrame : public CustomFrame
{
public:
	ZooFrame();
	void selectTransitions();

	Action loop();

protected:
	void _postHiding(Event *);
	void _preShowing(Event *);

	void onFinished(Event *event);

	void setData();
private:
//	spAnimalFarmField _field;

	spRotatingContainer _rotatingContainer;
};

#endif