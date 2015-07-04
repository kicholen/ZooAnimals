#ifndef _SPECTATORSPAWNER_
#define _SPECTATORSPAWNER_

#include "Actor.h"
#include "ParticlesContainer.h"
#include "FlashUtils.h"
#include "SpectatorsContainer.h"

using namespace FlashUtils;
using namespace oxygine;

DECLARE_SMART(SpectatorSpawner, spSpectatorSpawner);

class SpectatorSpawner : public Actor
{
public:
	SpectatorSpawner(int spectatorsOnScreen, float scaleSpectatorsToHeight);
	~SpectatorSpawner();

	void addResAnim(const std::string &resAnim, bool shouldClear = false);
	void addTrack(const VectorArray<Vector2>& track, bool shouldClear = false);

protected:
	virtual void doUpdate(const UpdateState &us);
	void spawnSpectator();

private:
	VectorArray<std::string> _resAnims;
	VectorArray<VectorArray<Vector2> > _tracks;
	int _spectatorsOnScreen;
	float _frameTime;

	float _spectatorsHeight;

	spSpectatorsContainer _container;
};


#endif
