#include "Box2dFactory.h"
#include "SharedResources.h"
#include "Player2d.h"
#include "Static2d.h"
#include "Kinematic2d.h"
#include "Dynamic2d.h"
#include "Floor2d.h"
#include "RandomObstacle2d.h"

Box2dFactory::Box2dFactory(b2World *world, spActor oxyWorld, float scale) {
	_world = world;
	_oxyWorld = oxyWorld;

	SCALE = scale;
}

Entity* Box2dFactory::createEntity(int type, const Vector2& position, int bodyType, bool bullet) {
	b2Body* body = createBody(position, bodyType, bullet);

	if (type == player2d) {
		ResAnim* resource = animalsResources.getResAnim("bee");
		spSprite sprite = createSprite(resource, Vector2(0, 0), false);
		float ratio = sprite->getWidth() / sprite->getHeight();
		
		attachFixture(body, Vector2(20.0f, 20.0f * ratio));
		Player2d* player = new Player2d(_world, body, "player", SCALE);
		b2AABB aaBb = body->GetFixtureList()->GetAABB(0);
		Vector2 playerSize = convert(b2Vec2(aaBb.upperBound.x - aaBb.lowerBound.x, abs(aaBb.upperBound.y - aaBb.lowerBound.y)));
		
		sprite->setScale(playerSize.x / sprite->getWidth(), playerSize.y / sprite->getHeight());
		player->_sprite = sprite;

		player->_sprite->attachTo(_oxyWorld);
		player->_sprite->setUserData(body);

		return player;
	}
	else if (type == static2d) {
		attachFixture(body, Vector2(30.0f, 30.0f));

		Static2d* staticObject = new Static2d(_world, body, "static", SCALE);

		b2AABB aaBb = body->GetFixtureList()->GetAABB(0);
		Vector2 staticSize = convert(b2Vec2(aaBb.upperBound.x - aaBb.lowerBound.x, abs(aaBb.upperBound.y - aaBb.lowerBound.y)));

		staticObject->_sprite = createSprite(gameResources.getResAnim("quad_40"), staticSize);

		staticObject->_sprite->attachTo(_oxyWorld);
		staticObject->_sprite->setUserData(body);

		return staticObject;

	}
	else if (type == kinematic2d) {
		Kinematic2d* kinematicObject = new Kinematic2d(_world, body, "kinematic");

		b2AABB aaBb = body->GetFixtureList()->GetAABB(0);
		Vector2 size = convert(b2Vec2(aaBb.upperBound.x - aaBb.lowerBound.x, abs(aaBb.upperBound.y - aaBb.lowerBound.y)));

		kinematicObject->_sprite = createSprite(gameResources.getResAnim("quad_40"), size);

		kinematicObject->_sprite->attachTo(_oxyWorld);
		kinematicObject->_sprite->setUserData(body);

		return kinematicObject;
	}
	else if (type == floor2d) {
		attachFixture(body, Vector2(500.0f, 20.0f));

		Floor2d* floorObject = new Floor2d(_world, body, "floor", SCALE);
		b2AABB aaBb = body->GetFixtureList()->GetAABB(0);
		Vector2 staticSize = convert(b2Vec2(aaBb.upperBound.x - aaBb.lowerBound.x, abs(aaBb.upperBound.y - aaBb.lowerBound.y)));

		floorObject->_sprite = createSprite(gameResources.getResAnim("quad_40"), staticSize);

		floorObject->_sprite->attachTo(_oxyWorld);
		floorObject->_sprite->setUserData(body);

		return floorObject;
	}
	else if (type == randomObstacle2d) {
		attachFixture(body, Vector2(50.0f, 20.0f));

		RandomObstacle2d* obstacle = new RandomObstacle2d(_world, body, "obstacle", SCALE);
		b2AABB aaBb = body->GetFixtureList()->GetAABB(0);
		Vector2 staticSize = convert(b2Vec2(aaBb.upperBound.x - aaBb.lowerBound.x, abs(aaBb.upperBound.y - aaBb.lowerBound.y)));

		obstacle->_sprite = createSprite(gameResources.getResAnim("quad_40"), staticSize);

		obstacle->_sprite->attachTo(_oxyWorld);
		obstacle->_sprite->setUserData(body);

		return obstacle;
	}
	else  {
		Dynamic2d* dynamicObject = new Dynamic2d(_world, body, "dynamic");

		b2AABB aaBb = body->GetFixtureList()->GetAABB(0);
		Vector2 size = convert(b2Vec2(aaBb.upperBound.x - aaBb.lowerBound.x, abs(aaBb.upperBound.y - aaBb.lowerBound.y)));

		dynamicObject->_sprite = createSprite(gameResources.getResAnim("quad_40"), size);

		dynamicObject->_sprite->setInputChildrenEnabled(false);
		dynamicObject->_sprite->attachTo(_oxyWorld);
		dynamicObject->_sprite->setUserData(body);

		return dynamicObject;
	}
}

Entity* Box2dFactory::createEntity(int type, const Vector2& position, int bodyType, bool bullet, const Vector2& size) {
	b2Body* body = createBody(position, bodyType, bullet);

	if (type == player2d) {
		ResAnim* resource = animalsResources.getResAnim("bee");
		spSprite sprite = createSprite(resource, Vector2(0, 0), false);
		float ratio = sprite->getWidth() / sprite->getHeight();

		attachFixture(body, size);
		Player2d* player = new Player2d(_world, body, "player", SCALE);
		b2AABB aaBb = body->GetFixtureList()->GetAABB(0);
		Vector2 playerSize = convert(b2Vec2(aaBb.upperBound.x - aaBb.lowerBound.x, abs(aaBb.upperBound.y - aaBb.lowerBound.y)));

		sprite->setScale(playerSize.x / sprite->getWidth(), playerSize.y / sprite->getHeight());
		player->_sprite = sprite;

		player->_sprite->attachTo(_oxyWorld);
		player->_sprite->setUserData(body);

		return player;
	}
	else if (type == static2d) {
		attachFixture(body, size);

		Static2d* staticObject = new Static2d(_world, body, "static", SCALE);

		b2AABB aaBb = body->GetFixtureList()->GetAABB(0);
		Vector2 staticSize = convert(b2Vec2(aaBb.upperBound.x - aaBb.lowerBound.x, abs(aaBb.upperBound.y - aaBb.lowerBound.y)));

		staticObject->_sprite = createSprite(gameResources.getResAnim("quad_40"), staticSize);

		staticObject->_sprite->attachTo(_oxyWorld);
		staticObject->_sprite->setUserData(body);

		return staticObject;

	}
	else if (type == kinematic2d) {
		Kinematic2d* kinematicObject = new Kinematic2d(_world, body, "kinematic");

		b2AABB aaBb = body->GetFixtureList()->GetAABB(0);
		Vector2 size = convert(b2Vec2(aaBb.upperBound.x - aaBb.lowerBound.x, abs(aaBb.upperBound.y - aaBb.lowerBound.y)));

		kinematicObject->_sprite = createSprite(gameResources.getResAnim("quad_40"), size);

		kinematicObject->_sprite->attachTo(_oxyWorld);
		kinematicObject->_sprite->setUserData(body);

		return kinematicObject;
	}
	else if (type == floor2d) {
		attachFixture(body, size);

		Floor2d* floorObject = new Floor2d(_world, body, "floor", SCALE);
		b2AABB aaBb = body->GetFixtureList()->GetAABB(0);
		Vector2 staticSize = convert(b2Vec2(aaBb.upperBound.x - aaBb.lowerBound.x, abs(aaBb.upperBound.y - aaBb.lowerBound.y)));

		floorObject->_sprite = createSprite(gameResources.getResAnim("quad_40"), staticSize);

		//floorObject->addPlatformSprite();
		floorObject->_sprite->attachTo(_oxyWorld);
		floorObject->_sprite->setUserData(body);
		//floorObject->addPlatformSprite();

		return floorObject;
	}
	else  {
		Dynamic2d* dynamicObject = new Dynamic2d(_world, body, "dynamic");

		b2AABB aaBb = body->GetFixtureList()->GetAABB(0);
		Vector2 size = convert(b2Vec2(aaBb.upperBound.x - aaBb.lowerBound.x, abs(aaBb.upperBound.y - aaBb.lowerBound.y)));

		dynamicObject->_sprite = createSprite(gameResources.getResAnim("quad_40"), size);

		dynamicObject->_sprite->setInputChildrenEnabled(false);
		dynamicObject->_sprite->attachTo(_oxyWorld);
		dynamicObject->_sprite->setUserData(body);

		return dynamicObject;
	}
}

spSprite Box2dFactory::createSprite(ResAnim* res, const Vector2& size, bool shouldScale) {
	spSprite sprite = new Sprite();
	sprite->setAnchor(0.5f, 0.5f);
//	sprite->setColor(Color(25, 25, 112, 100));
	sprite->setResAnim(res);
	if (shouldScale) {
		sprite->setScale(size.x / sprite->getWidth(), size.y / sprite->getHeight());
	}
	
	return sprite;
}

void Box2dFactory::attachFixture(b2Body* body, VectorArray<Vector2*> *oxyVertices) {
	b2FixtureDef *createdFixture = createFixture();
	createdFixture->shape = createPolygonShape(oxyVertices);
	body->CreateFixture(createdFixture);
}

void Box2dFactory::attachFixture(b2Body* body, const Vector2& size) {
	b2FixtureDef *createdFixture = createFixture();
	createdFixture->shape = createPolygonShape(size);
	body->CreateFixture(createdFixture);
}

void Box2dFactory::attachFixture(b2Body* body, float radius, const Vector2& position) {
	b2FixtureDef *createdFixture = createFixture();
	createdFixture->shape = createCircleShape(position, radius);
	body->CreateFixture(createdFixture);
}

b2CircleShape* Box2dFactory::createCircleShape(const Vector2& position, float radius) {
	b2CircleShape* circleShape = new b2CircleShape();

	circleShape->m_radius = radius / SCALE;
	//circleShape->m_p = convert(position);

	return circleShape;
}

b2PolygonShape* Box2dFactory::createPolygonShape(VectorArray<Vector2*> *oxyVertices) {
	b2PolygonShape* polygonShape = new b2PolygonShape();

	b2Vec2 vertices[3];
	int32 count = 0;
	for (int currentVerticesCount = 0; count < 3; currentVerticesCount++) {
		Vector2 &fixtureVertices = *(*oxyVertices)[currentVerticesCount];
		if (count == 2) {
			vertices[1] = convertVertices(fixtureVertices);
		}
		else if (count == 1) {
			vertices[2] = convertVertices(fixtureVertices);
		}
		else {
			vertices[count] = convertVertices(fixtureVertices);
		}
		count++;
	}

	polygonShape->Set(vertices, count);

	return polygonShape;
}

b2PolygonShape* Box2dFactory::createPolygonShape(const Vector2& size) {
	b2PolygonShape* polygonShape = new b2PolygonShape();

	b2Vec2 convertedSize = convert(size);
	polygonShape->SetAsBox(convertedSize.x, convertedSize.y);

	return polygonShape;
}

b2Body* Box2dFactory::createBody(const Vector2& position, int bodyType, bool bullet) {
	b2BodyDef bodyDef;
	b2Vec2 bodyPosition = convertBody(position);
	bodyDef.position = bodyPosition;

	if (bodyType == dynamicBody) {
		bodyDef.type = b2_dynamicBody;
	}
	else if (bodyType == kinematicBody) {
		bodyDef.type = b2_kinematicBody;
	}
	else if (bodyType == staticBody) {
		bodyDef.type = b2_staticBody;
	}

	bodyDef.bullet = bullet;

	b2Body *body = _world->CreateBody(&bodyDef);

	return body;
}

b2FixtureDef* Box2dFactory::createFixture(double density, double friction, bool isSensor) {
	b2FixtureDef *fixtureDef = new b2FixtureDef();

	fixtureDef->density = density;
	fixtureDef->friction = friction;
	fixtureDef->isSensor = isSensor;

	return fixtureDef;
}

b2Vec2 Box2dFactory::convertBody(const Vector2 &pos) {
	return b2Vec2(pos.x / SCALE, (_oxyWorld->getHeight() - abs(pos.y)) / SCALE);
}

Vector2 Box2dFactory::convertBody(const b2Vec2 &pos) {
	return Vector2(pos.x * SCALE, 640.0f - pos.y * SCALE);
}

b2Vec2 Box2dFactory::convertVertices(const Vector2 &pos) {
	return b2Vec2(pos.x / SCALE, -pos.y / SCALE);
}

Vector2 Box2dFactory::convertVertices(const b2Vec2 &pos) {
	return Vector2(pos.x * SCALE, -pos.y * SCALE);
}

b2Vec2 Box2dFactory::convert(const Vector2 &pos) {
	return b2Vec2(pos.x / SCALE, pos.y / SCALE);
}

Vector2 Box2dFactory::convert(const b2Vec2 &pos) {
	return Vector2(pos.x * SCALE, pos.y * SCALE);
}
/*
void Box2dFactory::addRevJoint(b2Body* bodyA, b2Body* bodyB) {
b2RevoluteJointDef *jointDef = new b2RevoluteJointDef();
Vector2 deviceScale = Vector2(getRoot()->getWidth() / 960.0f, getRoot()->getHeight() / 640.0f);

bodyA->SetType(b2_staticBody);
bodyB->SetType(b2_dynamicBody);
jointDef->Initialize(bodyA, bodyB, convertBody(Vector2(195.0f * deviceScale.x, 501.0f * deviceScale.y)));
jointDef->collideConnected = false;
jointDef->type = e_revoluteJoint;
jointDef->enableMotor = true;
jointDef->motorSpeed = 10;
jointDef->lowerAngle = -360;
jointDef->upperAngle = 360;
jointDef->enableLimit = false;
jointDef->maxMotorTorque = 20;
b2Joint *joint = _world->CreateJoint(jointDef);
}
*/