#ifndef _ANIMALSMANAGER_
#define _ANIMALSMANAGER_

#include "FlashUtils.h"
#include "AnimalModel.h"
#include <map>
#include "s3eTimer.h"
#include "Timer.h"

using namespace FlashUtils;

#define UPDATER_STEP_IN_SECONDS 10

typedef std::map<std::string, spAnimalModel> animalMap;

class AnimalsManager : public EventDispatcher
{
public:
	class AnimalEvent : public Event
	{
	public:
		enum EV
		{
			COUNT_CHANGED = makefourcc('A', 'M', 'C', 'C'),
			CAN_FEED = makefourcc('A', 'M', 'C', 'F'),
			ANIMAL_FED = makefourcc('A', 'M', 'C', 'F')
		};

		AnimalEvent(EV ev, spAnimalModel model_) : Event(ev), model(model_) {}

		spAnimalModel model;
	};

	static AnimalsManager instance;
	
	AnimalsManager();
	~AnimalsManager();

	void init(const std::string& version);

	void feedAnimalByModel(spAnimalModel _model, int feedTime = 0);
	void feedAnimalByName(const std::string& name);
	bool canAnimalBeFedByModel(spAnimalModel model);
	bool canAnimalBeFedByName(const std::string& name);

	spAnimalModel getAnimalModel(const std::string& name);
	spAnimalModel getAnimalModelByRegion(const std::string& region, const std::string& name);
	const animalMap& getAnimalsByRegion(const std::string& region);

	const animalMap& getPossesedAnimalsByRegion(const std::string& region);
	const std::map< std::string, animalMap >& getPossesedAnimals() const;

	void increaseHappinessByPoints(spAnimalModel model, int points);
	void increaseAnimalCount(const std::string& region, const std::string& name, int count);
	void increaseAnimalCount(spAnimalModel model, int count);

	bool isRegionPopulated(const std::string& regionName);

	void store();
private:
	void createFarmAnimals();
	void createWinterAnimals();
	void createUnderwaterAnimals();
	void createSteppeAnimals();
	void createAsiaAnimals();
	void createAustraliaAnimals();
	void createTimer();

	void addAnimalModel(const std::string& regionName, const std::string& name, int happiness, int hunger, int count, int lastFeedS, int level = -1);
	const std::string& getAnimalRegion(spAnimalModel model);

	void updater(Event* event);

	void decreaseHappinessByPercent(spAnimalModel model, int percent);

	void dispatchAnimalCountChangedEvent(spAnimalModel model);
	void dispatchAnimalCanBeFedEvent(spAnimalModel model);
	void dispatchAnimalFedEvent(spAnimalModel model);
	
	int getCurrentTimeInSeconds();

private:
	std::map<std::string, animalMap > _animalsMap;
	std::map<std::string, animalMap > _posessedAnimalMap;

	int _speciesPossesedCount;
	int _animalsPossesedCount;

	spTimer _timer;
};

#endif