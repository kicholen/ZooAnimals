#include "Static2d.h"


Static2d::Static2d(b2World* world, b2Body *body, string bodyName, float scale) {
	m_body = body;
	m_world = world;
	m_body->SetUserData(this);
	_bodyName = bodyName;
	m_contacting = false;
	m_numContacts = 0;
	_scale = scale;
	_wasTweenAdded = false;
	_wasTweenFinished = true;
	_wasOutTweenAdded = false;
}

Static2d::~Static2d() {
	m_world->DestroyBody(m_body);
	m_body->SetUserData(0);
}

/** update only position of bodies **/
void Static2d::update(float playerPosition) {
	const b2Vec2& pos = m_body->GetPosition();
	Vector2 entityPosition = convert(pos);
	float rightSideOfScreenPosition = -playerPosition + getRoot()->getWidth() * 1.5f;
	
	if (entityPosition.x < playerPosition - (getRoot()->getWidth() / getRoot()->getHeight()) * 640 * 0.1f && !_wasOutTweenAdded) {
		_sprite->addTween(Actor::TweenAlpha(0), 2000 / 4, 1);
		_sprite->addTween(Actor::TweenY(640), 2000 / 4);//, 1, true, 0, Tween::ease_inExpo);
		_sprite->setX(entityPosition.x);
		_wasOutTweenAdded = true;
	}
	else if (entityPosition.x < playerPosition + (getRoot()->getWidth() / getRoot()->getHeight()) * 640 * 0.9f && !_wasTweenAdded) {//(-playerPosition + getRoot()->getHeight()) && !_wasTweenAdded) {
		_sprite->setY(0.0f);
		unsigned char alpha = 255;
		_sprite->setAlpha(0);
		_sprite->addTween(Actor::TweenAlpha(alpha), 2000 / 4, 1);//, true, 0, Tween::ease_inExpo);
		_sprite->addTween(Actor::TweenY(entityPosition.y), 2000 / 4)->addDoneCallback(CLOSURE(this, &Static2d::onTweenDone));//, 1, true, 0, Tween::ease_inExpo);
		_sprite->setX(entityPosition.x);
		_wasTweenAdded = true;
	}
	else if (_wasTweenAdded && !_wasTweenFinished) {
		_sprite->setX(entityPosition.x);
	}
	else {
		_sprite->setPosition(entityPosition);
	}
	
	/*if (_sprite->getY() > getHeight() + 50) {
		body->SetUserData(0);
		_world->DestroyBody(body);

		entity->_sprite->detach();
	}
	entity->update();
	*/


}

void Static2d::setBodyPosition() {
	if (!_sprite->getFirstTween() && !_wasOutTweenAdded) {
		m_body->SetTransform(convert(_sprite->getPosition()), m_body->GetAngle());
	}
}

void Static2d::startContact(b2Fixture *fixtureA, b2Fixture *fixtureB) {
	
}

void Static2d::endContact(b2Fixture *fixtureA, b2Fixture *fixtureB) {

}

void Static2d::restart() {
	_wasTweenAdded = false;
	_wasTweenFinished = true;
	_wasOutTweenAdded = false;
	_sprite->removeTweens(true);
}

void Static2d::onTweenDone(Event *event) {
	_wasTweenFinished = true;
}

b2Body* Static2d::getBody() {
	return m_body;
}

Vector2 Static2d::convert(const b2Vec2 &pos) {
	return Vector2(pos.x * _scale, pos.y * _scale);
}

b2Vec2 Static2d::convert(const Vector2 &pos) {
	return b2Vec2(pos.x / _scale, pos.y / _scale);
}