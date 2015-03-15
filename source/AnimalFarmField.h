#ifndef _ANIMALFARMFIELD_
#define _ANIMALFARMFIELD_

#include "oxygine-framework.h"
#include "FlashUtils.h"
#include "AnimalInFarmElement.h"
#include "SharedResources.h"
#include "TileField.h"

#define SORT_Z_DELAY 500

using namespace FlashUtils;
using namespace oxygine;

DECLARE_SMART(AnimalFarmField, spAnimalFarmField);

typedef enum {afCreating, afWaiting, afAnimating} AnimalFarmState;

class AnimalFarmField : public Actor
{
public:
	AnimalFarmField(Vector2 fieldSize);
	~AnimalFarmField();

	void setData(string animalName, uint animalsCount);
	void addAnimal(Event *event);
	//void addAnimals();
	spTileField createTileField();

protected:
	virtual void doUpdate(const UpdateState &us);
private:
	void createCustomElements(spTileField tileField);
	VectorArray<int> getAnimalParameters(string animalName);
	spAnimalInFarmElement createAnimal(string animalNumber, string spriteName, float jumpRange, float jumpHeight, float jumpTime, Vector2 delayRandom, bool isWaterAnimal = false);
	void createAddAnimalButton(string buttonName, Vector2 position);
	void tryToAnimateAnimals(Vector2 position);
	bool canAnimalsAnimate();
	void setAnimalsPriorityByY();
	Vector2 getPositionFromCenter(float angle, Vector2 center, float distanceFromCenter);

	void onTouchOver(Event *event);

	VectorArray<spAnimalInFarmElement> _animalsArray;
	VectorArray<spSprite> _zSortElements;
	AnimalFarmState _state;
	float _animateDuration;
	float _lastZSortTime;
	int _count;
	string _species;
	bool _canAnimalsUpdate;

	int _animateType;
};


#endif