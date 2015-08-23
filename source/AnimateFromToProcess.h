#ifndef _ANIMATEFROMTOPROCESS_
#define _ANIMATEFROMTOPROCESS_

#include "ProcessSlave.h"

class AnimateFromToProcess : public ProcessSlave
{
public:
	AnimateFromToProcess(const std::string& resource, spActor parent, const Vector2& from, const Vector2& to);
	~AnimateFromToProcess();

	virtual void process();
	virtual bool completed();

private:
	spSprite createSprite(const std::string& resource);
	void onAnimationCompleted(Event *event);

private:
	spSprite _sprite;
	Vector2 _to;
};

#endif