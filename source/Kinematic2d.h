#ifndef _KINEMATIC2D_
#define _KINEMATIC2D_

#include "Box2D/Box2D.h"
#include "Entity.h"
#include "Actor.h"

using namespace oxygine;

class Kinematic2d : public Entity
{
public:
	b2Body* m_body;
	b2World* m_world;

	bool m_contacting;
	int m_numContacts;
public:
	Kinematic2d(b2World* world, b2Body *body, std::string bodyName);
	~Kinematic2d();
	
	void update(float playerPosition);
	void startContact(b2Fixture *fixtureA, b2Fixture *fixtureB);
	void endContact(b2Fixture *fixtureA, b2Fixture *fixtureB);
	void restart();

	b2Body *getBody();
};

#endif