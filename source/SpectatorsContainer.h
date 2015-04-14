#ifndef _SPECTATORSCONTAINER_
#define _SPECTATORSCONTAINER_

#include "FlashUtils.h"
#include "Particle.h"
#include "WalkingSpectator.h"

using namespace FlashUtils;

DECLARE_SMART(SpectatorsContainer, spSpectatorsContainer);

class SpectatorsContainer : public Actor
{
public:
	SpectatorsContainer();
	~SpectatorsContainer();

	void removeSpectators();

	spWalkingSpectator addSpectator(const VectorArray<Vector2>& trackPoints, const string &resAnim, float height);

	int getSpectatorsInUseCount();
private:
	spWalkingSpectator createSpectator(const VectorArray<Vector2>& trackPoints);

	spWalkingSpectator getSpectator(const VectorArray<Vector2>& trackPoints);
	spWalkingSpectator getSpectatorFromPool(const VectorArray<Vector2>& trackPoints);
	void addEventListenersToSpectator(spWalkingSpectator spectator);

	//void onParticleEvent(Event *event);
	void onSpectatorDie(Event *ev);

	VectorArray<spWalkingSpectator> _spectatorsArray;
	VectorArray<uint> _spectatorsInPool;
};

#endif