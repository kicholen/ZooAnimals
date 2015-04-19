#ifndef _MONEYMANAGER_
#define _MONEYMANAGER_

#include "FlashUtils.h"

using namespace FlashUtils;

class MoneyManager : EventDispatcher
{
public:
	static MoneyManager instance;

	class MoneyEvent : public Event
	{
	public:
		enum EV
		{
			MONEY_COUNT = makefourcc('M', 'M', 'M', 'C'),
		};

		MoneyEvent(EV ev, int money_) : Event(ev), money(money_) {}

		int money;
	};

	MoneyManager();
	~MoneyManager();

	void init(const string& version);

	void increaseMoneyOnGameFinished(int zooLevel, int game, const string& difficulty);
	int getMoney();
	int getMoneyGained(int zooLevel, int game, const string& difficulty);

	void store();
private:
	void fillMoneyGainer();

	void dispatchMoneyCountEvent();
private:
	double _cashGainMultiplier;
	double _cashGainPower;
	double _secondMultiplier;
	double _thirdMultiplier;
	int _zooCount;
	vector<vector<int> > _moneyGainer;

	int _money;
};

#endif