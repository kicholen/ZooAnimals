#ifndef _ANIMALINFARMELEMENT_
#define _ANIMALINFARMELEMENT_

#include "oxygine-framework.h"

#define ANIMAL_IN_FARM_SIZE_X 20
#define ANIMAL_IN_FARM_SIZE_Y 20
#define JUMP_HEIGHT 20
#define JUMP_RANGE 30
#define JUMP_TIME 2000
#define JUMP_DELAY_MIN 500
#define JUMP_DELAY_MAX 1000

using namespace oxygine;

DECLARE_SMART(AnimalInFarmElement, spAnimalInFarmElement);

typedef enum {aifCreating, aifWaiting, aifJumping} AnimalInFarmElementState;

class AnimalInFarmElement : public Actor
{
public:
	AnimalInFarmElement(string spriteName, Vector2 size);
	~AnimalInFarmElement();

	void setAnimalSprite(string spriteName);
	void animateJump(Vector2 position, bool isRandom = false);
	float getShadowY();
protected:
	virtual void doUpdate(const UpdateState &us);
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
};

#endif
