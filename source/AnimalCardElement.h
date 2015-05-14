#ifndef _ANIMALCARDELEMENT_
#define _ANIMALCARDELEMENT_

#include "oxygine-framework.h"
#include "AnimalModel.h"
#include "AnimalSlot.h"

using namespace oxygine;

#define ANIMALSLOT_WIDTH_PERCENT 40.0f

DECLARE_SMART(AnimalCardElement, spAnimalCardElement);

class AnimalCardElement : public Actor
{
public:
	AnimalCardElement(const Vector2& size, spAnimalModel model);
	~AnimalCardElement();

	void switchAnimalModel(spAnimalModel model);
private:
	void setData(spAnimalModel model);

	void setAnimalSlot(const string& spriteName);

	void createBackground();

	spTextField createText(int lockitId, const Vector2& boundries, bool multiline = false);

private:
	spAnimalSlot _animalSlot;
};

#endif