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

#define BASE_SIZE_IN_PERCENT 5
#define EXPANDED_SIZE_IN_PERCENT 15
#define OFFSET_EDGES 1
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

	spAnimalModel getModel() {
		return _model;
	}

protected:
	virtual void doUpdate(const UpdateState &us);
private:
	void createCustomElements(spTileField tileField);
	spAnimalInFarmElement createAnimal(const string& animalNumber, spAnimalModel model);
	spButton createAnimalButton(const string& buttonName, Vector2 position);
	void animateAnimalsJump(Vector2 position);
	bool canAnimalsAnimate();
	void setAnimalsPriorityByY();

	void onTouchOver(Event *event);
	void onGameChosen(Event *event);

	VectorArray<spSprite> _zSortElements;
	AnimalFarmState _state;
	spAnimalFarmPanel _animalPanel;

	AnimalsFarmAnimationType _animationType;
	spAnimalsFarmAnimations _animalsFarmAnimation;
	spAnimalModel _model;

	int _count;
};


#endif