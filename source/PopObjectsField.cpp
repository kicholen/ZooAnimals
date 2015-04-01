#include "PopObjectsField.h"

PopObjectsField::PopObjectsField() {
	_emitter = new ParticleEmitter(Vector2(getWidth() / 2, getWidth() / 2), Vector2(getHeight() / 2, getHeight() / 2), Vector2(-0.2, 0.2), Vector2(-0.2, 0.2), Vector2(2, 2), Vector2(4, 4), "test_particle"); 
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