#ifndef _ANIMALMODEL_
#define _ANIMALMODEL_

#include "oxygine-framework.h"

using namespace oxygine;

DECLARE_SMART(AnimalModel, spAnimalModel);

class AnimalModel : public Object
{
public:
	AnimalModel(const string& name);
	~AnimalModel();

	void fromContent();
public:
	string animalName() { return _name; }
	int jumpHeight() const { return _jumpHeight; }
	int jumpRange() const { return _jumpRange; }
	int jumpTime() const { return _jumpTime; }
	const Vector2& jumpDelay() const { return _jumpDelay; }
	const bool isWaterAnimal() const { return _isWaterAnimal; }

private:
	string _name;
	int _jumpHeight;
	int _jumpRange;
	int _jumpTime;
	Vector2 _jumpDelay;
	bool _isWaterAnimal;
	
	float _hungerValue;
	float _tirednessValue;
};

#endif