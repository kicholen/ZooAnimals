#ifndef _ANIMALFARMFIELD_
#define _ANIMALFARMFIELD_

#include "oxygine-framework.h"
#include "FlashUtils.h"
#include "AnimalInFarmElement.h"
#include "SharedResources.h"
#include "TileField.h"
#include "AnimalsFarmAnimations.h"
#include "AnimalModel.h"
#include "AnimalFarmPanel.h"

using namespace FlashUtils;
using namespace oxygine;

DECLARE_SMART(AnimalFarmField, spAnimalFarmField);

typedef enum {afCreating, afWaiting, afAnimating} AnimalFarmState;

class AnimalFarmField : public Actor
{
public:
	AnimalFarmField(Vector2 fieldSize);
	~AnimalFarmField();

	void setData(spAnimalModel model);
	void addAnimal(Event *event);
	spTileField createTileField();
	void playNextAnimalsAnimation(Event *event);

protected:
	virtual void doUpdate(const UpdateState &us);
private:
	void createCustomElements(spTileField tileField);
	spAnimalInFarmElement createAnimal(const string& animalNumber, spAnimalModel model);//const string& spriteName, float jumpRange, float jumpHeight, float jumpTime, Vector2 delayRandom, bool isWaterAnimal = false);
	spButton createAnimalButton(const string& buttonName, Vector2 position);
	void animateAnimalsJump(Vector2 position);
	bool canAnimalsAnimate();
	void setAnimalsPriorityByY();

	void onTouchOver(Event *event);

	VectorArray<spSprite> _zSortElements;
	AnimalFarmState _state;
	spAnimalFarmPanel _animalPanel;

	AnimalsFarmAnimationType _animationType;
	spAnimalsFarmAnimations _animalsFarmAnimation;
	spAnimalModel _model;

	int _count;
};


#endif