#ifndef _REWARDSMANAGER_
#define _REWARDSMANAGER_

#include "oxygine-framework.h"
#include "RewardModel.h"

using namespace oxygine;

DECLARE_SMART(RewardsManager, spRewardsManager);

class RewardsManager : public Object
{
public:
	static RewardsManager instance;

	RewardsManager();
	~RewardsManager();

	void init();

	spRewardModel getReward(const std::string& rewardName);
	spRewardModel getReward(int rewardNum);
private:
	void parseRewards();

private:
	std::map <std::string, spRewardModel > _rewardsMap;
};

#endif