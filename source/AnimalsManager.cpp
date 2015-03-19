#include "AnimalsManager.h"
#include "ZooSettings.h"
#include "SharedResources.h"

AnimalsManager AnimalsManager::instance;

AnimalsManager::AnimalsManager() {

}


AnimalsManager::~AnimalsManager() {

}

void AnimalsManager::init(const string& version) {
	ZooSettings::instance.init(version);

	createFarmAnimals();
	createWinterAnimals();
	createUnderwaterAnimals();
	createSteppeAnimals();
	createAsiaAnimals();
	createAustraliaAnimals();
}

void AnimalsManager::createFarmAnimals() {
	int sizeOfArray = sizeof(FARM) / sizeof(FARM[0]);
	const string regionName = "farm";

	for (int i = 0; i < sizeOfArray; i++) {
		pugi::xml_node animalNode = ZooSettings::instance.getAnimal(regionName, FARM[i]);
		if (animalNode) {
			addAnimalModel(regionName, FARM[i], animalNode.attribute("h").as_int(), animalNode.attribute("g").as_int(), animalNode.attribute("c").as_int());
		}
		else {
			addAnimalModel(regionName, FARM[i], 100, 100, 0);
		}
	}
}

void AnimalsManager::createWinterAnimals() {
	int sizeOfArray = sizeof(WINTER) / sizeof(WINTER[0]);
	const string regionName = "winter";

	for (int i = 0; i < sizeOfArray; i++) {
		pugi::xml_node animalNode = ZooSettings::instance.getAnimal(regionName, WINTER[i]);
		if (animalNode) {
			addAnimalModel(regionName, WINTER[i], animalNode.attribute("h").as_int(), animalNode.attribute("g").as_int(), animalNode.attribute("c").as_int());
		}
		else {
			addAnimalModel(regionName, WINTER[i], 100, 100, 0);
		}
	}
}

void AnimalsManager::createUnderwaterAnimals() {
	int sizeOfArray = sizeof(UNDERWATER) / sizeof(UNDERWATER[0]);
	const string regionName = "underwater";

	for (int i = 0; i < sizeOfArray; i++) {
		pugi::xml_node animalNode = ZooSettings::instance.getAnimal(regionName, UNDERWATER[i]);
		if (animalNode) {
			addAnimalModel(regionName, UNDERWATER[i], animalNode.attribute("h").as_int(), animalNode.attribute("g").as_int(), animalNode.attribute("c").as_int());
		}
		else {
			addAnimalModel(regionName, UNDERWATER[i], 100, 100, 0);
		}
	}
}

void AnimalsManager::createSteppeAnimals() {
	int sizeOfArray = sizeof(STEPPE) / sizeof(STEPPE[0]);
	const string regionName = "steppe";

	for (int i = 0; i < sizeOfArray; i++) {
		pugi::xml_node animalNode = ZooSettings::instance.getAnimal(regionName, STEPPE[i]);
		if (animalNode) {
			addAnimalModel(regionName, STEPPE[i], animalNode.attribute("h").as_int(), animalNode.attribute("g").as_int(), animalNode.attribute("c").as_int());
		}
		else {
			addAnimalModel(regionName, STEPPE[i], 100, 100, 0);
		}
	}
}

void AnimalsManager::createAsiaAnimals() {
	int sizeOfArray = sizeof(ASIA) / sizeof(ASIA[0]);
	const string regionName = "asia";

	for (int i = 0; i < sizeOfArray; i++) {
		pugi::xml_node animalNode = ZooSettings::instance.getAnimal(regionName, ASIA[i]);
		if (animalNode) {
			addAnimalModel(regionName, ASIA[i], animalNode.attribute("h").as_int(), animalNode.attribute("g").as_int(), animalNode.attribute("c").as_int());
		}
		else {
			addAnimalModel(regionName, ASIA[i], 100, 100, 0);
		}
	}
}

void AnimalsManager::createAustraliaAnimals() {
	int sizeOfArray = sizeof(AUSTRALIA) / sizeof(AUSTRALIA[0]);
	const string regionName = "australia";

	for (int i = 0; i < sizeOfArray; i++) {
		pugi::xml_node animalNode = ZooSettings::instance.getAnimal(regionName, AUSTRALIA[i]);
		if (animalNode) {
			addAnimalModel(regionName, AUSTRALIA[i], animalNode.attribute("h").as_int(), animalNode.attribute("g").as_int(), animalNode.attribute("c").as_int());
		}
		else {
			addAnimalModel(regionName, AUSTRALIA[i], 100, 100, 0);
		}
	}
}

void AnimalsManager::addAnimalModel(const string& regionName, const string& name, int happiness, int hunger, int count) {
	spAnimalModel model = new AnimalModel(name, happiness, hunger, count);
	model->fromContent();
	_animalsMap[regionName].insert(make_pair(name, model));
}

spAnimalModel AnimalsManager::getAnimalModel(const string& name) {
	for(auto const &it1 : _animalsMap) {
		for(auto const &it2 : it1.second) {
			if (it2.first == name) {
				return it2.second;
			}
		}
    }

	log::error("Animal doesn't exists");
	return nullptr;
}

spAnimalModel AnimalsManager::getAnimalModelByRegion(const string& region, const string& name) {
	return _animalsMap[region][name];
}

void AnimalsManager::store() {
	for(auto const &it1 : _animalsMap) {
		for(auto const &it2 : it1.second) {
			if (it2.second->animalsCount() > 0) {
				ZooSettings::instance.setAnimal(it1.first, it2.first, it2.second->happinessValue(), it2.second->hungerValue(), it2.second->animalsCount());
			}
		}
    }

	ZooSettings::instance.save();
}