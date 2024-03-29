#ifndef _MATCHTWOFIELD_
#define _MATCHTWOFIELD_

#include "oxygine-framework.h"
#include "MatchTwoDraggable.h"
#include "MatchTwoSlot.h"
#include "FlashUtils.h"

using namespace oxygine;
using namespace FlashUtils;

DECLARE_SMART(MatchTwoField, spMatchTwoField);

typedef enum {mtWaiting, mtDragging, mtCompleted, mtAnimating} MatchTwoFieldState;

class MatchTwoField : public Actor
{
public:
	class MatchTwoFieldEvent: public Event {
	public:
		enum EV {
			FINISHED = makefourcc('M', 'T', 'F', 'F')
		};

		MatchTwoFieldEvent(EV ev):Event(ev) {}
	};

	MatchTwoField(Vector2 size, uint numberOfMatches, uint pairsFromContent);
	~MatchTwoField();

	void restart(uint numberOfMatches, uint pairsFromContent);
private:
	void spriteTouchDown(Event *event);
	void spriteTouchUp(Event *event);
	void touchUp(Event *event);

	void animationEndCallback(Event *event);
	void onBackAnimationFinished(Event *event);
	void onProperSlotFound(Vector2 animateTo);
	void onSlotFoundAnimationCompleted(Event *event);

	void fillField(uint pairsFromContent);
	void createSpotTakenVector(uint numberOfMatches);
	uint getRandomFreeSpot();
	Vector2 getSlotPosition(int i);
	Vector2 getSlotSize();
	void hidePreviousMatchesIfNecessary();

	void createDraggableSprite(spMatchTwoSlot slot, std::string name, Vector2 slotScale, std::string spriteName);
private:
	MatchTwoFieldState _state;
	uint _numberOfMatches;
	uint _animatedCount;
	uint _numberOfMatchesFound;
	uint _previousNumberOfMatches;

	Vector2 _baseDraggingScale;
	spMatchTwoDraggable _dragging;

	VectorArray<bool> _isSpotTaken;
};

#endif