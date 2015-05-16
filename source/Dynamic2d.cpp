#include "Dynamic2d.h"


Dynamic2d::Dynamic2d(b2World* world, b2Body *body, string bodyName) {
	m_body = body;
	m_world = world;
	m_body->SetUserData(this);
	_bodyName = bodyName;
	m_contacting = false;
	m_numContacts = 0;
}


Dynamic2d::~Dynamic2d() {
	m_world->DestroyBody(m_body);
	m_body->SetUserData(0);
}


void Dynamic2d::update() {

}

void Dynamic2d::startContact(b2Fixture *fixtureA, b2Fixture *fixtureB) {
	
}

void Dynamic2d::endContact(b2Fixture *fixtureA, b2Fixture *fixtureB) {

}

void Dynamic2d::restart() {

}

b2Body* Dynamic2d::getBody() {
	return m_body;
}