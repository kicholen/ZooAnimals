#include "Player2d.h"
#include "Box2dContainer.h"

Player2d::Player2d(b2World* world, b2Body *body, string bodyName, float scale) {
	m_body = body;
	_scale = scale;
	m_body->SetFixedRotation(true);
	m_body->SetGravityScale(4.0f);
	// set first sensor
	b2PolygonShape sensorShape;
	b2FixtureDef sensorFixtureDef;
	b2Fixture* currentFixture = m_body->GetFixtureList();
	float32 fixtureRadius = currentFixture->GetShape()->m_radius;
	currentFixture->SetFriction(0.3f);

	//currentFixture->GetMassData();

	sensorShape.SetAsBox(fixtureRadius, fixtureRadius, b2Vec2(0.0f, 0.0f), 0);
	sensorFixtureDef.shape = &sensorShape;
	sensorFixtureDef.isSensor = false;
	b2Fixture* fixture1 = m_body->CreateFixture(&sensorFixtureDef);
	fixture1->SetFriction(0.0f);
	fixture1->SetDensity(1.0f);
	fixture1->SetRestitution(0.0f);
	fixture1->SetUserData((void*)3);
/*
	sensorShape.SetAsBox(fixtureRadius * 0.9f, fixtureRadius * 0.1, b2Vec2(0, fixtureRadius * 0.95), 0);
	sensorFixtureDef.shape = &sensorShape;
	sensorFixtureDef.isSensor = true;
	b2Fixture* footSensorFixture1 = m_body->CreateFixture(&sensorFixtureDef);
	footSensorFixture1->SetUserData((void*)3);

	// set second sensor
	sensorShape.SetAsBox(fixtureRadius * 0.9f, fixtureRadius * 0.1, b2Vec2(0, -fixtureRadius * 0.95f), 0);
	sensorFixtureDef.shape = &sensorShape;
	b2Fixture* footSensorFixture2 = m_body->CreateFixture(&sensorFixtureDef);
	footSensorFixture2->SetUserData((void*)3);

	// set third sensor
	sensorShape.SetAsBox(fixtureRadius * 0.1, fixtureRadius * 0.9f, b2Vec2(-fixtureRadius * 0.95f, 0), 0);
	sensorFixtureDef.shape = &sensorShape;
	b2Fixture* footSensorFixture3 = m_body->CreateFixture(&sensorFixtureDef);
	footSensorFixture3->SetUserData((void*)3);

	// set fourth sensor
	sensorShape.SetAsBox(fixtureRadius * 0.1, fixtureRadius * 0.9f, b2Vec2(fixtureRadius * 0.95f, 0), 0);
	sensorFixtureDef.shape = &sensorShape;
	b2Fixture* footSensorFixture4 = m_body->CreateFixture(&sensorFixtureDef);
	footSensorFixture4->SetUserData((void*)3);
*/
	m_world = world;
	m_body->SetUserData(this);
	_startPosition = m_body->GetPosition();
	_force = -m_body->GetMass() * 35;
	_bodyName = bodyName;
	_contacting = false;
	_shouldJump = false;
	_numContacts = 0;
	_touchHold = 0;
	_jumpTimeout = 0;
	_jumpOnThisFrame = false;
	_frameCounter = 0;
}

Player2d::~Player2d() {
    m_world->DestroyBody(m_body);

	m_body->SetUserData(0);
}

void Player2d::update(float playerPosition) {
	b2Vec2 vel = m_body->GetLinearVelocity();
	vel.x = 15;
    m_body->SetLinearVelocity(vel);

	if (_numContacts > 0 && _frameCounter > 4) {
		m_body->SetAngularVelocity(0);
		onGroundTouchSetDefaultRotation();
	}

	if (_jumpOnThisFrame) {
		m_body->ApplyLinearImpulse(b2Vec2(0, _force), m_body->GetLocalCenter());
		_jumpOnThisFrame = false;
		m_body->SetAngularVelocity(ROT_PER_SECOND * DEGTORAD);
		 // 2 klatki musi min¹æ siiiiiiiiick
		_frameCounter = 0;
	}

	if (_shouldJump) {
		tryToJump();
	}
	else {
		_touchHold = 0;
		_canJump = false;
	}
	if (_jumpTimeout > 0) {
		_jumpTimeout--;
	}

	// rotate
	//if (_numContacts == 0) {
	//	m_body->SetTransform(m_body->GetPosition(), m_body->GetAngle() + ROT_PER_FRAME * DEGTORAD);
	//}
	//else {
	//	onGroundTouchSetDefaultRotation();
	//}

	_frameCounter++;

	const b2Vec2& pos = m_body->GetPosition();
	_sprite->setPosition(convert(pos));
	_sprite->setRotation(m_body->GetAngle());
}

void Player2d::restart() {
	m_body->SetTransform(_startPosition, 0);
	_contacting = false;
	_shouldJump = false;
	_touchHold = 0;
	_jumpTimeout = 0;
	_jumpOnThisFrame = false;
	_frameCounter = 0;
}

void Player2d::setBodyPosition() {
	m_body->SetTransform(convert(_sprite->getPosition()), m_body->GetAngle());
}

void Player2d::startContact(b2Fixture *fixtureA, b2Fixture *fixtureB) {
	Entity *entity = (Entity *)fixtureA->GetBody()->GetUserData();
	string bodyName = entity->_bodyName;
	if (bodyName == "smallerPower" && !fixtureA->IsSensor()) {
		static_cast<Box2dContainer*>(_sprite->getParent())->addForRemoval(entity);
		b2Fixture *fixture = m_body->GetFixtureList();
		while (fixture) {
			b2Shape *shape = fixture->GetShape();
			if (shape->GetType() == 0) {
				static_cast<b2CircleShape*>(shape)->m_radius *= 0.5f; 
			}
			b2Fixture *next = fixture->GetNext();
			fixture = next;
		}
	}
	else if (bodyName == "biggerPower" && !fixtureA->IsSensor()) {
		static_cast<Box2dContainer*>(_sprite->getParent())->addForRemoval(entity);
		b2Fixture *fixture = m_body->GetFixtureList();
		while (fixture) {
			b2Shape *shape = fixture->GetShape();
			if (shape->GetType() == 0) {
				static_cast<b2CircleShape*>(shape)->m_radius *= 1.5f; 
			}
			b2Fixture *next = fixture->GetNext();
			fixture = next;
		}
	}
	else if (bodyName == "reverseGravity" && !fixtureA->IsSensor()) {
		static_cast<Box2dContainer*>(_sprite->getParent())->addForRemoval(entity);
		m_body->SetGravityScale(m_body->GetGravityScale() * -1);
	}
	else if (bodyName == "angularVelocity" && !fixtureA->IsSensor()) {
		static_cast<Box2dContainer*>(_sprite->getParent())->addForRemoval(entity);
		m_body->SetAngularVelocity(10);
	}
	else if (bodyName == "meta" && !fixtureA->IsSensor()) {
		static_cast<Box2dContainer*>(_sprite->getParent())->addForRemoval(entity);
		static_cast<Box2dContainer*>(_sprite->getParent())->win();
	}
	else if (bodyName == "slowMode") {
		static_cast<Box2dContainer*>(_sprite->getParent())->addForRemoval(entity);
		static_cast<Box2dContainer*>(_sprite->getParent())->slowMode(true);
	}
	else if (bodyName == "normalMode") {
		static_cast<Box2dContainer*>(_sprite->getParent())->addForRemoval(entity);
		static_cast<Box2dContainer*>(_sprite->getParent())->slowMode(false);
	}
	else if (bodyName == "verticalBarrierKey") {
		static_cast<Box2dContainer*>(_sprite->getParent())->moveEntityByBodyName("verticalBarrier");
	}
	else if (bodyName == "horizontalBarrierKeynull") {
		static_cast<Box2dContainer*>(_sprite->getParent())->moveEntityByBodyName("horizontalBarrier");
	}

	//if ((int)fixtureB->GetUserData() == 3) {
		_numContacts++;
	//}
}

void Player2d::endContact(b2Fixture *fixtureA, b2Fixture *fixtureB) {
	//if ((int)fixtureB->GetUserData() == 3) {
		_numContacts--;
	//}
}

b2Body* Player2d::getBody() {
	return m_body;
}

void Player2d::tryToJump() {
	_touchHold++;
	if (_touchHold < 30) {
		if(_numContacts >= 1 && _jumpTimeout <= 0) {
			_canJump = true;
			_jumpTimeout = 15;
			_minJump = true;
		}
		if (_canJump) {
			if (_touchHold > 35) {
				_touchHold = 30;
				_canJump = false;
			}
			if (_minJump) {
				//float v0 = -sqrt(abs(float(2 * getRoot()->getHeight() / _scale * 30))); // gravity
				onGroundTouchSetDefaultRotation();
				_jumpOnThisFrame = true;
				_minJump = false;
				//float angle = m_body->GetAngle() != 0.0f ? m_body->GetAngle() : 0.1;
				//m_body->SetTransform(m_body->GetPosition(), angle); 
			}
			//else {
				//_force = m_body->GetMass() * _touchHold;
				//m_body->ApplyForce(b2Vec2(0, _force), m_body->GetLocalCenter());
			//}
		}
	}
}

void Player2d::onGroundTouchSetDefaultRotation() {
	float32 currentAngle = RADTODEG * m_body->GetAngle();
	m_body->SetAngularVelocity(0.0f);
	if (currentAngle <= 45) {
		currentAngle = 0;
	}
	else if (currentAngle > 45 && currentAngle <= 135) {
		currentAngle = 90;
	}
	else if (currentAngle > 135 && currentAngle <= 225) {
		currentAngle = 180;
	}
	else if (currentAngle > 225 && currentAngle <= 315) {
		currentAngle = 270;
	}
	else {
		currentAngle = 0;
	}
	m_body->SetTransform(m_body->GetPosition(), currentAngle * DEGTORAD);
}

void Player2d::startJump() {
	_shouldJump = true;
}

void Player2d::stopJump() {
	_shouldJump = false;
}

b2Vec2 Player2d::convertForce(const Vector2 &force) {
	return b2Vec2(force.x * _scale, -force.y * _scale);
}

Vector2 Player2d::convert(const b2Vec2 &pos) {
	return Vector2(pos.x * _scale, pos.y * _scale);
}

b2Vec2 Player2d::convert(const Vector2 &pos) {
	return b2Vec2(pos.x / _scale, pos.y / _scale);
}