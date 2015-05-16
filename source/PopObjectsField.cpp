#include "PopObjectsField.h"

PopObjectsField::PopObjectsField() {
	_emitter = new ParticleEmitter(Vector2(getWidth() / 2.0f, getWidth() / 2.0f), Vector2(getHeight() / 2.0f, getHeight() / 2.0f), Vector2(-0.2f, 0.2f), Vector2(-0.2f, 0.2f), Vector2(2.0f, 2.0f), Vector2(4.0f, 4.0f), "test_particle"); 
	_emitter->setDestroyParticleOnTouch(true);
	_emitter->setDispatchEventOnParticleDeadByTouch(true);
	_emitter->addEventListener(Particle::ParticleEvent::DIE_EVENT, CLOSURE(this, &PopObjectsField::onParticlePopped));
	addChild(_emitter);
	_particlesPoppedCount = 0;
}

PopObjectsField::~PopObjectsField() {

}

void PopObjectsField::onParticlePopped(Event *ev) {
	++_particlesPoppedCount;
}