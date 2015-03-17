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

	void init();

	spAnimalModel getAnimalModel(const string& name);

private:
	map<string, spAnimalModel> _animalsMap;
};

#endif