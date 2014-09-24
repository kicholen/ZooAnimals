#ifndef _ROTATINGCONTAINER2D_
#define _ROTATINGCONTAINER2D_

#include "oxygine_include.h"
#include "Actor.h"

using namespace oxygine;

DECLARE_SMART(RotatingContainer2d, spRotatingContainer2d);

class RotatingContainer2d : public Actor
{
public:
	RotatingContainer2d(Vector2 size);
	~RotatingContainer2d();

	void addMainChild(spActor child);
	void addSubChild(spActor child);
	void clear();

protected:
	void doUpdate(const UpdateState &us);

private:
	void invalidateAll();
	Vector2 getSubChildPosition(float angle, Vector2 center, float distanceFromCenter);
	void scaleChild(spActor sprite, float scaleInParent);
private:
	bool _needsRevalidate;
	spActor _mainChild;
	spActor _wheel;

	float _mainChildScale;
	float _centerSubChildrenAt;
};

#endif