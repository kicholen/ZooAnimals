#ifndef _EXPMANAGER_
#define _EXPMANAGER_

#include "oxygine-framework.h"
#include "RewardModel.h"
#include "FlashUtils.h"

using namespace FlashUtils;
using namespace oxygine;

class ExpManager : EventDispatcher
{
public:
	static ExpManager instance;

	class ExpEvent : public Event
	{
	public:
		enum EV
		{
			EXP_COUNT = makefourcc('E', 'E', 'E', 'C'),
			LEVEL_UP = makefourcc('E', 'M', 'L', 'U'),
		};

		ExpEvent(EV ev, int exp_, int level_) : Event(ev), exp(exp_), level(level_) {}

		int exp;
		int level;
	};

	ExpManager();
	~ExpManager();

	void init(const std::string& version);

	int getLevel();
	const VectorArray<std::string>& getRewardsForLevel(int level);
	VectorArray<std::string> getRewardsForCurrentLevel();
	const VectorArray< VectorArray<std::string> >& getAllRewards();

	void increaseExpByPoints(int points);

	void store();
private:
	void fillNeededExp();
	void updateLevelIfGained();
	void parseRewards();

	void dispatchExpCountEvent();
	void dispatchLevelUpEvent();

private:
	double _expGainerMultiplier;
	double _expGainPower;
	int _levelCount;
	std::vector<int> _expNeededForLevels;
	VectorArray< VectorArray<std::string> > _rewardsForLevels;

	int _exp;
	int _level;
};

#endif