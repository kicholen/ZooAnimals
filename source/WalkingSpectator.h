#ifndef _WALKINGSPECTATOR_
#define _WALKINGSPECTATOR_

#include "oxygine-framework.h"
#include "FlashUtils.h"

using namespace oxygine;
using namespace FlashUtils;

DECLARE_SMART(WalkingSpectator, spWalkingSpectator);

typedef enum { wsNotUsed, wsJumping, wsWaiting } WalkingSpectatorState;

class WalkingSpectator : public Sprite
{
public:
	class WalkingSpectatorEvent : public Event {
	public:
		enum EV {
			DIE_EVENT = makefourcc('W', 'S', 'E', 'E')
		};

		uint spectatorNumber;

		WalkingSpectatorEvent(uint number) :Event(DIE_EVENT), spectatorNumber(number) {}
	};

	WalkingSpectator(const VectorArray<Vector2>& trackPoints, uint number);
	~WalkingSpectator();


	void revive(const VectorArray<Vector2>& trackPoints);

private:
	void die();
	void tryToAnimateToNextPosition();
	Vector2 getNextTrackPoint();
	void faceAccordingToMovement(float destX);

	void onTweenEnded(Event *ev);
private:
	VectorArray<Vector2> _trackPoints;
	uint _number;
	bool _dead;

	float _lastX;
	float _lastY;
};

#endif