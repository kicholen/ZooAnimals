#ifndef _ANIMALSFARMANIAMTIONS_
#define _ANIMALSFARMANIMATIONS_

#include "oxygine-framework.h"
#include "AnimalInFarmElement.h"
#include "FlashUtils.h"

#define SORT_Z_DELAY_MIN 500
#define SORT_Z_DELAY_MAX 1000

using namespace FlashUtils;
using namespace oxygine;

DECLARE_SMART(AnimalsFarmAnimations, spAnimalsFarmAnimations);

typedef enum {afaNone, afaVertical, afaHorizontal, afaCircle, afaSmile, afaSnake, afaCount} AnimalsFarmAnimationType;

class AnimalsFarmAnimations : public Object
{
public:
	AnimalsFarmAnimations(const Vector2& fieldSize);
	~AnimalsFarmAnimations();

	void addAnimal(spAnimalInFarmElement animal);
	spAnimalInFarmElement removeAnimal();
	void animateAnimalsJump(Vector2 position);
	void playAnimalsAnimation(AnimalsFarmAnimationType animationType);
	void doUpdate(const UpdateState &us, bool isOnScreen);

	int getAnimalsCount();
private:
	Vector2 getPositionFromCenter(float angle, Vector2 center, float distanceFromCenter);
	void setAnimalsPriorityByY();

	void playAnimalsAnimation();
	void playVerticalAnimation();
	void playHorizontalAnimation();
	void playCircleAnimation();
	void playSmileAnimation();
	void playSnakeAnimation();
	void stopJumpingAnimation();
	
	void moveSnakeToNextAnimalPosition();
	bool isSnakeCompleted();
	bool canSnakeMoveToNextAnimal();

private:
	Vector2 _playFieldSize;
	VectorArray<spAnimalInFarmElement> _animalsArray;
	float _lastZSortTime;
	float _updateTime;
	bool _canAnimalsUpdate;

	float _nextSnakeUpdate;
	int _snakeLength;
	Vector2 _targetPosition;
};

#endif