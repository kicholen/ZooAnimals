#ifndef _ANIMALMODEL_
#define _ANIMALMODEL_

#include "oxygine-framework.h"

#define HAPPINESS_MULTIPLIER 0.55f

using namespace oxygine;

DECLARE_SMART(AnimalModel, spAnimalModel);

class AnimalModel : public Object
{
public:
	AnimalModel(const std::string& name, int happiness, int hunger, int count, int lastFeedS);
	~AnimalModel();

	void fromContent();
public:
	std::string animalName() { return _name; }
	int jumpHeight() const { return _jumpHeight; }
	int jumpRange() const { return _jumpRange; }
	int jumpTime() const { return _jumpTime; }
	const Vector2& jumpDelay() const { return _jumpDelay; }
	const bool isWaterAnimal() const { return _isWaterAnimal; }
	int hungerValue() const { return _hungerValue; }
	int happinessValue() const { return _happinessValue; }
	int happinessNeededForNextAnimal() const { return (int)ceilf(pow((float)getAnimalsCountFromHappiness() + 1.0f, 1.0f / 0.55f)); }
	int happinessNeededForCurrentAnimal() const { return (int)ceilf(pow((float)getAnimalsCountFromHappiness(), 1.0f / 0.55f)); }
	const Vector2& lockitIdsRange() const { return _lockitIdsRange; }

	void setHappiness(int value) { _happinessValue = value; }
	/* Inviolable count, use totalAnimalsCount */
	int animalsCount() const { return _count; } 
	void setAnimalsCount(int value) { _count = value; }

	/* this is total animal count, happiness + count */
	int totalAnimalsCount() const { return _count + getAnimalsCountFromHappiness(); }

	int lastFeedS() const { return _lastFeedS; }
	void setLastFeedS(int value) { _lastFeedS = value; }	

	const std::string& topGame() { return _topGame; }
	const std::string& midGame() { return _midGame; }
	const std::string& lowGame() { return _lowGame; }

	int lifespan() { return _lifespan; }
	int weight() { return _weight; }
	int groupLockit() { return _groupLockit; }
	int infoLockit() { return _infoLockit; }
	int nameLockit() { return _nameLockit; }

	int getGameValue(const std::string& name) {
		if (_topGame == name) {
			return 2;
		}
		else if (_midGame == name) {
			return 1;
		}
		else {
			return 0;
		}
	}

	int getLevel() const {
		return _level;
	}

	void setLevel(int value) {
		_level = value;
	}
private:
	int getAnimalsCountFromHappiness() const { return (int)floor(pow((float)_happinessValue, 0.55f)); }

private:
	std::string _name;
	int _jumpHeight;
	int _jumpRange;
	int _jumpTime;
	Vector2 _jumpDelay;
	bool _isWaterAnimal;
	Vector2 _lockitIdsRange;

	int _count;
	int _hungerValue;
	int _happinessValue;

	std::string _topGame;
	std::string _midGame;
	std::string _lowGame;

	int _lifespan;
	int _weight;
	int _groupLockit;
	int _infoLockit;
	int _nameLockit;

	int _level;

	int _lastFeedS;
};

#endif