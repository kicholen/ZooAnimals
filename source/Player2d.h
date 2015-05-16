#ifndef _PLAYER2D_
#define _PLAYER2D_

#include "Box2D/Box2D.h"
#include "Entity.h"

#define ROT_PER_SECOND 180
using namespace oxygine;

class Player2d : public Entity
{
public:
	b2Body* m_body;
	b2World* m_world;

public:
	Player2d(b2World* world, b2Body *body, string bodyName, float scale);
	~Player2d();

	void update(float playerPosition);
	void restart();
	void setBodyPosition();

	void startContact(b2Fixture *fixtureA, b2Fixture *fixtureB);
	void endContact(b2Fixture *fixtureA, b2Fixture *fixtureB);

	void startJump();
	void stopJump();

	b2Body *getBody();

protected:
	void tryToJump();
	void onGroundTouchSetDefaultRotation();
	b2Vec2 convertForce(const Vector2 &force);
	Vector2 convert(const b2Vec2 &position);
	b2Vec2 convert(const Vector2 &pos);
private:
	bool _contacting;
	int _numContacts;
	b2Vec2 _actualVelocity;
	float32 _impulse;
	int _touchHold;
	float32 _force;
	bool _canJump;
	bool _minJump;
	int _jumpTimeout;
	bool _shouldJump;
	b2Vec2 _startPosition;
	float _scale;
	int _frameCounter;

	bool _jumpOnThisFrame;
};

#endif