#ifndef _TWODCONTACTLISTENER_
#define _TWODCONTACTLISTENER_

#include "Box2D/Box2D.h"
#include "Entity.h"

class TwoDContactListener : public b2ContactListener {
	void BeginContact(b2Contact* contact) {
		void* bodyUserAData = contact->GetFixtureA()->GetBody()->GetUserData();
		void* bodyUserBData = contact->GetFixtureB()->GetBody()->GetUserData();

		if (bodyUserAData && bodyUserBData) {
			b2Fixture *fixtureA = contact->GetFixtureA();
			b2Fixture *fixtureB = contact->GetFixtureB();
			static_cast<Entity*>(bodyUserAData)->startContact(fixtureB, fixtureA);
			static_cast<Entity*>(bodyUserBData)->startContact(fixtureA, fixtureB);
		}
	}
	
	void EndContact(b2Contact* contact) {
		void* bodyUserAData = contact->GetFixtureA()->GetBody()->GetUserData();
		void* bodyUserBData = contact->GetFixtureB()->GetBody()->GetUserData();

		if (bodyUserAData && bodyUserBData) {
			b2Fixture *fixtureA = contact->GetFixtureA();
			b2Fixture *fixtureB = contact->GetFixtureB();
			static_cast<Entity*>(bodyUserAData)->endContact(fixtureB, fixtureA);
			static_cast<Entity*>(bodyUserBData)->endContact(fixtureA, fixtureB);
		}
	}
};

#endif