#include "ExpManager.h"
#include "ZooSettings.h"
#include "Content.h"
#include "FlashUtils.h"
#include "Content.h"


ExpManager ExpManager::instance;

ExpManager::ExpManager() {

}

ExpManager::~ExpManager() {
	_expNeededForLevels.clear();
	_rewardsForLevels.clear();
}

void ExpManager::init(const std::string& version) {
	_level = 0;
	_exp = ZooSettings::instance.getPlayerValue("exp").as_int();
	_expGainerMultiplier = Content::instance.getParameterValue("gain_multiplier_exp").as_double();
	_expGainPower = Content::instance.getParameterValue("gain_power_exp").as_double();
	_levelCount = Content::instance.getParameterAsInt("level_count");
	fillNeededExp();
	parseRewards();
}

void ExpManager::increaseExpByPoints(int points) {
	_exp += points;
	updateLevelIfGained();
	dispatchExpCountEvent();
}

int ExpManager::getLevel() {
	return _level;
}

const VectorArray<spRewardModel>& ExpManager::getRewardsForLevel(int level) {
	return _rewardsForLevels[level];
}

const VectorArray< VectorArray<spRewardModel> >& ExpManager::getAllRewards() {
	return _rewardsForLevels;
}

void ExpManager::store() {
	ZooSettings::instance.addPlayerValue("exp").set_value(_exp);
}

void ExpManager::fillNeededExp() {
	_expNeededForLevels = vector<int>(_levelCount);
	for (int i = 0; i <= _levelCount; i++) {
		_expNeededForLevels[i] = (int)round(_expGainerMultiplier * pow((double)i, _expGainPower));
		if (_exp >= _expNeededForLevels[i]) {
			_level += 1;
		}
	}
	_level -= 1;
}

void ExpManager::updateLevelIfGained() {
	if (_exp >= _expNeededForLevels[_level + 1]) {
		_level += 1;
	}
}

void ExpManager::dispatchExpCountEvent() {
	ExpEvent ev(ExpEvent::EXP_COUNT, _exp, _level);
	getRoot()->dispatchEvent(&ev);
}

void ExpManager::dispatchLevelUpEvent() {
	ExpEvent ev(ExpEvent::EXP_COUNT, _exp, _level);
	getRoot()->dispatchEvent(&ev);
}

void ExpManager::parseRewards() {
	pugi::xml_node rewards = Content::instance.getLevelRewardsNode();
	pugi::xml_node rewardLevel = rewards.first_child();
	int type = 0;
	std::string name = "";
	int count = 0;
	int level = 0;

	while (!rewardLevel.empty()) {
		pugi::xml_node reward = rewardLevel.first_child();
		VectorArray<spRewardModel> innerVector;

		while (!reward.empty()) {
			if (!strcmp(reward.name(), "hat")) {
				type = 0;
			}
			else if (!strcmp(reward.name(), "animal")) {
				type = 1;
			}
			else if (!strcmp(reward.name(), "gold")) {
				type = 2;
			}
			
			pugi::xml_attribute attribute = reward.first_attribute();
			name = attribute.as_string();
			attribute = attribute.next_attribute();
			count = attribute.as_int();

			innerVector.push(new RewardModel(type, count, name));
			reward = reward.next_sibling();
		}

		_rewardsForLevels.push(innerVector);
		level += 1;
		rewardLevel = rewardLevel.next_sibling();
	}
}