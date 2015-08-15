#include "RewardModel.h"

RewardModel::RewardModel(int type, int count, const std::string& name)
{
	_type = static_cast<RewardType>(type);
	_count = count;
	_name = name;
}


RewardModel::~RewardModel()
{
}
