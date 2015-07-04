#ifndef _ANIMALSLOT_
#define _ANIMALSLOT_

#include "oxygine-framework.h"

#define SLOT_SIZE_X 70
#define SLOT_SIZE_Y 70

using namespace oxygine;

DECLARE_SMART(AnimalSlot, spAnimalSlot);

class AnimalSlot : public Actor
{
public:
	AnimalSlot();
	AnimalSlot(const std::string& id);

	void switchAnimalSprite(const std::string& id);
	std::string getId();

protected:
	void createAnimalSprite();
	void createBackground();
private:
	std::string _id;

	spSprite _animalSprite;
};

#endif