#ifndef _MEMORYFIELD_
#define _MEMORYFIELD_

#include "MemoryCard.h"
#include "oxygine-framework.h"

using namespace oxygine;

DECLARE_SMART(MemoryField, spMemoryField);

typedef enum {mfWaiting, mfFirstFound, mfSecondFound, mfFalling, mfChecking, mfBackSwaping} MemoryFieldState;

class MemoryField : public Actor
{
public:
	class MemoryFieldEvent: public Event {
	public:
		enum EV
		{
			CARDS_SCORED = makefourcc('M', 'F', 'C', 'S')
		};

		MemoryFieldEvent(EV ev):Event(ev) {}
	};

	MemoryFieldState getState() {
		return _state;
	}
	MemoryField(Point numberOfFields);

	Vector2	getCellPosition(int i, int j);
	Point getCellIndex(Vector2 position);

	void cardClick(Event *event);
	bool swap(spMemoryCard firstCard, spMemoryCard secondCard, bool skip_animation = false);
	bool backSwap();

	void fillField(bool first_time = false);
	void animateField();
private:
	MemoryFieldState _state;
	Point _numberOfFields;
	
	Point _firstCardIndex;
	Point _secondCardIndex;
	int droped_count;
	int animated_count;

	void generateRandomCardsArrayByType(std::string *buffer);
	int rotateCard(Point ind, bool show);

	bool areSelectedCardsPair(spMemoryCard first, spMemoryCard second);

	int checkCard(Point ind, bool donotscore=false);

	void endSwapCallback(Event *ev);
	void endAnimationCallback(Event *ev);
	void dropEndCallback(Event *ev);

	void dispatchCardsScored();
};

#endif