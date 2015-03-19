#ifndef _ANIMALSMANAGER_
#define _ANIMALSMANAGER_

#include "FlashUtils.h"
#include "AnimalModel.h"
#include <map>
using namespace FlashUtils;

class AnimalsManager
{
public:
	static AnimalsManager instance;
	
	AnimalsManager();
	~AnimalsManager();

	void init(const string& version);

	spAnimalModel getAnimalModel(const string& name);
	spAnimalModel getAnimalModelByRegion(const string& region, const string& name);

	void store();
private:
	void createFarmAnimals();
	void createWinterAnimals();
	void createUnderwaterAnimals();
	void createSteppeAnimals();
	void createAsiaAnimals();
	void createAustraliaAnimals();

	void addAnimalModel(const string& regionName, const string& name, int happiness, int hunger, int count);
private:
	map<string, map<string, spAnimalModel>> _animalsMap;
};

#endif