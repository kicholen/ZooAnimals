#include "World.h"

using namespace FlashUtils;

int World::levelNumber = 0;
double World::lastStart = 0;

World::World() {
	waveArray = new Array<SpriteContainer*>();
}

void World::addWave(SpriteContainer *wave) {
	waveArray->push(wave);
}

Array<Group*>* World::getLevelData() {
	int i = 0;
	return (*waveArray)[i]->getGroupData();
}

void World::Reset() {
	levelNumber = 0;
	for(int i=0; i<waveArray->length(); i++) {
		(*waveArray)[i]->reset();
	}
}
