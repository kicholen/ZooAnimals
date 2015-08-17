#ifndef _ACHIEVEMENTMODEL_
#define _ACHIEVEMENTMODEL_

#include "oxygine-framework.h"

using namespace oxygine;

DECLARE_SMART(AchievementModel, spAchievementModel);

class AchievementModel : public Object
{
public:
	AchievementModel(const std::string& resourceName, int lockitTitle, int lockitDescription, const std::string& state);
	~AchievementModel();

	bool increaseProgress();
	
	void addPart(int progress);
	void setProgress(int progress);
	void revalidate();

	const std::string& getResourceName() const {
		return _resourceName;
	}

	const int getCurrentPart() const {
		return _currentPart;
	}

	const int getAllPartsCount() const {
		return _parts.size();
	}

	const int getProgress() const {
		return _progress;
	}

	const int getProgressNeededByPart(int part) const;

	const int getMaxProgress() const {
		return _maxProgress;
	}

	const int getLockitTitle() const {
		return _lockitTitle;
	}

	const int getLockitDescription() const {
		return _lockitDescription;
	}

private:
	std::string _resourceName;
	std::string _state;
	std::vector<int> _parts;

	int _maxProgress;
	int _progress;
	int _currentPart;

	int _lockitTitle;
	int _lockitDescription;
};

#endif