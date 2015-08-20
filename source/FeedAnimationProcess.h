#ifndef _FEEDANIMATIONPROCESS_
#define _FEEDANIMATIONPROCESS_

#include "ProcessSlave.h"
#include "AnimalFarmField.h"
#include "WalkingSpectator.h"

class FeedAnimationProcess : public ProcessSlave
{
public:
	FeedAnimationProcess(spAnimalFarmField farm, spSlidingActor container);
	~FeedAnimationProcess();

	virtual void process();
	virtual bool completed();

private:
	void createFeederMan(float spectatorsHeight);
	spSprite createBoat(float boatHeight);

	void moveToNextPart(Event *event);
private:
	spAnimalFarmField _farm;
	spSlidingActor _container;
	
	int _animalNumber;
	spWalkingSpectator _feederMan;
	spSprite _boat;
	bool _isWater;
	bool _isBoatAttachedToFeederMan;
};

#endif