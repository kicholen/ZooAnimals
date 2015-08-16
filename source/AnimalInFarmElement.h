#ifndef _ANIMALINFARMELEMENT_
#define _ANIMALINFARMELEMENT_

#include "oxygine-framework.h"

#define ANIMAL_PERCENT_SIZE 7.0f

using namespace oxygine;

DECLARE_SMART(AnimalInFarmElement, spAnimalInFarmElement);

typedef enum {aifCreating, aifWaiting, aifJumping, aifStopped} AnimalInFarmElementState;

class AnimalInFarmElement : public Actor
{
public:
	AnimalInFarmElement(const std::string& spriteName, const Vector2& size, float jumpRange, float jumpHeight, float jumpTime, const Vector2& delayRandom, bool isWaterAnimal);
	~AnimalInFarmElement();

	void setAnimalSprite(const std::string& spriteName);
	void animateJump(const Vector2& position, bool isRandom = false);
	void attachWearable(const std::string& hatName, const std::string& animalName);
	void setJumpSpeedFactor(float value) {
		_jumpSpeedFactor = value;
	}
	float getShadowY();
	float getShadowX();
	float getJumpTime();

	void addTooltipToAnimal(spActor actor);
	void setAsLeader();
	void jumpToExactPosition(const Vector2& exactPosition);
	void stopJumpingExact();
	void stopJumping();
	void resumeJumping();

protected:
	void doUpdate(const UpdateState &us);
private:
	void createAnimalSprite(const std::string& spriteName);
	void createShadowSprite();

	void animateAppear();
	void animateDisappear();
	void jumpToPosition(const Vector2& position);

	const Vector2& getRandomPointOnRectangleEdge();
	void drawDebugLineAndJumpTo(const Vector2& destinedPosition);
	Vector2 checkAndChangePointIfNeeded(Vector2 position);
	Vector2 calculateJumpPosition(Vector2 position);
	bool canJump();

	// callbacks
	void onJumpEnded(Event *event);

	int _calculateNewPointCounter;
	Vector2 _randomPointOnEdge;
	bool _shouldCalculateNewPoint;
	float _nextJumpDelay;
	AnimalInFarmElementState _state;
	spSprite _animalSprite;
	spSprite _debugJumpLine;
	spSprite _shadowSprite;

private:
	float _jumpHeight;
	float _jumpRange;
	float _jumpTime;
	Vector2 _jumpDelay;
	bool _isWaterAnimal;

	Vector2 _exactPosition;
	bool _isJumpingExact;

	float _jumpSpeedFactor;
public:
	bool _canUpdate;
};

#endif
