#ifndef _REWARDMODEL_
#define _REWARDMODEL_

#include "oxygine-framework.h"

using namespace oxygine;

DECLARE_SMART(RewardModel, spRewardModel);

typedef enum {rmHat, rmAnimal, rmGold} RewardType;

class RewardModel :	public Object
{
public:
	RewardModel(int type, int count, const std::string& resourceName, int lockitId);
	~RewardModel();

	RewardType getType() const {
		return _type;
	}

	int getCount() const {
		return _count;
	}

	const std::string& getName() { return _resourceName; }

	int getLockit() const {
		return _lockitId;
	}

private:
	RewardType _type;
	int _count;
	std::string _resourceName;
	int _lockitId;
};

#endif