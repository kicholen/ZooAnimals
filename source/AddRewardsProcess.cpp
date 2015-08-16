#include "AddRewardsProcess.h"
#include "HatManager.h"
#include "AnimalsManager.h"
#include "MoneyManager.h"

AddRewardsProcess::AddRewardsProcess(VectorArray<spRewardModel> rewards)
{
	_rewards = rewards;
	_canProcess = true;
}


AddRewardsProcess::~AddRewardsProcess()
{
}

void AddRewardsProcess::process() {
	if (_part == 0) {
		for (int i = 0; i < _rewards.length(); i++) {
			parse(_rewards[i]);
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