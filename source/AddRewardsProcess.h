#ifndef _ADDREWARDSPROCESS_
#define _ADDREWARDSPROCESS_

#include "ProcessSlave.h"
#include "FlashUtils.h"
#include "RewardModel.h"

using namespace FlashUtils;
class AddRewardsProcess : public ProcessSlave
{
public:
	AddRewardsProcess(const VectorArray<std::string>&  rewards);
	~AddRewardsProcess();

	virtual void process();
	virtual bool completed();

private:
	void parse(spRewardModel model);

	VectorArray<std::string> _rewards;
};

#endif