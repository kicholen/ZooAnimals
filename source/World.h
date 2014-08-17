#pragma once
#include "SpriteContainer.h"
#include "FlashUtils.h"

using namespace FlashUtils;

class World
{
public:
	static int levelNumber;
	static double lastStart;

	World();

	//Vars:
	Array<SpriteContainer*> *waveArray;
	double waveDelay;

	//Funcs:
	Array<Group*>* getLevelData();

	void addWave(SpriteContainer *wave);

	void Reset();
};

#pragma once