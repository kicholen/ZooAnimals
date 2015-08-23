#include "AchievementModel.h"

AchievementModel::AchievementModel(const std::string& resourceName, int lockitTitle, int lockitDescription, const std::string& state) {
	_resourceName = resourceName;
	_lockitTitle = lockitTitle;
	_lockitDescription = lockitDescription;
	_state = state;	
	_progress = 0;
	_maxProgress = 0;
	_currentPart = 0;
}

AchievementModel::~AchievementModel() {
	_rewards.clear();
	_parts.clear();
}

void AchievementModel::addPart(int progress) {
	_parts.push_back(progress);
}

void AchievementModel::addReward(int part, const std::string& rewardName) {
	if (_rewards.size() <= part) {
		std::vector<std::string> part;
		_rewards.push_back(part);
	}
	_rewards[part].push_back(rewardName);
}

const std::vector<std::string>& AchievementModel::getRewards(int part) {
	return _rewards[part];
}

void AchievementModel::setProgress(int progress) {
	_progress = progress;
}

const int AchievementModel::getProgressNeededByPart(int part) const {
	return _parts[part];
}

bool AchievementModel::isCompleted() {
	return _progress >= _maxProgress;
}

void AchievementModel::revalidate() {
	_maxProgress = 0;
	for (uint i = 0; i < _parts.size(); i++) {
		if (_progress >= _parts[i]) {
			_currentPart = i;
		}
		_maxProgress += _parts[i];
	}
}

bool AchievementModel::increaseProgress() {
	_progress += 1;

	bool partCompleted = _progress >= _parts[_currentPart];
	revalidate();

	return partCompleted;
}