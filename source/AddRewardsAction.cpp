#include "AddRewardsAction.h"
#include "HatManager.h"
#include "AnimalsManager.h"
#include "MoneyManager.h"
#include "RewardsManager.h"

AddRewardsAction::AddRewardsAction(const std::vector<std::string>& rewards) {
	_rewards = rewards;
	doAction();
}


AddRewardsAction::~AddRewardsAction() {
	_rewards.clear();
}

void AddRewardsAction::doAction() {
	for (uint i = 0; i < _rewards.size(); i++) {
		parse(RewardsManager::instance.getReward(_rewards[i]));
	}

	complete();
}

void AddRewardsAction::complete() {
	detach();
}

void AddRewardsAction::parse(spRewardModel model) {
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
