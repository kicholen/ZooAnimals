#include "AnimalsManager.h"
#include "ZooSettings.h"
#include "SharedResources.h"
#include "Timer.h"
#include "StartGameConfig.h"

AnimalsManager AnimalsManager::instance;

AnimalsManager::AnimalsManager() {
	_speciesPossesedCount = 0;
}

AnimalsManager::~AnimalsManager() {
	for (map<std::string, map<std::string, spAnimalModel> >::iterator outerIterator = _animalsMap.begin(); outerIterator != _animalsMap.end(); ++outerIterator) {
		for (map<std::string, spAnimalModel>::iterator innerIterator = outerIterator->second.begin(); innerIterator != outerIterator->second.end(); ++innerIterator) {
			innerIterator->second->releaseRef();
		}
	}
	_animalsMap.clear();
	_posessedAnimalMap.clear();

	_timer->detach();
}

void AnimalsManager::init(const std::string& version) {
	ZooSettings::instance.init(version);

	createFarmAnimals();
	createWinterAnimals();
	createUnderwaterAnimals();
	createSteppeAnimals();
	createAsiaAnimals();
	createAustraliaAnimals();
	createTimer();
	updater(0);
	addRef();
}

void AnimalsManager::feedAnimalByModel(spAnimalModel model) {
	model->setLastFeedS(getCurrentTimeInSeconds());
}

void AnimalsManager::feedAnimalByName(const std::string& name) {
	feedAnimalByModel(getAnimalModel(name));
}

bool AnimalsManager::canAnimalBeFedByModel(spAnimalModel model) {
	return model->lastFeedS() + FEED_INTERVAL_SECONDS >= getCurrentTimeInSeconds();
}

bool AnimalsManager::canAnimalBeFedByName(const std::string& name) {
	return canAnimalBeFedByModel(getAnimalModel(name));
}
// time consuming, maps was made on purpose, and here it is removed
spAnimalModel AnimalsManager::getAnimalModel(const std::string& name) {
	for (map<string, map<string, spAnimalModel> >::iterator outerIterator = _animalsMap.begin(); outerIterator != _animalsMap.end(); ++outerIterator) {
		for (map<string, spAnimalModel>::iterator innerIterator = outerIterator->second.begin(); innerIterator != outerIterator->second.end(); ++innerIterator) {
			if (innerIterator->second->animalName() == name) {
				return innerIterator->second;
			}
		}
	}

	log::error("Animal doesn't exist");
	return new AnimalModel("", 0, 0, 0, 0);
}

spAnimalModel AnimalsManager::getAnimalModelByRegion(const std::string& region, const std::string& name) {
	return _animalsMap[region][name];
}

const animalMap& AnimalsManager::getAnimalsByRegion(const std::string& name) {
	return _animalsMap[name];
}

bool AnimalsManager::isRegionPopulated(const std::string& regionName) {
	return _posessedAnimalMap.count(regionName) != 0;
}

const animalMap& AnimalsManager::getPossesedAnimalsByRegion(const std::string& region) {
	return _posessedAnimalMap[region];
}

const map<std::string, animalMap >& AnimalsManager::getPossesedAnimals() {
	return _posessedAnimalMap;
}

void AnimalsManager::increaseHappinessByPoints(spAnimalModel model, int points) {
	int countBefore = model->totalAnimalsCount();
	model->setHappiness(model->happinessValue() + points);

	if (countBefore != model->totalAnimalsCount()) {
		dispatchAnimalCountChangedEvent(model);
	}
}

void AnimalsManager::increaseAnimalCount(const std::string& region, const std::string& name, int count) {
	spAnimalModel model = getAnimalModelByRegion(region, name);
	model->setAnimalsCount(model->animalsCount() + count);

	if (_posessedAnimalMap[region].count(name) == 0) {
		_speciesPossesedCount += 1;
		model->setLevel(_speciesPossesedCount);
		model->setLastFeedS(getCurrentTimeInSeconds());
		_posessedAnimalMap[region].insert(make_pair(name, model));
	}

	dispatchAnimalCountChangedEvent(model);
}

void AnimalsManager::increaseAnimalCount(spAnimalModel model, int count) {
	model->setAnimalsCount(model->animalsCount() + count);
	const std::string& region = getAnimalRegion(model);

	if (_posessedAnimalMap[region].count(model->animalName()) == 0) {
		_speciesPossesedCount += 1;
		model->setLevel(_speciesPossesedCount);
		model->setLastFeedS(getCurrentTimeInSeconds());
		_posessedAnimalMap[region].insert(make_pair(model->animalName(), model));
	}

	dispatchAnimalCountChangedEvent(model);
	

}

void AnimalsManager::store() {
	for (map<string, animalMap >::iterator outerIterator = _posessedAnimalMap.begin(); outerIterator != _posessedAnimalMap.end(); ++outerIterator) {
		for (animalMap::iterator innerIterator = outerIterator->second.begin(); innerIterator != outerIterator->second.end(); ++innerIterator) {
			ZooSettings::instance.setAnimal(outerIterator->first, innerIterator->first, innerIterator->second->happinessValue(), innerIterator->second->hungerValue(), innerIterator->second->animalsCount(), innerIterator->second->lastFeedS(), innerIterator->second->getLevel());
		}
	}
}

void AnimalsManager::createFarmAnimals() {
	int sizeOfArray = sizeof(FARM) / sizeof(FARM[0]);
	const std::string regionName = "farm";

	for (int i = 0; i < sizeOfArray; i++) {
		pugi::xml_node animalNode = ZooSettings::instance.getAnimal(regionName, FARM[i]);
		if (animalNode) {
			addAnimalModel(regionName, FARM[i], animalNode.attribute("h").as_int(), animalNode.attribute("g").as_int(), animalNode.attribute("c").as_int(), animalNode.attribute("lf").as_int(), animalNode.attribute("l").as_int());
		}
		else {
			addAnimalModel(regionName, FARM[i], 0, 0, 0, 0);
		}
	}
}

void AnimalsManager::createWinterAnimals() {
	int sizeOfArray = sizeof(WINTER) / sizeof(WINTER[0]);
	const std::string regionName = "winter";

	for (int i = 0; i < sizeOfArray; i++) {
		pugi::xml_node animalNode = ZooSettings::instance.getAnimal(regionName, WINTER[i]);
		if (animalNode) {
			addAnimalModel(regionName, WINTER[i], animalNode.attribute("h").as_int(), animalNode.attribute("g").as_int(), animalNode.attribute("c").as_int(), animalNode.attribute("lf").as_int(), animalNode.attribute("l").as_int());
		}
		else {
			addAnimalModel(regionName, WINTER[i], 0, 0, 0, 0);
		}
	}
}

void AnimalsManager::createUnderwaterAnimals() {
	int sizeOfArray = sizeof(UNDERWATER) / sizeof(UNDERWATER[0]);
	const std::string regionName = "underwater";

	for (int i = 0; i < sizeOfArray; i++) {
		pugi::xml_node animalNode = ZooSettings::instance.getAnimal(regionName, UNDERWATER[i]);
		if (animalNode) {
			addAnimalModel(regionName, UNDERWATER[i], animalNode.attribute("h").as_int(), animalNode.attribute("g").as_int(), animalNode.attribute("c").as_int(), animalNode.attribute("lf").as_int(), animalNode.attribute("l").as_int());
		}
		else {
			addAnimalModel(regionName, UNDERWATER[i], 0, 0, 0, 0);
		}
	}
}

void AnimalsManager::createSteppeAnimals() {
	int sizeOfArray = sizeof(STEPPE) / sizeof(STEPPE[0]);
	const std::string regionName = "steppe";

	for (int i = 0; i < sizeOfArray; i++) {
		pugi::xml_node animalNode = ZooSettings::instance.getAnimal(regionName, STEPPE[i]);
		if (animalNode) {
			addAnimalModel(regionName, STEPPE[i], animalNode.attribute("h").as_int(), animalNode.attribute("g").as_int(), animalNode.attribute("c").as_int(), animalNode.attribute("lf").as_int(), animalNode.attribute("l").as_int());
		}
		else {
			addAnimalModel(regionName, STEPPE[i], 0, 0, 0, 0);
		}
	}
}

void AnimalsManager::createAsiaAnimals() {
	int sizeOfArray = sizeof(ASIA) / sizeof(ASIA[0]);
	const std::string regionName = "asia";

	for (int i = 0; i < sizeOfArray; i++) {
		pugi::xml_node animalNode = ZooSettings::instance.getAnimal(regionName, ASIA[i]);
		if (animalNode) {
			addAnimalModel(regionName, ASIA[i], animalNode.attribute("h").as_int(), animalNode.attribute("g").as_int(), animalNode.attribute("c").as_int(), animalNode.attribute("lf").as_int(), animalNode.attribute("l").as_int());
		}
		else {
			addAnimalModel(regionName, ASIA[i], 0, 0, 0, 0);
		}
	}
}

void AnimalsManager::createAustraliaAnimals() {
	int sizeOfArray = sizeof(AUSTRALIA) / sizeof(AUSTRALIA[0]);
	const std::string regionName = "australia";

	for (int i = 0; i < sizeOfArray; i++) {
		pugi::xml_node animalNode = ZooSettings::instance.getAnimal(regionName, AUSTRALIA[i]);
		if (animalNode) {
			addAnimalModel(regionName, AUSTRALIA[i], animalNode.attribute("h").as_int(), animalNode.attribute("g").as_int(), animalNode.attribute("c").as_int(), animalNode.attribute("lf").as_int(), animalNode.attribute("l").as_int());
		}
		else {
			addAnimalModel(regionName, AUSTRALIA[i], 0, 0, 0, 0);
		}
	}
}

void AnimalsManager::createTimer() {
	_timer = new Timer(UPDATER_STEP_IN_SECONDS * 1000, -1, false);
	_timer->setTimerCallback(CLOSURE(this, &AnimalsManager::updater));
	_timer->start();
}

void AnimalsManager::addAnimalModel(const std::string& regionName, const std::string& name, int happiness, int hunger, int count, int lastFeedS, int level) {
	spAnimalModel model = new AnimalModel(name, happiness, hunger, count, lastFeedS);
	model->setLevel(level);
	model->fromContent();
	_animalsMap[regionName].insert(make_pair(name, model));
	
	if (count > 0) {
		_speciesPossesedCount += 1;
		_posessedAnimalMap[regionName].insert(make_pair(name, model));
	}
}

const std::string& AnimalsManager::getAnimalRegion(spAnimalModel model) {
	for (map<string, animalMap >::iterator outerIterator = _animalsMap.begin(); outerIterator != _animalsMap.end(); ++outerIterator) {
		for (animalMap::iterator innerIterator = outerIterator->second.begin(); innerIterator != outerIterator->second.end(); ++innerIterator) {
			if (innerIterator->second->animalName() == model->animalName()) {
				return outerIterator->first;
			}
		}
	}

	OX_ASSERT(false);
	return "";
}

void AnimalsManager::updater(Event* event) {
	Timer::TimerEvent* timerEvent = static_cast<Timer::TimerEvent*>(event);
	int currentTime = getCurrentTimeInSeconds();

	for (map<string, animalMap >::iterator outerIterator = _posessedAnimalMap.begin(); outerIterator != _posessedAnimalMap.end(); ++outerIterator) {
		for (animalMap::iterator innerIterator = outerIterator->second.begin(); innerIterator != outerIterator->second.end(); ++innerIterator) {
			if (canAnimalBeFedByModel(innerIterator->second)) {
				dispatchAnimalCanBeFedEvent(innerIterator->second);
			}
			
			if (innerIterator->second->lastFeedS() + HUNGER_BARRIER_TIME_SECONDS < currentTime) {
				decreaseHappinessByPercent(innerIterator->second, HUNGER_HAPPINES_DECREASE_PERCENT);
				feedAnimalByModel(innerIterator->second);
			}
		}
	}
}

void AnimalsManager::decreaseHappinessByPercent(spAnimalModel model, int percent) {
	int countBefore = model->totalAnimalsCount();
	model->setHappiness(model->happinessValue() * percent / 100);

	if (countBefore != model->totalAnimalsCount()) {
		dispatchAnimalCountChangedEvent(model);
	}
}

void AnimalsManager::dispatchAnimalCountChangedEvent(spAnimalModel model) {
	AnimalEvent ev(AnimalEvent::COUNT_CHANGED, model);
	dispatchEvent(&ev);
}

void AnimalsManager::dispatchAnimalCanBeFedEvent(spAnimalModel model) {
	AnimalEvent ev(AnimalEvent::CAN_FEED, model);
	dispatchEvent(&ev);
}

int AnimalsManager::getCurrentTimeInSeconds() {
	return (int)(s3eTimerGetUTC() / 1000);
}