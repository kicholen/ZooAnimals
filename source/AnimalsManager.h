#ifndef _ANIMALSMANAGER_
#define _ANIMALSMANAGER_

#include "FlashUtils.h"
#include "AnimalModel.h"
#include <map>
#include "s3eTimer.h"
#include "Timer.h"

using namespace FlashUtils;

#define UPDATER_STEP_IN_SECONDS 10

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
		};

		AnimalEvent(EV ev, spAnimalModel model_) : Event(ev), model(model_) {}

		spAnimalModel model;
	};

	static AnimalsManager instance;
	
	AnimalsManager();
	~AnimalsManager();

	void init(const string& version);

	void feedAnimalByModel(spAnimalModel _model);
	void feedAnimalByName(const string& name);
	bool canAnimalBeFedByModel(spAnimalModel model);
	bool canAnimalBeFedByName(const string& name);

	spAnimalModel getAnimalModel(const string& name);
	spAnimalModel getAnimalModelByRegion(const string& region, const string& name);
	map<string, spAnimalModel>& getAnimalsByRegion(const string& region);

	map<string, spAnimalModel>& getPossesedAnimalsByRegion(const string& region);
	map<string, map<string, spAnimalModel> >& getPossesedAnimals();

	void increaseHappinessByPoints(spAnimalModel model, int points);

	bool isRegionPopulated(const string& regionName);

	void store();
private:
	void createFarmAnimals();
	void createWinterAnimals();
	void createUnderwaterAnimals();
	void createSteppeAnimals();
	void createAsiaAnimals();
	void createAustraliaAnimals();
	void createTimer();

	void addAnimalModel(const string& regionName, const string& name, int happiness, int hunger, int count, int lastFeedS, int level = -1);

	void updater(Event* event);

	void decreaseHappinessByPercent(spAnimalModel model, int percent);

	void dispatchAnimalCountChangedEvent(spAnimalModel model);
	void dispatchAnimalCanBeFedEvent(spAnimalModel model);

	int getCurrentTimeInSeconds();

private:
	map<string, map<string, spAnimalModel> > _animalsMap;
	map<string, map<string, spAnimalModel> > _posessedAnimalMap;

	int _speciesPossesedCount;

	spTimer _timer;
};

#endif