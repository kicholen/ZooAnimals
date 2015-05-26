#ifndef _CARDNAVIGATOR_
#define _CARDNAVIGATOR_

#include "oxygine-framework.h"
#include "deque"

using namespace oxygine;

DECLARE_SMART(CardNavigator, spCardNavigator);
enum CardNavigatorAlign { cnVertical = 0, cnHorizontal = 1 };
typedef enum CardNavigatorState { cnBlock, cnWait };
/*
*	CardNavigator will automatically be size of added items, changing size of this element will do nothing.
*	Remember to call updateCards.
*	Center of current card will be exactly on position of CardNavigator.
*	One more, don't manipulate position of CardNavigator ^^
*/
class CardNavigator : public Actor
{
public:
	CardNavigator(int alignType = 0, float offsetBetweenCards = 10.0f);
	~CardNavigator();
	
	void addCard(spActor actor);

	/* For proper display, call on cards count modification*/
	void updateCards();

	void animateToNextCard();
	void animateToPreviousCard();
	void animateToCardByIndex(int index);

private:
	void setPositionByIndex(int index, bool shouldAnimate);

	void setChildPosition(spActor actor);
	float getIndexPosition(int index);

	void onTweenEnded(Event *ev);
	void updatePriority();
private:
	int _currentChildIndex;
	int _childrenCount;
	CardNavigatorAlign _alignType;
	CardNavigatorState _state;
	float _offsetBetweenCards;
	deque<spActor> _cards;
};

#endif