#ifndef _RESOLVERRADIALCHAIKIN_
#define _RESOLVERRADIALCHAIKIN_

#include "FlashUtils.h"

using namespace FlashUtils;

#define ITERATIONS 
#define SIMPLIFY_TOLERANCE 35

DECLARE_SMART(ResolverRadialChaikin, spResolverRadialChaikin);

class ResolverRadialChaikin : public Object
{
public:
	ResolverRadialChaikin();
	~ResolverRadialChaikin();

	void resolve(DequeArray<Vector2>& input, DequeArray<Vector2>& output);
protected:
	void smooth(DequeArray<Vector2>& input, DequeArray<Vector2>& output);
	void simplify(DequeArray<Vector2>& points, float sqTolerance, DequeArray<Vector2>& out);

private:
	float distSq(Vector2 p1, Vector2 p2);

	DequeArray<Vector2> _tempArray;

	int _iterations;
	float _simplifyTolerance;
};

#endif