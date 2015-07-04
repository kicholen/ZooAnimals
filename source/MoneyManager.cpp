#include "MoneyManager.h"
#include "Content.h"
#include "ZooSettings.h"

MoneyManager MoneyManager::instance;

MoneyManager::MoneyManager() {

}

MoneyManager::~MoneyManager() {
	_moneyGainer.clear();
}

void MoneyManager::init(const std::string& version) {
	_money = ZooSettings::instance.getPlayerValue("money").as_int();
	_secondMultiplier = Content::instance.getParameterValue("second_multi_cash").as_double();
	_thirdMultiplier = Content::instance.getParameterValue("third_multi_cash").as_double();
	_cashGainMultiplier = Content::instance.getParameterValue("gain_multiplier_cash").as_double();
	_cashGainPower = Content::instance.getParameterValue("gain_power_cash").as_double();
	_zooCount = Content::instance.getParameterAsInt("zoo_count");
	fillMoneyGainer();
}

void MoneyManager::increaseMoneyOnGameFinished(int zooLevel, int game, const std::string& difficulty) {
	_money += getMoneyGained(zooLevel, game, difficulty);
	dispatchMoneyCountEvent();
}

void MoneyManager::decreaseMoneyOnItemBought(int money) {
	_money -= money;
	dispatchMoneyCountEvent();
}

int MoneyManager::getMoney() {
	return _money;
}

/*
** ZooLevel - when zoo is unblocked level is signed to it [0, zoo_count - 1]
** game - top - 2 / medium - 1 / low - 0
** difficulty - easy / normal / hard
*/
int MoneyManager::getMoneyGained(int zooLevel, int game, const std::string& difficulty) {
	int gameDifficulty = 0;
	int gamesPerAnimal = 3;

	if (difficulty == "easy") {
		gameDifficulty = 0;
	}
	else if (difficulty == "normal") {
		gameDifficulty = 1;
	}
	else {
		gameDifficulty = 2;
	}
	
	return _moneyGainer[zooLevel * gamesPerAnimal + gameDifficulty][game];
}

void MoneyManager::store() {
	ZooSettings::instance.addPlayerValue("money").set_value(_money);
}

void MoneyManager::fillMoneyGainer() {
	int gamesPerZoo = 3;
	int difficulties = 3;
	_moneyGainer = vector<vector<int> >(_zooCount * difficulties, vector<int>(gamesPerZoo));

	// fill statics
	_moneyGainer[0][0] = 1;
	_moneyGainer[0][1] = 2;
	_moneyGainer[0][2] = 3;

	for (int i = 0; i < 3; i++) {
		_moneyGainer[1][i] = (int)round(_secondMultiplier * (double)_moneyGainer[0][i]);
		_moneyGainer[2][i] = (int)round(_thirdMultiplier * (double)_moneyGainer[0][i]);
	}
	_moneyGainer[3][0] = 2;
	_moneyGainer[3][1] = (int)round(pow(_cashGainMultiplier * (double)_moneyGainer[0][1], _cashGainPower));
	_moneyGainer[3][2] = (int)round(pow(_cashGainMultiplier * (double)_moneyGainer[0][2], _cashGainPower));

	// fill rest
	for (int i = 4; i < _zooCount * difficulties; i++) {
		for (int j = 0; j < gamesPerZoo; j++) {
			_moneyGainer[i][j] = (int)round(pow(_cashGainMultiplier * (double)_moneyGainer[i - 3][j], _cashGainPower));
		}
	}
}

void MoneyManager::dispatchMoneyCountEvent() {
	MoneyEvent ev(MoneyEvent::MONEY_COUNT, _money);
	getRoot()->dispatchEvent(&ev);
}