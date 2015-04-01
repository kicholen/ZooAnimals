#ifndef _SWIPEPOINTSGENERATOR_
#define _SWIPEPOINTSGENERATOR_

#include "FlashUtils.h"

using namespace FlashUtils;

DECLARE_SMART(SwipePointsGenerator, spSwipePointsGenerator);

class SwipePointsGenerator : public Object
{
public:
	SwipePointsGenerator(float thickness, float endcap);
	~SwipePointsGenerator();

	int generate(DequeArray<Vector2>& input, VectorArray<Vector2> &_triangles, int mult);

private:
	Vector2 _perp;
	float _thickness;
	float _endcap;
};

#endif

