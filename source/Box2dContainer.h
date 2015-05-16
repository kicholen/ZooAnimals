#ifndef _BOX2DCONTAINER_
#define _BOX2DCONTAINER_

#include "oxygine-framework.h"
#include "Box2D\Box2D.h"
#include "Box2DDebugDraw.h"
#include "FlashUtils.h"
//#include "SpriteContainer.h"
#include "Main.h"
#include "Entity.h"
#include <set>
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
	class LevelWonEvent: public Event
	{
	public:
		enum EV
		{
			LEVEL_WON = makefourcc('L', 'L', 'W', 'N'),
		};

		LevelWonEvent(EV ev):Event(ev) {};
	};

	class LevelRestartEvent: public Event
	{
	public:
		enum EV
		{
			LEVEL_RESTART = makefourcc('L', 'L', 'R', 'R'),
		};

		LevelRestartEvent(EV ev):Event(ev) {};
	};

	Box2dContainer();
	~Box2dContainer();

	b2World *_world;
	spBox2DDraw _debugDraw;

//	b2Body* create2dObject(Group2d group);
//	b2Body* create2dObject(Group2d group, Group image);

	void addRevJoint(const char *bodyAName, const char* bodyBName);

	void addForRemoval(Entity* e1);

	// events/interaction
	void win();
	void moveEntityByBodyName(string bodyName);
	void slowMode(bool enable);
	void showHideDebug(Event *event);
	void pauseWorld(Event *event);
	void addQuad(Event *event);
	void saveWorld(Event *event);

protected:
	void onTouchDown(Event *event);
	void onTouchUp(Event *event);

	void doUpdate(const UpdateState &us);

	b2Body* addBody(double x, double y, string bodyType, bool bullet);

//	b2FixtureDef addFixture(string shapeType, double density, double friction, bool isSensor, double circleRadius, double circleX, double circleY, Array<Vector2*>* vertexArray);

	void removeBodies();
	void restart();

	void makeBoom(Event *event);

	//void saveWorldToFile();
private:
	b2Vec2 convert(const Vector2 &pos);
	b2Vec2 convertBody(const Vector2 &pos);
	Vector2 convertBody(const b2Vec2 &pos);
	b2Vec2 convertVertices(const Vector2 &pos);
	Vector2 convertVertices(const b2Vec2 &pos);
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

//	spPixelBackground _pixelBackground;
	spSprite _floor;

	// edit
	bool _isWorldPaused;

	spColorRectSprite _uiContainer;
};

#endif