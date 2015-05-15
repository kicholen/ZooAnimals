#ifndef _CARDNAVIGATOR_
#define _CARDNAVIGATOR_

#include "oxygine-framework.h"

using namespace oxygine;

DECLARE_SMART(CardNavigator, spCardNavigator);
enum CardNavigatorAlign { cnVertical = 0, cnHorizontal = 1 };

/*
*	CardNavigator will automatically be size of added items, changing size of this element will do nothing.
*	Remember to call updateCards.
*	Center of current card will be exactly on position of CardNavigator.
*	One more, don't manipulate position of CardNavigator ^^
*/
class CardNavigator : public Actor
{
public:
	CardNavigator(int alignType = 0, float offsetBetweenCards = 0.05f);
	~CardNavigator();
	
	void addCard(spActor actor);

	/* For proper display, call on cards count modification*/
	void updateCards();

	void animateToNextCard();
	void animateToPreviousCard();
	void animateToCardByIndex(int index);

private:
	void animateToIndex(int index);

	void setChildPosition(spActor actor);

	int _currentChildIndex;
	int _childrenCount;
	CardNavigatorAlign _alignType;
	float _offsetBetweenCards;
};

#endif