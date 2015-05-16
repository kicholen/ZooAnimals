#include "Floor2d.h"
#include "SharedResources.h"

//#include "PlatformSprite.h"

Floor2d::Floor2d(b2World* world, b2Body *body, string bodyName, float scale) {
	m_body = body;
	m_world = world;
	m_body->SetUserData(this);
	_bodyName = bodyName;
	m_contacting = false;
	m_numContacts = 0;
	_scale = scale;
	_blockWidth = 64.0f;
	_wasTweenAdded = false;
	_wasTweenFinished = true;
	_wasOutTweenAdded = false;
}


Floor2d::~Floor2d() {
	m_world->DestroyBody(m_body);
	m_body->SetUserData(0);
	//PlatformSprite::cleanup();
}

void Floor2d::update(float playerPosition) {
	const b2Vec2& pos = m_body->GetPosition();
	Vector2 entityPosition = convert(pos);

	if (playerPosition + (getRoot()->getWidth() / getRoot()->getHeight()) * 640 * 0.8f > _startPosition.x) {
		//PlatformSprite *part = new PlatformSprite(_startPosition.x, _startPosition.y - 500, _startPosition.x, _startPosition.y, 0.0f, 0xfffbe5, 300, 0);
		//_startPosition += Vector2(part->getDerivedWidth(), 0.0f);
	}

	_sprite->setPosition(entityPosition);
	
}


void Floor2d::startContact(b2Fixture *fixtureA, b2Fixture *fixtureB) {
	
}

void Floor2d::endContact(b2Fixture *fixtureA, b2Fixture *fixtureB) {

}

void Floor2d::restart() {
	_wasTweenAdded = false;
	_wasTweenFinished = true;
	_wasOutTweenAdded = false;
	_startPosition = _savedStartPosition;
	//PlatformSprite::cleanup();
	//PlatformSprite::initialize("box");
	//_sprite->addChild(PlatformSprite::ThePlatformSprite);
	_sprite->removeTweens(true);
	_sprite->removeAllEventListeners();
}

void Floor2d::onTweenDone(Event *event) {
	_wasTweenFinished = true;
}

b2Body* Floor2d::getBody() {
	return m_body;
}

Vector2 Floor2d::convert(const b2Vec2 &pos) {
	return Vector2(pos.x * _scale, pos.y * _scale);
}

void Floor2d::addPlatformSprite() {
	//float duration = 3;
	//unsigned int col = 0xfffbe5;
	//float angle = 0.0f;
	//float startX = destX;
	//float startY = destY - 200;
//	PlatformSprite::initialize("box");
//	_sprite->addChild(PlatformSprite::ThePlatformSprite);
	_startPosition = Vector2(0.0f, 0.0f);
	fillStartPosition();
	//PlatformSprite *part = new PlatformSprite(startX, startY, destX, destY, _blockWidth * _scale, angle, col, duration, 0.0f);
}

void Floor2d::fillStartPosition() {
	while (_startPosition.x < (getRoot()->getWidth() / getRoot()->getHeight()) * 640) {
		spSprite sprite = new Sprite();
		sprite->setAnchor(0.0f, 0.0f);
		sprite->setColor(Color(255, 255, 255, 255));
		sprite->setResAnim(gameResources.getResAnim("box"));
		//sprite->setScaleX(_blockWidth * getRoot()->getHeight() / 640 / sprite->getWidth());
		sprite->attachTo(_sprite);
		sprite->setPosition(_startPosition);
		_startPosition += Vector2(sprite->getDerivedWidth(), 0.0f);
	}
	_savedStartPosition = Vector2(_startPosition.x, _startPosition.y);
}