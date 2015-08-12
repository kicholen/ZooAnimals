#ifndef _HATMANAGER_
#define _HATMANAGER_

#include "oxygine-framework.h"
#include "FlashUtils.h"
#include "AnimalModel.h"

using namespace FlashUtils;
using namespace oxygine;

// typedef std::map<std::string, spHatModel> hatMap; any sense of creating hat model? 
// what is needed? list of all hats
// every animal farm will have special 
// maybe hats in use? maybe not
// save as ints in saving file !
class HatManager : public EventDispatcher
{
public:
	static HatManager instance;

	HatManager();
	~HatManager();

	void init();

	bool canPlaceHat();
	std::string getRandomHatResource(spAnimalModel model);
	void getHatParametersForAnimal(const std::string hat, spAnimalModel model);

private:
	void createHatList();

private:
	VectorArray<std::string> _hatList;
};

#endif