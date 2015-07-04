#include "Kinematic2d.h"


Kinematic2d::Kinematic2d(b2World* world, b2Body *body, std::string bodyName) {
	m_body = body;
	m_world = world;
	m_body->SetUserData(this);
	_bodyName = bodyName;
	m_contacting = false;
	m_numContacts = 0;
}


Kinematic2d::~Kinematic2d() {
	m_world->DestroyBody(m_body);
	m_body->SetUserData(0);
}

void Kinematic2d::update(float playerPosition) {

}

void Kinematic2d::startContact(b2Fixture *fixtureA, b2Fixture *fixtureB) {
	
}

void Kinematic2d::endContact(b2Fixture *fixtureA, b2Fixture *fixtureB) {

}

void Kinematic2d::restart() {

}


b2Body* Kinematic2d::getBody() {
	return m_body;
}