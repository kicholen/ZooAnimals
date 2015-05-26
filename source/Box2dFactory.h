#ifndef _BOX2DFACTORY_
#define _BOX2DFACTORY_

#include "oxygine-framework.h"
#include "Box2D\Box2D.h"
#include "Entity.h"
#include "FlashUtils.h"

using namespace FlashUtils;
using namespace oxygine;

DECLARE_SMART(Box2dFactory, spBox2dFactory);

typedef enum { staticBody, kinematicBody, dynamicBody } BodyType;
typedef enum { circleShape, rectangleShape } FixtureShape;
typedef enum EntityType { player2d, static2d, kinematic2d, dynamic2d, floor2d, randomObstacle2d };

class Box2dFactory : public Object
{
public:
	Box2dFactory(b2World *world, spActor oxyWorld, float scale);

	Entity* createEntity(EntityType type, const Vector2& position, BodyType bodyType, bool bullet);
	Entity* createEntity(EntityType type, const Vector2& position, BodyType bodyType, bool bullet, const Vector2& size);

protected:
	void getPlayer();
	void getDynamic();
	void getStatic();
	void getKinematic();

private:
	void attachFixture(b2Body* body, VectorArray<Vector2*> *oxyVertices);
	void attachFixture(b2Body* body, float radius, const Vector2& position);
	void attachFixture(b2Body* body, const Vector2& size);

	b2CircleShape* createCircleShape(const Vector2& position, float radius);
	b2PolygonShape* createPolygonShape(VectorArray<Vector2*> *oxyVertices);
	b2PolygonShape* createPolygonShape(const Vector2& size);

	b2Body* createBody(const Vector2& position, BodyType bodyType, bool bullet);
	b2FixtureDef* createFixture(double density = 1.0f, double friction = 1.0f, bool isSensor = false);

	spSprite createSprite(ResAnim* res, const Vector2& size, bool shouldScale = true);

	b2Vec2 convertBody(const Vector2 &pos);
	Vector2 convertBody(const b2Vec2 &pos);

	b2Vec2 convertVertices(const Vector2 &pos);
	Vector2 convertVertices(const b2Vec2 &pos);

	b2Vec2 convert(const Vector2 &pos);
	Vector2 convert(const b2Vec2 &pos);
	//void addRevJoint(b2Body* bodyA, b2Body* bodyB);
private:
	b2World *_world;
	spActor _oxyWorld;

	float SCALE;
};

#endif