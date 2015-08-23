#include "AddRewardsProcess.h"
#include "HatManager.h"
#include "AnimalsManager.h"
#include "MoneyManager.h"
#include "RewardsManager.h"

AddRewardsProcess::AddRewardsProcess(const std::vector<std::string>& rewards) {
	_rewards = rewards;
	_canProcess = true;
}

AddRewardsProcess::~AddRewardsProcess() {
	_rewards.clear();
}

void AddRewardsProcess::process() {
	if (_part == 0) {
		for (int i = 0; i < _rewards.size(); i++) {
			parse(RewardsManager::instance.getReward(_rewards[i]));
		}

		_completed = true;
	}
}

void AddRewardsProcess::parse(spRewardModel model) {
	if (model->getType() == rmHat) {
		HatManager::instance.addWearableToFreeHats(model->getName(), model->getCount());
	}
	else if (model->getType() == rmAnimal) {
		spAnimalModel animalModel = AnimalsManager::instance.getAnimalModel(model->getName());
		AnimalsManager::instance.increaseAnimalCount(animalModel, model->getCount());
	}
	else if (model->getType() == rmGold) {
		MoneyManager::instance.addMoney(model->getCount());
	}
}

bool AddRewardsProcess::completed() {
	return _completed;
}