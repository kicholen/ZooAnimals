#ifndef _MONEYMANAGER_
#define _MONEYMANAGER_

#include "FlashUtils.h"

using namespace FlashUtils;

class MoneyManager : EventDispatcher
{
public:
	static MoneyManager instance;

	MoneyManager();
	~MoneyManager();

	void init(const string& version);

	void increaseMoneyOnGameFinished(int zooLevel, int game, const string& difficulty);
	int getMoneyGained(int zooLevel, int game, const string& difficulty);

	void store();

private:
	void fillMoneyGainer();

private:
	double _cashGainMultiplier;
	double _cashGainPower;
	int _zooCount;
	vector<vector<int> > _moneyGainer;

	int _money;
};

#endif