#ifndef _ENDGAMEACTION_
#define _ENDGAMEACTION_

#include "IAction.h"

DECLARE_SMART(EndGameAction, spEndGameAction);

class EndGameAction : public IAction
{
public:
	EndGameAction();
	~EndGameAction();

	virtual void doAction();
	virtual void complete();
};

#endif