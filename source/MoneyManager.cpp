#include "MoneyManager.h"
#include "Content.h"
#include "ZooSettings.h"

MoneyManager MoneyManager::instance;

MoneyManager::MoneyManager() {

}

MoneyManager::~MoneyManager() {
	_moneyGainer.clear();
}

void MoneyManager::init(const string& version) {
	_money = ZooSettings::instance.getPlayerValue("money").as_int();
	_cashGainMultiplier = Content::instance.getParameterValue("cash_gain_multiplier").as_double();
	_cashGainPower = Content::instance.getParameterValue("cash_gain_power").as_double();
	_zooCount = Content::instance.getParameterAsInt("zoo_count");
	fillMoneyGainer();
}

void MoneyManager::increaseMoneyOnGameFinished(int zooLevel, int game, const string& difficulty) {
	_money += getMoneyGained(zooLevel, game, difficulty);
}

/*
** ZooLevel - when zoo is unblocked level is signed to it [0, zoo_count - 1]
** game - top - 3 / medium - 2 / low - 1
** difficulty - easy / normal / hard
*/
int MoneyManager::getMoneyGained(int zooLevel, int game, const string& difficulty) {
	int gameDifficulty = 0;
	
	if (difficulty == "easy") {
		gameDifficulty = 1;
	}
	else if (difficulty == "normal") {
		gameDifficulty = 2;
	}
	else {
		gameDifficulty = 3;
	}
	// TODO
	return _moneyGainer[zooLevel * gameDifficulty][game];
}

void MoneyManager::store() {
	ZooSettings::instance.addPlayerValue("money").set_value(_money);
	ZooSettings::instance.save();
}

void MoneyManager::fillMoneyGainer() {
	//_moneyGainer.reserve(16);
	int gamesPerZoo = 3;
	_moneyGainer = vector<vector<int> >(_zooCount, vector<int>(gamesPerZoo - 1));

	// fill statics
	_moneyGainer[0][0] = 1;
	_moneyGainer[0][1] = 2;
	_moneyGainer[0][2] = 3;
	
	
	for (int i = 0; i < 3; i++) {
		_moneyGainer[1][i] = (int)round(_cashGainMultiplier * (double)_moneyGainer[0][i]);
		_moneyGainer[2][i] = (int)round(_cashGainMultiplier * (double)_moneyGainer[0][i]);
	}
	
	_moneyGainer[3][0] = 2;
	_moneyGainer[3][1] = (int)round(pow(_cashGainMultiplier * (double)_moneyGainer[1][1], _cashGainPower));
	_moneyGainer[3][2] = (int)round(pow(_cashGainMultiplier * (double)_moneyGainer[1][2], _cashGainPower));

	// fill rest
	for (int i = 4; i < _zooCount; i++) {
		for (int j = 0; j < gamesPerZoo; j++) {
			_moneyGainer[i][j] = (int)round(pow(_cashGainMultiplier * (double)_moneyGainer[i - 3][j], _cashGainPower));
		}
	}

}