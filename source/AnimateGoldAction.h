#ifndef _ANIMATEGOLDACTION_
#define _ANIMATEGOLDACTION_

#include "IAction.h"

DECLARE_SMART(AnimateGoldAction, spAnimateGoldAction);

class AnimateGoldAction : public IAction
{
public:
	AnimateGoldAction(spActor parent, int goldAmount, const Vector2& startPosition, const Vector2& endPosition);
	~AnimateGoldAction();

	virtual void doAction();
	virtual void complete();

private:
	spSprite createGoldSprite(const Vector2& startPosition);
	float getScaleFactor();

	void onAnimationCompleted(Event *event);
	void onCoinClicked(Event *event);
private:
	spSprite _gold;
	spActor _parent;
	int _goldAmount;
	Vector2 _startPosition;
	Vector2 _endPosition;
};

#endif