#ifndef _ADDREWARDSACTION_
#define _ADDREWARDSACTION_

#include "IAction.h"
#include "RewardModel.h"

class AddRewardsAction : public IAction
{
public:
	AddRewardsAction(const std::vector<std::string>&  rewards);
	~AddRewardsAction();

	virtual void doAction();
	virtual void complete();

private:
	void parse(spRewardModel model);

	std::vector<std::string> _rewards;

};

#endif