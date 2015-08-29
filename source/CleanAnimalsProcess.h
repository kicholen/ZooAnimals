#ifndef _CLEANANIMALSPROCESS_
#define _CLEANANIMALSPROCESS_

#include "ProcessSlave.h"
#include "AnimalFarmField.h"
#include "DraggableSprite.h"

#define DROPLETS_PER_SECOND 5

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
	void spawnDroplets();
	void onCloudBack(Event *event);
	void updateDroplet(const UpdateState &us);

	bool isFarmCleaned();
private:
	std::vector<Vector2> _positions;
	spAnimalFarmField _farm;
	spButton _source;

	spDraggableSprite _cloud;

	VectorArray<spSprite> _droplets;
	UpdateCallback _updateCallback;
	int _lastDropletIndex;
	float _timeSinceLastDroplet;

	bool _isWater;
	int _count;
	float _cloudHeight;
};

#endif