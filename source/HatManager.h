#ifndef _HATMANAGER_
#define _HATMANAGER_

#include "oxygine-framework.h"
#include "FlashUtils.h"

using namespace FlashUtils;
using namespace oxygine;

// typedef std::map<std::string, spHatModel> hatMap; any sense of creating hat model? 
// what is needed? list of all hats
// every animal farm will have special 
// maybe hats in use? maybe not
// save as ints in saving file !

typedef std::map<std::string, int > hatsMap;

class HatManager : public EventDispatcher
{
public:
	static HatManager instance;

	struct hatParams {
		int scale;
		int offsetX;
		int offsetY;
	};

	HatManager();
	~HatManager();

	void init();
	void store();

	int getHatsCountPerAnimal(const std::string& animalName);
	void addWearableToAnimal(const std::string& animalName, const std::string& wearableName);
	std::string getWearable(const std::string& animalName, int hatIndex);
	std::string getRandomHatResource(const std::string& spriteName);
	hatParams* getHatParametersForAnimal(const std::string& hat, const std::string& spriteName);

private:
	void createHatList();
	void parseSettings();

private:
	VectorArray<std::string> _hatList;
	std::map<std::string, hatsMap > _hatsMap;
};

#endif