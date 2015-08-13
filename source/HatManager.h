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

	bool canPlaceHat();
	std::string getRandomHatResource(const std::string spriteName);
	hatParams* getHatParametersForAnimal(const std::string hat, const std::string spriteName);

private:
	void createHatList();

private:
	VectorArray<std::string> _hatList;
};

#endif