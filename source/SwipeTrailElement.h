#ifndef _SWIPETRAILELEMENT_
#define _SWIPETRAILELEMENT_

#include "oxygine-framework.h"
#include "FlashUtils.h"
#include "ResolverRadialChaikin.h"

using namespace oxygine;
using namespace FlashUtils;

namespace oxygine
{
	class ShaderProgramGL;
}

DECLARE_SMART(SwipeTrailElement, spSwipeTrailElement);

class SwipeTrailElement : public Actor
{
public:
	SwipeTrailElement(int maxInputPoints = 10);
	~SwipeTrailElement();

protected:
	void doUpdate(const UpdateState &us);
	void doRender(const RenderState &rs);

private:
	void startDrag(Vector2 localPosition);
	void onMove(Vector2 localPosition);

	void resolve();
	/*
	*	Touch event handlers
	*/
	void onTouchEvent(Event *event);

	int generate(DequeArray<Vector2>& input, int mult);

	// Drawing stuff
	void drawAllLines();
	void drawPrimitives(bool drawTriangles, bool drawLines, int count, bool otherColor);
	void drawSegment(const Vector2& p1, const Vector2& p2, bool otherColor, bool isTriangle);

	// Storage for touch points.
	CapacityDeque<Vector2> _touchPoints;
	DequeArray<Vector2> _simplifiedPoints;
	Vector2 _lastPoint;
	spResolverRadialChaikin _pointsSimplifier;

	int _initialDistance;
	int _minDistance;

	static const int MAX_VERTICES = 256;
	Vector2 mVertices[MAX_VERTICES];
	ShaderProgramGL *_program;
	bool _pressed;

	VectorArray<Vector2> _triangles;
	VectorArray<Vector2> _texCoord;
	Vector2 _perp;

	int _batchSize;
	float _thickness;
	float _endcap;

};

#endif