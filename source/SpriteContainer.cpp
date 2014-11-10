#include "SpriteContainer.h"


SpriteContainer::SpriteContainer() {
	groupArray = new VectorArray<Group*>();
}

void SpriteContainer::reset() {	
	for(int i = 0; i < groupArray->length(); i++) {
		Group &g = *(*groupArray)[i];
	}
}

void SpriteContainer::addGroup(SpriteModel &gd) {
	Group *g = new Group;
	double d[15] = { gd.scaleX, gd.scaleY, gd.positionX, gd.positionY, gd.zPriority };//todo

	memcpy(g->v, d, sizeof(d));
	g->spriteName = gd.spriteName;

	groupArray->push(g);
}

VectorArray<Group*>* SpriteContainer::getGroupData() {
	return groupArray;
}