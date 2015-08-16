#include "RewardModel.h"

RewardModel::RewardModel(int type, int count, const std::string& resourceName, int lockitId)
{
	_type = static_cast<RewardType>(type);
	_count = count;
	_resourceName = resourceName;
	_lockitId = lockitId;
}


RewardModel::~RewardModel()
{
}
