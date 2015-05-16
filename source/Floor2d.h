#ifndef _FLOOR2D_
#define _FLOOR2D_

#include "Box2D/Box2D.h"
#include "Entity.h"
#include "Actor.h"
//#include "SpriteContainer.h"
#include "Main.h"

using namespace oxygine;

class Floor2d : public Entity
{
public:
	b2Body* m_body;
	b2World* m_world;

	bool m_contacting;
	int m_numContacts;
public:
	Floor2d(b2World* world, b2Body *body, string bodyName, float scale);
	~Floor2d();

	void update(float playerPosition);
	void startContact(b2Fixture *fixtureA, b2Fixture *fixtureB);
	void endContact(b2Fixture *fixtureA, b2Fixture *fixtureB);
	void restart();

	b2Body *getBody();
	void addPlatformSprite();
protected:
	Vector2 convert(const b2Vec2 &pos);
	void onTweenDone(Event *event);
	void fillStartPosition();
private:
	float _scale;
	bool _wasTweenAdded;
	bool _wasTweenFinished;
	bool _wasOutTweenAdded;
	float _blockWidth;
	Vector2 _startPosition;
	Vector2 _savedStartPosition;
};

#endif