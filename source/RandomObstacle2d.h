#ifndef _RANDOMOBSTACLE2D_
#define _RANDOMOBSTACLE2D_

#include "Box2D/Box2D.h"
#include "Entity.h"
#include "Actor.h"

using namespace oxygine;

class RandomObstacle2d : public Entity
{
public:
	b2Body* m_body;
	b2World* m_world;

	bool m_contacting;
	int m_numContacts;
public:
	RandomObstacle2d(b2World* world, b2Body *body, std::string bodyName, float scale);
	~RandomObstacle2d();

	void update(float playerPosition);
	void startContact(b2Fixture *fixtureA, b2Fixture *fixtureB);
	void endContact(b2Fixture *fixtureA, b2Fixture *fixtureB);
	void restart();

	b2Body *getBody();

protected:
	Vector2 convert(const b2Vec2 &pos);
	b2Vec2 convert(const Vector2 &pos);
	void onTweenDone(Event *event);

private:
	Vector2 _startPosition;
	float _scale;

	bool _isTweenInProgress;
	
	bool _wasStartPositionSet;
};

#endif