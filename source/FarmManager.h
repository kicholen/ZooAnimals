#ifndef _FARMMANAGER_
#define _FARMMANAGER_

#include "FlashUtils.h"
#include "AnimalModel.h"

using namespace FlashUtils;

class FarmManager : public EventDispatcher
{
public:
	static FarmManager instance;

	FarmManager();
	~FarmManager();

	void init();

	std::string getFarmLevelByModel(spAnimalModel model);
	int getGateHumanCount();
	int getFarmHumanCount();
	int getGateShopHumanCount();
	std::string getZooGateLevel(int regionsUnblocked);

	int getZooSize();
};

#endif