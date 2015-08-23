#include "FarmManager.h"

FarmManager FarmManager::instance;

FarmManager::FarmManager()
{
}


FarmManager::~FarmManager()
{
}

void FarmManager::init() {

}

std::string FarmManager::getFarmLevelByModel(spAnimalModel model) {
	int animalsCount = model->totalAnimalsCount();

	if (animalsCount > 20) {
		return "third";
	}
	else if (animalsCount > 10) {
		return "second";
	}
	else {
		return "first";
	}
}

int FarmManager::getGateHumanCount() {
	return 25;
}

int FarmManager::getGateShopHumanCount() {
	return 5;
}

int FarmManager::getFarmHumanCount() {
	return 50;
}

int FarmManager::getZooSize() {
	return 0;
}