#include "Box2dContainer.h"
#include "EditableSprite.h"
#include "SharedResources.h"
//#include "Box2dWorldSaver.h"

Box2dContainer::Box2dContainer() : _world(0), _isWorldPaused(false) {
	SCALE = P2M_RATIO;
	float boxScaleFactor = getRoot()->getHeight() / 640;
	setScale(boxScaleFactor);
	setSize(getRoot()->getSize().x * 10 / boxScaleFactor, getRoot()->getSize().y / boxScaleFactor);

	Vector2 pos = getPosition();
	Vector2 scl = getScale();
	float der = getDerivedHeight();
	float der2 = getDerivedWidth();
	_worldWidth = (float)(getRoot()->getSize().x / SCALE);
	_worldHeight = (float)(getRoot()->getSize().y / SCALE);
	_worldStep = 1000.0f;

	addEventListener(TouchEvent::TOUCH_DOWN, (detail::CreateClosure(&Box2dContainer::onTouchDown).Init<&Box2dContainer::onTouchDown>(this)));
	addEventListener(TouchEvent::TOUCH_UP, (detail::CreateClosure(&Box2dContainer::onTouchUp).Init<&Box2dContainer::onTouchUp>(this)));

	_world = new b2World(b2Vec2(0, 30), false);
	_world->SetContactListener(&contactListenerInstance);

	_floor = new ColorRectSprite();
	_floor->setAnchor(0.0f, 0.0f);
	_floor->setPosition(0, 0);
	_floor->setSize(this->getWidth(), this->getHeight());
	_floor->attachTo(this);
	_floor->setColor(Color(124, 222, 123, 140));
	_floor->addTween(ColorRectSprite::TweenColor(Color(50, 22, 10, 140)), 8000, 1, false, 1000)->addEventListener(TweenEvent::COMPLETE, CLOSURE(this, &Box2dContainer::makeBoom));
	
	_previousFrameX = -19380;
}


void Box2dContainer::makeBoom(Event *e) {
	_floor->addTween(ColorRectSprite::TweenColor(Color(255, 0, 0, 200)), 500, 2, true);
}

Box2dContainer::~Box2dContainer() {

}

void Box2dContainer::doUpdate(const UpdateState &us) {
	bool shouldRestartLevel = false;

	if (!_isWorldPaused) {
		_world->Step(us.dt / _worldStep, 6, 2);

		float currentFrameX = -convert(_player->getBody()->GetPosition()).x * getRoot()->getHeight() / 640;
	
		if (currentFrameX != _previousFrameX) {
			_previousFrameX = currentFrameX;
		}
		else {
			shouldRestartLevel = true;
		}
	
		float playerPosition = _previousFrameX + getRoot()->getWidth() * 0.2f;
		//_floor->setX(_previousFrameX);

		setX(playerPosition);
		playerPosition = convert(_player->getBody()->GetPosition()).x;

		b2Body *body = _world->GetBodyList();

		while(body) {
			Entity *entity = (Entity *)body->GetUserData();
			b2Body *next = body->GetNext();

			if (entity && entity->_sprite) {
				entity->update(playerPosition);
			}

			body = next;
		}
	}
	else {
		float currentFrameX = -convert(_player->getBody()->GetPosition()).x * getRoot()->getHeight() / 640;
	
		if (currentFrameX != _previousFrameX) {
			_previousFrameX = currentFrameX;
		}
	
		float playerPosition = _previousFrameX + getRoot()->getWidth() * 0.2f;
		//_floor->setX(_previousFrameX);

		setX(playerPosition);
		playerPosition = convert(_player->getBody()->GetPosition()).x;

		b2Body *body = _world->GetBodyList();

		while(body) {
			Entity *entity = (Entity *)body->GetUserData();
			b2Body *next = body->GetNext();

			if (entity && entity->_sprite && entity->_sprite->getParent()) {
				entity->setBodyPosition();
			}
			else {
				addForRemoval(entity);
			}

			body = next;
		}
	}
	
	removeBodies();

	if (shouldRestartLevel) {
		restart();
	}
}

void Box2dContainer::removeBodies() {
	_STL::set<Entity*>::iterator it = _entitiesScheduledForRemoval.begin();
	_STL::set<Entity*>::iterator end = _entitiesScheduledForRemoval.end();
	for (; it != end; ++it) {
		Entity* dyingEntity = *it;
		if (dyingEntity->_sprite) {
			dyingEntity->_sprite->detach();
		}

		delete dyingEntity;

		_STL::vector<Entity*>::iterator it = _STL::find(_entities.begin(), _entities.end(), dyingEntity);
		if (it != _entities.end()) {
			_entities.erase(it);
		}
	}

	_entitiesScheduledForRemoval.clear();
}

void Box2dContainer::addForRemoval(Entity* e1) {
	_entitiesScheduledForRemoval.insert(e1);
}

void Box2dContainer::showHideDebug(Event *event) {
	TouchEvent *te = safeCast<TouchEvent*>(event);
	te->stopsImmediatePropagation = true;
	if (_debugDraw) {
		_debugDraw->detach();
		_debugDraw = 0;
		return;
	}

	_debugDraw = new Box2DDraw;		
	_debugDraw->SetFlags(b2Draw::e_shapeBit | b2Draw::e_jointBit | b2Draw::e_pairBit | b2Draw::e_centerOfMassBit);
	_debugDraw->attachTo(this);
	_debugDraw->setWorld(SCALE, _world);
	_debugDraw->setPriority(1);
}

void Box2dContainer::onTouchDown(Event *event) {
	if (!_isWorldPaused) {
		_player->startJump();
	}
}

void Box2dContainer::onTouchUp(Event *event) {
	_player->stopJump();
}

void Box2dContainer::win() {
	if (getParent()) {
		LevelWonEvent levelWonEvent(LevelWonEvent::LEVEL_WON);
		dispatchEvent(&levelWonEvent);
	}
}
/*
b2Body* Box2dContainer::create2dObject(Group2d group) {
	b2Body* body = addBody(group[0], group[1], group.bodyType, group.isBullet);
	Array<Fixture2dModel*> *fixtureArray = new Array<Fixture2dModel*>();
	int currentVerticesCount = 0;

	fixtureArray = group.fixtureArray;
	for(int i = 0; i < fixtureArray->length(); i++) {
		Fixture2dModel &fixture = *(*fixtureArray)[i];
		b2FixtureDef createdFixture = addFixture(fixture.shapeType, fixture.density, fixture.friction, fixture.isSensor, fixture.circleRadius, fixture.circleX, fixture.circleY, fixture.vertexArray);
		b2CircleShape circleShape;
		b2PolygonShape polygonShape;

		string fixtureShape = fixture.shapeType;
		if (fixtureShape == "circleShape") {
			circleShape.m_radius = fixture.circleRadius / SCALE;
			circleShape.m_p = convert(Vector2(fixture.circleX, fixture.circleY));

			createdFixture.shape = &circleShape;
		}
		else if (fixtureShape == "polygonShape") {
			b2Vec2 vertices[3];
			Array<Vector2*> *vertexArray= new Array<Vector2*>();
			vertexArray = fixture.vertexArray;

			int32 count = 0;
			int32 countTest = 0;
			for(currentVerticesCount; count < 3; currentVerticesCount++) {
				Vector2 &fixtureVertices = *(*vertexArray)[currentVerticesCount];
				if (count == 2) {
					vertices[1] = convertVertices(fixtureVertices);
					countTest = 1;
				}
				else if (count == 1) {
					vertices[2] = convertVertices(fixtureVertices);
					countTest = 2;
				}
				else {
					vertices[count] = convertVertices(fixtureVertices);
					countTest = 0;
				}
				count++;
			}
			polygonShape.Set(vertices, count);
			createdFixture.shape = &polygonShape;
		}

		body->CreateFixture(&createdFixture);
	}

	if (group.bodyName == "start") {
		//spColorRectSprite sprite = new ColorRectSprite();
		//spSprite sprite = new Sprite();
		spEditableSprite sprite = new EditableSprite();
		sprite->setAnchor(0.5f, 0.5f);
		sprite->setColor(Color(25, 25, 112, 255));
		sprite->setResAnim(gameResources.getResAnim("player"));
		Vector2 playerSize = convert(b2Vec2(body->GetFixtureList()->GetShape()->m_radius * 2, body->GetFixtureList()->GetShape()->m_radius * 2));
		sprite->setScale(playerSize.x / sprite->getWidth(), playerSize.y / sprite->getHeight());
		//sprite->setSize(playerSize);

		Player2d* player = new Player2d(_world, body, group.bodyName, SCALE);
		_entities.push_back(player);

		sprite->attachTo(this);
		sprite->setUserData(body);
		sprite->setName(group.bodyName);
		player->_sprite = sprite;
		_player = player;
	}
	else if (group.bodyType == "static") {
		spEditableSprite sprite = new EditableSprite();
		sprite->setAnchor(0.5f, 0.5f);
		sprite->setColor(Color(255, 255, 255, 255));
		b2AABB aaBb = body->GetFixtureList()->GetAABB(0);
		Vector2 staticSize = convert(b2Vec2(aaBb.upperBound.x - aaBb.lowerBound.x, abs(aaBb.upperBound.y - aaBb.lowerBound.y)));
		string::size_type loc = group.bodyName.find("bottomwall", 0);
		if (loc != string::npos) {
			Floor2d* floorObject = new Floor2d(_world, body, group.bodyName, SCALE);
			_entities.push_back(floorObject);
			floorObject->_sprite = sprite;
			floorObject->addPlatformSprite();
			sprite->setSize(staticSize);
		}
		else {
			Static2d* staticObject = new Static2d(_world, body, group.bodyName, SCALE);
			_entities.push_back(staticObject);
			sprite->setResAnim(gameResources.getResAnim("box"));
			sprite->setScale(staticSize.x / sprite->getWidth(), staticSize.y / sprite->getHeight());
			sprite->setSize(staticSize);
			staticObject->_sprite = sprite;

			b2Fixture* currentFixture = body->GetFixtureList();
			while (currentFixture) {
				currentFixture->SetFriction(0.0f);
				currentFixture->SetRestitution(0.0f);

				currentFixture = currentFixture->GetNext();
			}
			_staticObject = staticObject;
		}
		sprite->attachTo(this);
		sprite->setUserData(body);
		sprite->setName(group.bodyName);
	}
	else if (group.bodyType == "dynamic") {
		Dynamic2d* dynamicObject = new Dynamic2d(_world, body, group.bodyName);
		_entities.push_back(dynamicObject);
	}
	else if (group.bodyType == "kinematic") {
		string::size_type loc = group.bodyName.find("platform", 0);
		if (loc != string::npos) {
			body->SetAngularVelocity(CMath::random(20, 50) * 0.0174532925199432957f);
		}
		Kinematic2d* kinematicObject = new Kinematic2d(_world, body, group.bodyName);
		_entities.push_back(kinematicObject);
	}

	return body;
}

b2Body* Box2dContainer::create2dObject(Group2d group, Group image) {
	b2Body* body = create2dObject(group);

	spSprite sprite = new Sprite();
	sprite->setResAnim(gameResources.getResAnim(image.path));
	sprite->attachTo(this);
	sprite->setAnchor(Vector2(0.5f, 0.5f));
	sprite->setScale(image[0], image[1]);
	sprite->setPosition(image[2], image[3]);
	sprite->setRotation(CMath::degreesToRadians(image[4]));
	sprite->setPriority(image[5]);
	sprite->setInputChildrenEnabled(false);
	sprite->setUserData(body);
	sprite->setName(group.bodyName);

	return body;
}
*/
b2Body* Box2dContainer::addBody(double x, double y, string bodyType, bool bullet) {
	b2BodyDef bodyDef;
	b2Vec2 bodyPosition = convertBody(Vector2(x, y));
	bodyDef.position = bodyPosition;
	if (bodyType == "dynamic") {
		bodyDef.type = b2_dynamicBody;
	}
	else if (bodyType == "kinematic") {
		bodyDef.type = b2_kinematicBody;
	}
	else if (bodyType == "static") {
		bodyDef.type = b2_staticBody;
	}

	bodyDef.bullet = bullet;

	b2Body *body = _world->CreateBody(&bodyDef);

	return body;
}

void Box2dContainer::addRevJoint(const char *bodyAName, const char* bodyBName) {
	b2RevoluteJointDef *jointDef = new b2RevoluteJointDef();

	b2Body* bodyA;
	b2Body* bodyB;
	int counter = 0;

	_STL::vector<Entity*>::iterator it = _entities.begin();
	_STL::vector<Entity*>::iterator end = _entities.end();
	for (; it != end; ++it) {
		Entity* entity = *it;
		if (entity->_bodyName == bodyAName) {
			bodyA = entity->getBody();
			counter++;
		}
		else if (entity->_bodyName == bodyBName) {
			bodyB = entity->getBody();
			counter++;
		}
	}
	Vector2 deviceScale = Vector2(getRoot()->getWidth() / 960, getRoot()->getHeight() / 640);
	if (counter == 2) {
		bodyA->SetType(b2_staticBody);
		bodyB->SetType(b2_dynamicBody);
		jointDef->Initialize(bodyA, bodyB, convertBody(Vector2(195 * deviceScale.x, 501 * deviceScale.y)));
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

}

void Box2dContainer::moveEntityByBodyName(string bodyName) {
	_STL::vector<Entity*>::iterator it = _entities.begin();
	_STL::vector<Entity*>::iterator end = _entities.end();
	for (; it != end; ++it) {
		Entity* entity = *it;
		if (entity->_bodyName == bodyName) {
			entity->getBody()->SetType(b2_dynamicBody);
			entity->getBody()->SetGravityScale(-1);
		}
	}
}

void Box2dContainer::slowMode(bool enable) {
	_worldStep = enable ? 5000.0f : 1000.0f;
}
/*
b2FixtureDef Box2dContainer::addFixture(string shapeType, double density, double friction, bool isSensor, double circleRadius, double circleX, double circleY, Array<Vector2*>* vertexArray) {
	b2FixtureDef fixtureDef;

	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.isSensor = isSensor;

	return fixtureDef;
}
*/
b2Vec2 Box2dContainer::convert(const Vector2 &pos) {
	return b2Vec2(pos.x / SCALE, pos.y / SCALE);
}

Vector2 Box2dContainer::convert(const b2Vec2 &pos) {
	return Vector2(pos.x * SCALE, pos.y * SCALE);
}

b2Vec2 Box2dContainer::convertBody(const Vector2 &pos) {
	return b2Vec2(pos.x / SCALE, (getHeight() - abs(pos.y)) / SCALE);
}

Vector2 Box2dContainer::convertBody(const b2Vec2 &pos) {
	return Vector2(pos.x * SCALE, 640.0f - pos.y * SCALE);
}

b2Vec2 Box2dContainer::convertVertices(const Vector2 &pos) {
	return b2Vec2(pos.x / SCALE, -pos.y / SCALE);
}

Vector2 Box2dContainer::convertVertices(const b2Vec2 &pos) {
	return Vector2(pos.x * SCALE, -pos.y * SCALE);
}

void Box2dContainer::restart() {
	_STL::vector<Entity*>::iterator it = _entities.begin();
	_STL::vector<Entity*>::iterator end = _entities.end();
	for (; it != end; ++it) {
		Entity* entity = *it;
		entity->restart();
	}

	_floor->setColor(Color(124, 222, 123, 140));
	_floor->removeTweens();
	_floor->addTween(ColorRectSprite::TweenColor(Color(50, 22, 10, 140)), 8000, 1, false, 1000)->addEventListener(TweenEvent::COMPLETE, CLOSURE(this, &Box2dContainer::makeBoom));
	LevelRestartEvent LevelRestartEvent(LevelRestartEvent::LEVEL_RESTART);
	dispatchEvent(&LevelRestartEvent);
}

// edit stuff
void Box2dContainer::saveWorld(Event *event) {
//	saveWorldToFile();
}

void Box2dContainer::pauseWorld(Event *event) {
	_isWorldPaused = !_isWorldPaused;
}

void Box2dContainer::addQuad(Event *event) {
	if (_isWorldPaused) {
		spEditableSprite sprite = new EditableSprite();
		sprite->setAnchor(0.5f, 0.5f);
		sprite->setColor(Color(255, 255, 255, 255));
		b2AABB aaBb = _staticObject->getBody()->GetFixtureList()->GetAABB(0);
		Vector2 staticSize = convert(b2Vec2(aaBb.upperBound.x - aaBb.lowerBound.x, abs(aaBb.upperBound.y - aaBb.lowerBound.y)));

		b2Body *staticBody = _staticObject->getBody();

		b2World *world = _world; 
		b2BodyDef bodyDef;
		bodyDef.type = staticBody->GetType();
		bodyDef.position = _player->getBody()->GetPosition();
		b2Body *clonedBody = _world->CreateBody(&bodyDef);

		b2Fixture *fixture = staticBody->GetFixtureList();

		while (fixture) {
			clonedBody->CreateFixture(fixture->GetShape(), fixture->GetDensity());

			fixture = fixture->GetNext();
		}

		Static2d* staticObject = new Static2d(_world, clonedBody, _staticObject->_bodyName + CMath::doubleToString(CMath::random(1, 123123)), SCALE);
		_entities.push_back(staticObject);
		sprite->setResAnim(gameResources.getResAnim("box"));
		sprite->setScale(staticSize.x / sprite->getWidth(), staticSize.y / sprite->getHeight());
		sprite->setSize(staticSize);
		Vector2 spritePosition = convert(clonedBody->GetPosition());
		sprite->setPosition(spritePosition.x, spritePosition.y);
		staticObject->_sprite = sprite;

		b2Fixture* currentFixture = clonedBody->GetFixtureList();
		while (currentFixture) {
			currentFixture->SetFriction(0.0f);
			currentFixture->SetRestitution(0.0f);

			currentFixture = currentFixture->GetNext();
		}
		sprite->attachTo(this);
		sprite->setUserData(clonedBody);
		sprite->setName(staticObject->_bodyName);

		clonedBody->SetUserData(staticObject);
	}
}