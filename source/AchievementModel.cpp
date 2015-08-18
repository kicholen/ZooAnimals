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

}

void AchievementModel::addPart(int progress) {
	_parts.push_back(progress);
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