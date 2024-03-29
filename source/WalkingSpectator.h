#ifndef _WALKINGSPECTATOR_
#define _WALKINGSPECTATOR_

#include "oxygine-framework.h"
#include "FlashUtils.h"

using namespace oxygine;
using namespace FlashUtils;

DECLARE_SMART(WalkingSpectator, spWalkingSpectator);

typedef enum { wsDead, wsMoving, wsWaiting } WalkingSpectatorState;

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
	WalkingSpectator();
	~WalkingSpectator();

	void revive(const VectorArray<Vector2>& trackPoints);
	void setFaceAccordingToMovement(float destX);
	void animateMoveToPosition(const Vector2& position);
	void makeSpriteAsChild();

	uint getNumber() {
		return _number;
	}

	void setNumber(uint value) {
		_number = value;
	}

protected:
	void doUpdate(const UpdateState &us);
	void doVelocity(float dt, const Vector2& destPosition);

private:
	void die();
	void tryToAnimateToNextPosition();
	Vector2 getNextTrackPoint();
	void setVelocityByNextPoint();
	void checkPositionsByNextPoint();

	void onAnimationEnded(Event *ev);

private:
	WalkingSpectatorState _state;
	VectorArray<Vector2> _trackPoints;
	uint _number;

	Vector2 _velocity;
	bool _isProperX;
	bool _isProperY;
};

#endif