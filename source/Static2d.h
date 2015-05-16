#ifndef _STATIC2D_
#define _STATIC2D_

#include "Box2D/Box2D.h"
#include "Entity.h"
#include "Actor.h"
#include "SpriteContainer.h"
#include "Main.h"

using namespace oxygine;

class Static2d : public Entity
{
public:
	b2Body* m_body;
	b2World* m_world;

	bool m_contacting;
	int m_numContacts;
public:
	Static2d(b2World* world, b2Body *body, string bodyName, float scale);
	~Static2d();

	void update(float playerPosition);
	void setBodyPosition();
	void startContact(b2Fixture *fixtureA, b2Fixture *fixtureB);
	void endContact(b2Fixture *fixtureA, b2Fixture *fixtureB);
	void restart();

	b2Body *getBody();
protected:
	Vector2 convert(const b2Vec2 &pos);
	b2Vec2 convert(const Vector2 &pos);
	void onTweenDone(Event *event);
private:
	float _scale;
	bool _wasTweenAdded;
	bool _wasTweenFinished;
	bool _wasOutTweenAdded;
};

#endif