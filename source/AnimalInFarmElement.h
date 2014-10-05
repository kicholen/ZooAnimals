#ifndef _ANIMALINFARMELEMENT_
#define _ANIMALINFARMELEMENT_

#include "oxygine-framework.h"

#define ANIMAL_PERCENT_SIZE 5

using namespace oxygine;

DECLARE_SMART(AnimalInFarmElement, spAnimalInFarmElement);

typedef enum {aifCreating, aifWaiting, aifJumping} AnimalInFarmElementState;

class AnimalInFarmElement : public Actor
{
public:
	AnimalInFarmElement(string spriteName, Vector2 size, float jumpRange, float jumpHeight, float jumpTime, Vector2 delayRandom, bool isWaterAnimal);
	~AnimalInFarmElement();

	void setAnimalSprite(string spriteName);
	void animateJump(Vector2 position, bool isRandom = false);
	float getShadowY();
protected:
	void doUpdate(const UpdateState &us);
private:
	void createAnimalSprite();
	void createShadowSprite();

	void animateAppear();
	void animateDisappear();
	void jumpToPosition(Vector2 position);

	Vector2 getRandomPointOnRectangleEdge();
	void drawDebugLineAndJumpTo(Vector2 destinedPosition);
	Vector2 checkAndChangePointIfNeeded(Vector2 position);
	Vector2 calculateJumpPosition(Vector2 position);

	// callbacks
	void onJumpEnded(Event *event);

	int _calculateNewPointCounter;
	Vector2 _randomPointOnEdge;
	bool _shouldCalculateNewPoint;
	float _nextJumpDelay;
	AnimalInFarmElementState _state;
	string _id;
	spSprite _animalSprite;
	spSprite _debugJumpLine;
	spSprite _shadowSprite;

private:
	float _jumpHeight;
	float _jumpRange;
	float _jumpTime;
	Vector2 _jumpDelay;
	bool _isWaterAnimal;

public:
	bool _canUpdate;
};

#endif
