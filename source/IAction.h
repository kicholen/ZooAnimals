#ifndef _IACTION_
#define _IACTION_

#include "oxygine-framework.h"

using namespace oxygine;

class IAction : public Actor
{
public:
	IAction();
	~IAction();

	virtual void doAction();
	virtual void complete();
};

#endif