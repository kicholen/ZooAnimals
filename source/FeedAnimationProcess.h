#ifndef _FEEDANIMATIONPROCESS_
#define _FEEDANIMATIONPROCESS_

#include "ProcessSlave.h"
#include "AnimalFarmField.h"
#include "WalkingSpectator.h"

class FeedAnimationProcess : public ProcessSlave
{
public:
	FeedAnimationProcess(spAnimalFarmField farm, spWalkingSpectator spectator);
	~FeedAnimationProcess();

	virtual void process();
	virtual bool completed();

private:
	spAnimalFarmField _farm;
	spWalkingSpectator _spectator;
};

#endif