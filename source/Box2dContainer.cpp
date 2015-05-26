#include "Box2dContainer.h"

Box2dContainer::Box2dContainer(const Vector2& size) : _world(0), _isWorldPaused(false), _pauseWorldInFrames(-1) {
	SCALE = P2M_RATIO;
	setSize(size);

	Vector2 pos = getPosition();
	Vector2 scl = getScale();
	float der = getDerivedHeight();
	float der2 = getDerivedWidth();
	_worldWidth = (float)(getRoot()->getSize().x / SCALE);
	_worldHeight = (float)(getRoot()->getSize().y / SCALE);
	_worldStep = 1000.0f;

	_world = new b2World(b2Vec2(0, 30), false);
	_world->SetContactListener(&contactListenerInstance);
	
	_forceRestart = false;
	_previousFrameX = -19380;
}

Box2dContainer::~Box2dContainer() {

}

void Box2dContainer::addPlayer(Player2d* player) {
	_player = player;
	addEntity(player);
}

void Box2dContainer::addEntity(Entity* entity) {
	_entities.push_back(entity);
}

void Box2dContainer::addForRemoval(Entity* entity) {
	_entitiesScheduledForRemoval.insert(entity);
}

void Box2dContainer::win() {
	if (getParent()) {
		Box2dEvent levelWonEvent(Box2dEvent::LEVEL_WON);
		dispatchEvent(&levelWonEvent);
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

void Box2dContainer::showHideDebug() {
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

void Box2dContainer::pauseWorld() {
	if (_isWorldPaused) {
		_isWorldPaused = false;
		_pauseWorldInFrames = -1;
	}
	else {
		_isWorldPaused = true;
	}
}

void Box2dContainer::pauseWorldAfter(int frames) {
	_pauseWorldInFrames = frames;
}

void Box2dContainer::doUpdate(const UpdateState &us) {
	bool shouldSoftRestartLevel = false;
	updatePause();

	if (!_isWorldPaused) {
		_world->Step(us.dt / _worldStep, 6, 2);
		
		float currentFrameX = -convert(_player->getBody()->GetPosition()).x * getRoot()->getHeight() / 640;
	
		if (currentFrameX != _previousFrameX) {
			_previousFrameX = currentFrameX;
		}
		else {
			shouldSoftRestartLevel = true;
		}
	
		float playerPosition = _previousFrameX + getRoot()->getWidth() * 0.2f;
		//_floor->setX(_previousFrameX);

		setX(playerPosition);
		playerPosition = convert(_player->getBody()->GetPosition()).x;
		
		//float playerPosition = convert(_player->getBody()->GetPosition()).x;

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
		
		//float playerPosition = convert(_player->getBody()->GetPosition()).x;
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

	if (shouldSoftRestartLevel) {
		pauseWorld();
		_player->animateDeath(CLOSURE(this, &Box2dContainer::onPlayerDeathAnimationFinished));
	}
	else if (_forceRestart) {
		_forceRestart = false;
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

void Box2dContainer::onTouchDown(Event *event) {
	if (!_isWorldPaused) {
		_player->startJump();
	}
}

void Box2dContainer::onTouchUp(Event *event) {
	_player->stopJump();
}

b2Vec2 Box2dContainer::convert(const Vector2 &pos) {
	return b2Vec2(pos.x / SCALE, pos.y / SCALE);
}

Vector2 Box2dContainer::convert(const b2Vec2 &pos) {
	return Vector2(pos.x * SCALE, pos.y * SCALE);
}

void Box2dContainer::restart() {
	_STL::vector<Entity*>::iterator it = _entities.begin();
	_STL::vector<Entity*>::iterator end = _entities.end();
	for (; it != end; ++it) {
		Entity* entity = *it;
		entity->restart();
	}

	Box2dEvent LevelRestartEvent(Box2dEvent::LEVEL_RESTART);
	dispatchEvent(&LevelRestartEvent);
}

void Box2dContainer::onPlayerDeathAnimationFinished(Event *ev) {
	pauseWorld();
	restart();
}

/*
void Box2dContainer::addQuad() {
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
}*/

void Box2dContainer::updatePause() {
	if (_pauseWorldInFrames > 0) {
		_pauseWorldInFrames--;
		if (_pauseWorldInFrames == 0) {
			_isWorldPaused = true;
		}
	}
}