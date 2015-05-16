#ifndef _DYNAMIC2D_
#define _DYNAMIC2D_

#include "Box2D/Box2D.h"
#include "Entity.h"
#include "Actor.h"

using namespace oxygine;

class Dynamic2d : public Entity
{
public:
	b2Body* m_body;
	b2World* m_world;

	bool m_contacting;
	int m_numContacts;
public:
	Dynamic2d(b2World* world, b2Body *body, string bodyName);
	~Dynamic2d();
	
	void update();
	void startContact(b2Fixture *fixtureA, b2Fixture *fixtureB);
	void endContact(b2Fixture *fixtureA, b2Fixture *fixtureB);
	void restart();

	b2Body *getBody();
};

#endif