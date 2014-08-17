#ifndef _ANIMALSLOT_
#define _ANIMALSLOT_

#include "oxygine-framework.h"

#define SLOT_SIZE_X 75
#define SLOT_SIZE_Y 75

using namespace oxygine;

DECLARE_SMART(AnimalSlot, spAnimalSlot);

class AnimalSlot : public Actor
{
public:
	AnimalSlot(string id);

	void switchAnimalSprite(string id);
	string getId();

protected:
	void createAnimalSprite();
	void createBackground();
private:
	string _id;

	spSprite _animalSprite;
};

#endif