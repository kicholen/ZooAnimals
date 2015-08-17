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

const VectorArray<std::string>& ExpManager::getRewardsForLevel(int level) {
	return _rewardsForLevels[level];
}

VectorArray<std::string> ExpManager::getRewardsForCurrentLevel() {
	return _rewardsForLevels[_level];
}

const VectorArray< VectorArray<std::string> >& ExpManager::getAllRewards() {
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

	while (!rewardLevel.empty()) {
		pugi::xml_node reward = rewardLevel.first_child();
		VectorArray<std::string> rewardsVector;

		while (!reward.empty()) {
			rewardsVector.push(reward.name());

			reward = reward.next_sibling();
		}

		_rewardsForLevels.push(rewardsVector);
		rewardLevel = rewardLevel.next_sibling();
	}
}