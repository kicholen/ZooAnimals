#ifndef _MESSAGEMODEL_
#define _MESSAGEMODEL_

#include "oxygine-framework.h"
#include "RewardModel.h"

using namespace oxygine;

DECLARE_SMART(MessageModel, spMessageModel);

class MessageModel : public Object
{
public:
	MessageModel(int type, int lockitTitle, int lockitDesc, const std::string& resourceName, int dateMS);
	MessageModel();
	~MessageModel();

	void setData(int type, int lockitTitle, int lockitDesc, const std::string& resourceName, int dateMS);
	void addReward(int rewardNum);

	const int getType() const {
		return _type;
	}

	const int getDateMS() const {
		return _dateMS;
	}

	const int getLockitDescription() const {
		return _lockitDescription;
	}

	const int getLockitTitle() const {
		return _lockitTitle;
	}

	const std::string& getResourceName() const {
		return _resourceName;
	}

	const std::vector<int>& getRewards() const {
		return _rewards;
	}

private:
	int _type;
	int _dateMS;
	int _lockitDescription;
	int _lockitTitle;
	std::string _resourceName;

	std::vector<int> _rewards;
};

#endif