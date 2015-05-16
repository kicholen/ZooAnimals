#ifndef _ENTITY_
#define _ENTITY_

#include "Box2D\Box2D.h"
#include "Sprite.h"
#include "string"

class Entity {
public:
	std::string _bodyName;
	oxygine::spSprite _sprite;

public:
    virtual ~Entity() {};
	virtual void setBodyPosition() {};
	virtual void update(float playerPosition) {};
	virtual void startContact(b2Fixture *fixtureA, b2Fixture *fixtureB) {};
	virtual void endContact(b2Fixture *fixtureA, b2Fixture *fixtureB) {};
	virtual void restart() {};
	virtual b2Body* getBody() { 
		b2Body *dupa; 
		return dupa;
	};
};

#endif