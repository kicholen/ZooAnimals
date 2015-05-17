#ifndef _BOX2DCONTAINER_
#define _BOX2DCONTAINER_

#include <set>
#include "oxygine-framework.h"
#include "Box2D\Box2D.h"
#include "Box2DDebugDraw.h"
#include "FlashUtils.h"
#include "Entity.h"
#include "Player2d.h"
#include "Dynamic2d.h"
#include "Static2d.h"
#include "Kinematic2d.h"
#include "TwoDContactListener.h"
#include "Floor2d.h"

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

using namespace FlashUtils;

const float P2M_RATIO = 32.0f;

DECLARE_SMART(Box2dContainer, spBox2dContainer);

class Box2dContainer : public Actor
{
public:
	class Box2dEvent: public Event
	{
	public:
		enum EV
		{
			LEVEL_WON = makefourcc('B', '2', 'D', 'W'),
			LEVEL_RESTART = makefourcc('B', '2', 'D', 'R')
		};

		Box2dEvent(EV ev) :Event(ev) {};
	};

	Box2dContainer();
	~Box2dContainer();

	b2World *_world;
	spBox2DDraw _debugDraw;

	void addPlayer(Player2d* player);
	void addEntity(Entity* entity);
	void addForRemoval(Entity* entity);

	// events/interaction
	void win();
	void moveEntityByBodyName(string bodyName);
	void slowMode(bool enable);
	void showHideDebug();
	void pauseWorld();

protected:
	void onTouchDown(Event *event);
	void onTouchUp(Event *event);

	void doUpdate(const UpdateState &us);

	void removeBodies();
	void restart();
private:
	b2Vec2 convert(const Vector2 &pos);
	Vector2 convert(const b2Vec2 &pos);

	float SCALE;
	float _worldWidth;
	float _worldHeight;
	float _worldStep;

	TwoDContactListener contactListenerInstance;
	std::vector<Entity*> _entities;
	std::set<Entity*> _entitiesScheduledForRemoval;
	Player2d* _player;
	Static2d* _staticObject;

	float _previousFrameX;

	bool _isWorldPaused;
};

#endif