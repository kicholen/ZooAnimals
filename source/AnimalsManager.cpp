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
	for (map<string, map<string, spAnimalModel> >::iterator outerIterator = _animalsMap.begin(); outerIterator != _animalsMap.end(); ++outerIterator) {
		for (map<string, spAnimalModel>::iterator innerIterator = outerIterator->second.begin(); innerIterator != outerIterator->second.end(); ++innerIterator) {
			innerIterator->second->releaseRef();
		}
	}
	_animalsMap.clear();
	for (map<string, map<string, spAnimalModel> >::iterator outerIterator = _posessedAnimalMap.begin(); outerIterator != _posessedAnimalMap.end(); ++outerIterator) {
		for (map<string, spAnimalModel>::iterator innerIterator = outerIterator->second.begin(); innerIterator != outerIterator->second.end(); ++innerIterator) {
			innerIterator->second->releaseRef();
		}
	}
	_posessedAnimalMap.clear();

	_timer->detach();
}

void AnimalsManager::init(const string& version) {
	ZooSettings::instance.init(version);

	createFarmAnimals();
	createWinterAnimals();
	createUnderwaterAnimals();
	createSteppeAnimals();
	createAsiaAnimals();
	createAustraliaAnimals();
	createTimer();
}

void AnimalsManager::feedAnimalByModel(spAnimalModel model) {
	model->setLastFeedS(getCurrentTimeInSeconds());
}

void AnimalsManager::feedAnimalByName(const string& name) {
	feedAnimalByModel(getAnimalModel(name));
}

bool AnimalsManager::canAnimalBeFedByModel(spAnimalModel model) {
	return model->lastFeedS() + FEED_INTERVAL_SECONDS >= getCurrentTimeInSeconds();
}

bool AnimalsManager::canAnimalBeFedByName(const string& name) {
	return canAnimalBeFedByModel(getAnimalModel(name));
}

spAnimalModel AnimalsManager::getAnimalModel(const string& name) {
	for (map<string, map<string, spAnimalModel> >::iterator outerIterator = _animalsMap.begin(); outerIterator != _animalsMap.end(); ++outerIterator) {
		for (map<string, spAnimalModel>::iterator innerIterator = outerIterator->second.begin(); innerIterator != outerIterator->second.end(); ++innerIterator) {
			return innerIterator->second;
		}
	}

	log::error("Animal doesn't exist");
	return new AnimalModel("", 0, 0, 0, 0);
}

spAnimalModel AnimalsManager::getAnimalModelByRegion(const string& region, const string& name) {
	return _animalsMap[region][name];
}

map<string, spAnimalModel>& AnimalsManager::getAnimalsByRegion(const string& name) {
	return _animalsMap[name];
}

bool AnimalsManager::isRegionPopulated(const string& regionName) {
	return _posessedAnimalMap.count(regionName) != 0;
}

map<string, spAnimalModel>& AnimalsManager::getPossesedAnimalsByRegion(const string& region) {
	return _posessedAnimalMap[region];
}

map<string, map<string, spAnimalModel> >& AnimalsManager::getPossesedAnimals() {
	return _posessedAnimalMap;
}

void AnimalsManager::increaseHappinessByPoints(spAnimalModel model, int points) {
	int countBefore = model->totalAnimalsCount();
	model->setHappiness(model->happinessValue() + points);

	if (countBefore != model->totalAnimalsCount()) {
		dispatchAnimalCountChangedEvent(model);
	}
}

void AnimalsManager::store() {
	for (map<string, map<string, spAnimalModel> >::iterator outerIterator = _posessedAnimalMap.begin(); outerIterator != _posessedAnimalMap.end(); ++outerIterator) {
		for (map<string, spAnimalModel>::iterator innerIterator = outerIterator->second.begin(); innerIterator != outerIterator->second.end(); ++innerIterator) {
			ZooSettings::instance.setAnimal(outerIterator->first, innerIterator->first, innerIterator->second->happinessValue(), innerIterator->second->hungerValue(), innerIterator->second->animalsCount(), innerIterator->second->lastFeedS(), innerIterator->second->getLevel());
		}
	}

	ZooSettings::instance.save();
}

void AnimalsManager::createFarmAnimals() {
	int sizeOfArray = sizeof(FARM) / sizeof(FARM[0]);
	const string regionName = "farm";

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
	const string regionName = "winter";

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
	const string regionName = "underwater";

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
	const string regionName = "steppe";

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
	const string regionName = "asia";

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
	const string regionName = "australia";

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

void AnimalsManager::addAnimalModel(const string& regionName, const string& name, int happiness, int hunger, int count, int lastFeedS, int level) {
	spAnimalModel model = new AnimalModel(name, happiness, hunger, count, lastFeedS);
	model->setLevel(level);
	model->fromContent();
	_animalsMap[regionName].insert(make_pair(name, model));

	
	if (count > 0) {
		_speciesPossesedCount += 1;
		_posessedAnimalMap[regionName].insert(make_pair(name, model));
	}
}

void AnimalsManager::updater(Event* event) {
	Timer::TimerEvent* timerEvent = static_cast<Timer::TimerEvent*>(event);
	int currentTime = getCurrentTimeInSeconds();

	for (map<string, map<string, spAnimalModel> >::iterator outerIterator = _posessedAnimalMap.begin(); outerIterator != _posessedAnimalMap.end(); ++outerIterator) {
		for (map<string, spAnimalModel>::iterator innerIterator = outerIterator->second.begin(); innerIterator != outerIterator->second.end(); ++innerIterator) {
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