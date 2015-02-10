#ifndef _SWIPETRAILELEMENT_
#define _SWIPETRAILELEMENT_

#include "oxygine-framework.h"
#include "FlashUtils.h"
#include "ResolverRadialChaikin.h"
#include "SwipePointsGenerator.h"
#include "GLShaderTriangleStrip.h"

using namespace oxygine;
using namespace FlashUtils;

DECLARE_SMART(SwipeTrailElement, spSwipeTrailElement);

class SwipeTrailElement : public GLShaderTriangleStrip
{
public:
	SwipeTrailElement(int maxInputPoints = 10, float thickness = 30.0f, float endPoint = 9.0f);
	~SwipeTrailElement();

	void setSloppyTime(int timeMs);
protected:
	void doUpdate(const UpdateState &us);
private:
	void startDrag(Vector2 localPosition);
	void onMove(Vector2 localPosition);
	void onTouchEvent(Event *event);
	void startDragAgainIfPlayerTooSloppy(Vector2 &position);

	void resolve();
	int generate(DequeArray<Vector2>& input, int mult);

	// Storage for touch points.
	CapacityDeque<Vector2> _touchPoints;
	DequeArray<Vector2> _simplifiedPoints;
	Vector2 _lastPoint;
	spResolverRadialChaikin _pointsSimplifier;
	spSwipePointsGenerator _swipePointsGenerator;

	int _initialDistance;
	int _minDistance;
	int _sloppyTimeMs;

	bool _pressed;
	int _lastDragTimeMs;
};

#endif