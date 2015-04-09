#ifndef _ANIMALMODEL_
#define _ANIMALMODEL_

#include "oxygine-framework.h"

#define HAPPINESS_MULTIPLIER 0.55f

using namespace oxygine;

DECLARE_SMART(AnimalModel, spAnimalModel);

class AnimalModel : public Object
{
public:
	AnimalModel(const string& name, int happiness, int hunger, int count, int lastFeedS);
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
	int happinessNeededForNextAnimal() const { return (int)ceilf(pow((float)getAnimalsCountFromHappiness() + 1.0f, 1 / 0.55f)); }
	int happinessNeededForCurrentAnimal() const { return (int)ceilf(pow((float)getAnimalsCountFromHappiness(), 1 / 0.55f)); }

	void setHappiness(int value) { _happinessValue = value; }
	/* Inviolable count, use totalAnimalsCount */
	int animalsCount() const { return _count; } 
	void setAnimalsCount(int value) { _count = value; }

	/* this is total animal count, happiness + count */
	int totalAnimalsCount() const { return _count + getAnimalsCountFromHappiness(); }

	int lastFeedS() const { return _lastFeedS; }
	void setLastFeedS(int value) { _lastFeedS = value; }	

	const string& topGame() { return _topGame; }
	int topGameValue() const { return _topGameValue; }
	const string& midGame() { return _midGame; }
	int midGameValue() const { return _midGameValue; }
	const string& lowGame() { return _lowGame; }
	int lowGameValue() const { return _lowGameValue; }

private:
	int getAnimalsCountFromHappiness() const { return (int)floor(pow((float)_happinessValue, 0.55f)); }

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

	string _topGame;
	int _topGameValue;
	string _midGame;
	int _midGameValue;
	string _lowGame;
	int _lowGameValue;

	int _lastFeedS;
};

#endif