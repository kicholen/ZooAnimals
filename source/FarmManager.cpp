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

std::string FarmManager::getZooGateLevel(int regionsUnblocked) {
	std::string level;
	if (regionsUnblocked == 1) {
		level = "first";
	}
	else if (regionsUnblocked == 2) {
		level = "second";
	}
	else if (regionsUnblocked == 3) {
		level = "third";
	}
	else if (regionsUnblocked == 4) {
		level = "fourth";
	}
	else if (regionsUnblocked == 5) {
		level = "fifth";
	}
	else if (regionsUnblocked == 6) {
		level = "sixth";
	}

	return level;
}