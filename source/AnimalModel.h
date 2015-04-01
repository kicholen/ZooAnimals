#ifndef _ANIMALMODEL_
#define _ANIMALMODEL_

#include "oxygine-framework.h"

using namespace oxygine;

DECLARE_SMART(AnimalModel, spAnimalModel);

class AnimalModel : public Object
{
public:
	AnimalModel(const string& name, int happiness, int hunger, int count);
	~AnimalModel();

	void fromContent();
public:
	string animalName() { return _name; }
	int jumpHeight() const { return _jumpHeight; }
	int jumpRange() const { return _jumpRange; }
	int jumpTime() const { return _jumpTime; }
	const Vector2& jumpDelay() const { return _jumpDelay; }
	const bool isWaterAnimal() const { return _isWaterAnimal; }
	int hungerValue() const { return _hungerValue; }
	int happinessValue() const { return _happinessValue; }
	int animalsCount() const { return _count; }
	void setAnimalsCount(int value) { _count = value; }

private:
	string _name;
	int _jumpHeight;
	int _jumpRange;
	int _jumpTime;
	Vector2 _jumpDelay;
	bool _isWaterAnimal;
	
	int _count;
	int _hungerValue;
	int _happinessValue;
};

#endif