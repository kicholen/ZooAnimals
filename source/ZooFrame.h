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
	ZooFrame(const string& regionName);
	void selectTransitions();

	Action loop();

protected:
	void _postHiding(Event *);
	void _preShowing(Event *);

	void onFinished(Event *event);
	void onGameChosen(Event *event);

	void setData();
private:
//	spAnimalFarmField _field;
	VectorArray<spAnimalFarmField> _farmArray;
	string _region;

	spRotatingContainer _rotatingContainer;
	bool _shouldRemoveTiles;
};

#endif