#ifndef _SPECTATORSAREA_
#define _SPECTATORSAREA_

#include "oxygine-framework.h"
#include "WalkingSpectator.h"

#define SORT_Z_DELAY 1000

using namespace oxygine;

DECLARE_SMART(SpectatorsArea, spSpectatorsArea);

class SpectatorsArea : public Actor
{
public:
	SpectatorsArea();
	~SpectatorsArea();

	void init(int count);

	void doUpdate(const UpdateState &us);

	void removeSpectators();
	int getSpectatorsInUseCount();

private:
	template<class T> inline const T& max(const T& a, const T& b)
	{
		return b < a ? a : b;
	}

	template<class T> inline const T& min(const T& a, const T& b)
	{
		return b > a ? a : b;
	}

	spWalkingSpectator addSpectator(const std::string &resAnim, float height);

	spWalkingSpectator createSpectator();

	spWalkingSpectator getSpectator();
	spWalkingSpectator getSpectatorFromPool();
	
	void setTmpVectorToRandomPosition(const Vector2& position);

	void addTooltipToSpectator(spActor tooltip, spActor spectator);

	VectorArray<spWalkingSpectator> _spectatorsArray;
	VectorArray<uint> _spectatorsInPool;

	float _lastZSortTime;
	float _radius;
	float _offsetX;
	float _offsetY;
	float _unavailableOffset;
	Vector2 _tmpVector;
};

#endif