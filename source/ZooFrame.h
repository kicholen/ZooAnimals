#ifndef _ZOOFRAME_
#define _ZOOFRAME_

#include "CustomFrame.h"
#include "FlashUtils.h"
#include "AnimalFarmField.h"

DECLARE_SMART(ZooFrame, spZooFrame);

class ZooFrame : public CustomFrame
{
public:
	ZooFrame(const std::string& regionName);
	void selectTransitions();

	Action loop();

protected:
	void _postHiding(Event *);
	void _preShowing(Event *);

	void onFinished(Event *event);
	void onGameChosen(Event *event);
	void onAnimalFed(Event *event);

	void onCleaningProgress(Event *event);

	void setData();

	spAnimalFarmField getFarmFieldByModel(spAnimalModel model);
private:
	VectorArray<spAnimalFarmField> _farmArray;
	std::string _region;

	spSlidingActor _rotatingContainer;
};

#endif