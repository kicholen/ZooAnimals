#include "AchievementModel.h"

AchievementModel::AchievementModel(const std::string& resourceName, int lockitTitle, int lockitDescription, const std::string& state) {
	_resourceName = resourceName;
	_lockitTitle = lockitTitle;
	_lockitDescription = lockitDescription;
	_state = state;	
	_currentPart = 0;
}

AchievementModel::~AchievementModel() {

}

void AchievementModel::addPart(int progress) {
	_parts.push_back(progress);
}

const int AchievementModel::getProgressNeededByPart(int part) const {
	return _parts[part];
}


void AchievementModel::revalidate() {
	for (int i = 0; i < _parts.size(); i++) {
		if (_progress >= _parts[i]) {
			_currentPart = i;
		}
	}
}

bool AchievementModel::increaseProgress() {
	_progress += 1;

	return _progress >= _maxProgress;
}