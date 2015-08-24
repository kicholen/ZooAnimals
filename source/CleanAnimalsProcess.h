#ifndef _CLEANANIMALSPROCESS_
#define _CLEANANIMALSPROCESS_

#include "ProcessSlave.h"
#include "AnimalFarmField.h"
#include "DraggableSprite.h"

class CleanAnimalsProcess : public ProcessSlave
{
public:
	CleanAnimalsProcess(spAnimalFarmField farm, spButton source, Event *event);
	~CleanAnimalsProcess();

	virtual void process();
	virtual bool completed();

private:
	void createCloudSprite();

	void spriteTouchUp(Event *event);

	void onCloudBack(Event *event);
private:
	spAnimalFarmField _farm;
	spButton _source;

	spDraggableSprite _cloud;

	bool _isWater;
	int _animalsCleaned;
	float _cloudHeight;
};

#endif