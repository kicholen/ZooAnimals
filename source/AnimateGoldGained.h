#ifndef _ANIMATEGOLDGAINED_
#define _ANIMATEGOLDGAINED_

#include "ProcessSlave.h"

class AnimateGoldGained : public ProcessSlave
{
public:
	AnimateGoldGained(spActor parent, int goldAmount, const Vector2& startPosition, const Vector2& endPosition);
	~AnimateGoldGained();

	virtual void process();
	virtual bool completed();

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