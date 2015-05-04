#include "ExpManager.h"
#include "ZooSettings.h"
#include "Content.h"
#include "FlashUtils.h"
ExpManager ExpManager::instance;

ExpManager::ExpManager() {

}

ExpManager::~ExpManager() {
	_expNeededForLevels.clear();
}

void ExpManager::init(const string& version) {
	_level = 0;
	_exp = ZooSettings::instance.getPlayerValue("exp").as_int();
	_expGainerMultiplier = Content::instance.getParameterValue("gain_multiplier_exp").as_double();
	_expGainPower = Content::instance.getParameterValue("gain_power_exp").as_double();
	_levelCount = Content::instance.getParameterAsInt("level_count");
	fillNeededExp();
}

void ExpManager::increaseExpByPoints(int points) {
	_exp += points;
	updateLevelIfGained();
	dispatchExpCountEvent();
}

int ExpManager::getLevel() {
	return _level;
}

void ExpManager::store() {
	ZooSettings::instance.addPlayerValue("exp").set_value(_exp);
}

void ExpManager::fillNeededExp() {
	_expNeededForLevels = vector<int>(_levelCount);
	for (int i = 0; i <= _levelCount; i++) {
		_expNeededForLevels[i] = (int)round(_expGainerMultiplier * pow((double)i, _expGainPower));
		//oxygine::log::warning(FlashUtils::CMath::intToString(_expNeededForLevels[i]).c_str());
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