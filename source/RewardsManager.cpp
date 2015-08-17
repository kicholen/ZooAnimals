#include "RewardsManager.h"
#include "Content.h"

RewardsManager RewardsManager::instance;

RewardsManager::RewardsManager() {

}

RewardsManager::~RewardsManager() {
	_rewardsMap.clear();
}

spRewardModel RewardsManager::getReward(const std::string& rewardName) {
	return _rewardsMap[rewardName];
}

void RewardsManager::init() {
	parseRewards();
}

void RewardsManager::parseRewards() {
	pugi::xml_node rewards = Content::instance.getRewardsNode();
	pugi::xml_node reward = rewards.first_child();
	int type = 0;
	std::string name = "";
	int count = 0;
	int level = 0;
	int lockitId = 0;

	while (!reward.empty()) {
		pugi::xml_attribute attribute = reward.first_attribute();

		if (!strcmp(attribute.as_string(), "hat")) {
			type = 0;
		}
		else if (!strcmp(attribute.as_string(), "animal")) {
			type = 1;
		}
		else if (!strcmp(attribute.as_string(), "gold")) {
			type = 2;
		}

		attribute = attribute.next_attribute();
		name = attribute.as_string();
		attribute = attribute.next_attribute();
		count = attribute.as_int();
		attribute = attribute.next_attribute();
		lockitId = attribute.as_int();

		_rewardsMap[reward.name()] = new RewardModel(type, count, name, lockitId);
		
		reward = reward.next_sibling();
	}
}