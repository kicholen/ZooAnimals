#ifndef _REWARDMODEL_
#define _REWARDMODEL_

#include "oxygine-framework.h"

using namespace oxygine;

DECLARE_SMART(RewardModel, spRewardModel);

typedef enum {rmHat, rmAnimal, rmGold} RewardType;

class RewardModel :	public Object
{
public:
	RewardModel(int type, int count, const std::string& name);
	~RewardModel();

	RewardType getType() const {
		return _type;
	}

	int getCount() const {
		return _count;
	}

	const std::string& getName() { return _name; }

private:
	RewardType _type;
	int _count;
	std::string _name;
};

#endif