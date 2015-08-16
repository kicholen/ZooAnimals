#include "MessageModel.h"


MessageModel::MessageModel(int type, int lockitTitle, int lockitDesc, const std::string& resourceName, int dateMS) {
	setData(type, lockitTitle, lockitDesc, resourceName, dateMS);
}

MessageModel::MessageModel() {
	_rewards.clear();
}

MessageModel::~MessageModel() {
	_rewards.clear();
}

void MessageModel::setData(int type, int lockitTitle, int lockitDesc, const std::string& resourceName, int dateMS) {
	_type = type;
	_lockitTitle = lockitTitle;
	_lockitDescription = lockitDesc;
	_resourceName = resourceName;
	_dateMS = dateMS;
}

void MessageModel::addReward(int reward) {
	_rewards.push_back(reward);
}