#ifndef _ANIMALFARMFIELD_
#define _ANIMALFARMFIELD_

#include "oxygine-framework.h"
#include "FlashUtils.h"
#include "AnimalInFarmElement.h"
#include "SharedResources.h"

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
protected:
	virtual void doUpdate(const UpdateState &us);
private:
	spAnimalInFarmElement createAnimal(string animalNumber, string spriteName);
	void createAddAnimalButton(string buttonName, Vector2 position);
	void tryToAnimateAnimals(Vector2 position);
	bool canAnimalsAnimate();
	void setAnimalsPriorityByY();

	void onTouchOver(Event *event);

	Array<spAnimalInFarmElement> _animalsArray;
	AnimalFarmState _state;
	float _animateDuration;
	float _lastAnimalsAnimateTime;
	int _count;
	string _species;
};


#endif