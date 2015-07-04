#include "Floor2d.h"
#include "SharedResources.h"

Floor2d::Floor2d(b2World* world, b2Body *body, std::string bodyName, float scale) : _wasStartPositionSet(false) {
	m_body = body;
	m_world = world;
	m_body->SetUserData(this);
	_bodyName = bodyName;
	m_contacting = false;
	m_numContacts = 0;
	_scale = scale;
}

Floor2d::~Floor2d() {
	m_world->DestroyBody(m_body);
	m_body->SetUserData(0);
}

void Floor2d::update(float playerPosition) {
	const b2Vec2& pos = m_body->GetPosition();
	const Vector2& entityPosition = convert(pos);
	
	if (!_wasStartPositionSet) {
		_startPosition = entityPosition;
		_wasStartPositionSet = true;
	}

	if (playerPosition - 300.0f > entityPosition.x) {
		m_body->SetTransform(b2Vec2((entityPosition.x + _sprite->getDerivedWidth()) / _scale, entityPosition.y / _scale), 0);
	}

	_sprite->setPosition(entityPosition);
}


void Floor2d::startContact(b2Fixture *fixtureA, b2Fixture *fixtureB) {
	
}

void Floor2d::endContact(b2Fixture *fixtureA, b2Fixture *fixtureB) {

}

void Floor2d::restart() {
	m_body->SetTransform(b2Vec2(_startPosition.x / _scale, _startPosition.y / _scale), 0);
	_sprite->removeTweens(true);
	_sprite->removeAllEventListeners();
}

b2Body* Floor2d::getBody() {
	return m_body;
}

Vector2 Floor2d::convert(const b2Vec2 &pos) {
	return Vector2(pos.x * _scale, pos.y * _scale);
}