#include "RandomObstacle2d.h"
#include "SharedResources.h"

RandomObstacle2d::RandomObstacle2d(b2World* world, b2Body *body, string bodyName, float scale) : _wasStartPositionSet(false) {
	m_body = body;
	m_world = world;
	m_body->SetUserData(this);
	_bodyName = bodyName;
	m_contacting = false;
	m_numContacts = 0;
	_scale = scale;
	_isTweenInProgress = false;
}


RandomObstacle2d::~RandomObstacle2d() {
	m_world->DestroyBody(m_body);
	m_body->SetUserData(0);
}

void RandomObstacle2d::update(float playerPosition) {
	const b2Vec2& pos = m_body->GetPosition();
	const Vector2& entityPosition = convert(pos);
	bool shouldAddTween = false;

	if (!_wasStartPositionSet) {
		_startPosition = entityPosition;
		_wasStartPositionSet = true;
	}

	if (playerPosition - getRoot()->getWidth() * 0.2f > entityPosition.x) {
		m_body->SetTransform(b2Vec2((entityPosition.x + getRoot()->getWidth() + _sprite->getDerivedWidth()) / _scale, entityPosition.y / _scale), 0);
		shouldAddTween = true;
	}
	
	if (shouldAddTween) {
		_sprite->setY(0.0f);
		_sprite->setAlpha(0);
		unsigned char alpha = 255;
		_sprite->addTween(Actor::TweenAlpha(alpha), 2000 / 4, 1);
		_sprite->addTween(Actor::TweenY(entityPosition.y), 2000 / 4)->addDoneCallback(CLOSURE(this, &RandomObstacle2d::onTweenDone));
		_sprite->setX(entityPosition.x);
		_isTweenInProgress = true;
	}
	else if (_isTweenInProgress) {
		_sprite->setX(entityPosition.x);
	}
	else {
		_sprite->setPosition(entityPosition);
	}
}

void RandomObstacle2d::startContact(b2Fixture *fixtureA, b2Fixture *fixtureB) {

}

void RandomObstacle2d::endContact(b2Fixture *fixtureA, b2Fixture *fixtureB) {

}

void RandomObstacle2d::restart() {
	_isTweenInProgress = false;
	m_body->SetTransform(b2Vec2(_startPosition.x / _scale, _startPosition.y / _scale), 0);
	_sprite->removeTweens(true);
	_sprite->removeAllEventListeners();
}

void RandomObstacle2d::onTweenDone(Event *event) {
	_isTweenInProgress = false;
}

b2Body* RandomObstacle2d::getBody() {
	return m_body;
}

Vector2 RandomObstacle2d::convert(const b2Vec2 &pos) {
	return Vector2(pos.x * _scale, pos.y * _scale);
}