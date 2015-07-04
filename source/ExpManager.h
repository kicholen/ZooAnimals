#ifndef _EXPMANAGER_
#define _EXPMANAGER_

#include "oxygine-framework.h"

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
		};

		ExpEvent(EV ev, int exp_, int level_) : Event(ev), exp(exp_), level(level_) {}

		int exp;
		int level;
	};

	ExpManager();
	~ExpManager();

	void init(const std::string& version);

	int getLevel();
	void increaseExpByPoints(int points);

	void store();

private:
	void fillNeededExp();
	void updateLevelIfGained();
	void dispatchExpCountEvent();

private:
	double _expGainerMultiplier;
	double _expGainPower;
	int _levelCount;
	std::vector<int> _expNeededForLevels;

	int _exp;
	int _level;
};

#endif