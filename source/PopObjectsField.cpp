#include "PopObjectsField.h"

PopObjectsField::PopObjectsField() {
	_emitter = new ParticleEmitter(Vector2(getWidth() / 2.0f, getWidth() / 2.0f), Vector2(getHeight() / 2.0f, getHeight() / 2.0f), Vector2(-0.2f, 0.2f), Vector2(-0.2f, 0.2f), Vector2(2.0f, 2.0f), Vector2(4.0f, 4.0f), "test_particle"); 
	_emitter->setDestroyParticleOnTouch(true);
	_emitter->setDispatchEventOnParticleDeadByTouch(true);
	_emitter->setRadius(Vector2(20.0f, 25.0f));
	_emitter->addEventListener(Particle::ParticleEvent::DIE_EVENT, CLOSURE(this, &PopObjectsField::onParticlePopped));
	addChild(_emitter);
	_particlesPoppedCount = 0;

	_container = new ParticlesContainer();
	addChild(_container);
}

PopObjectsField::~PopObjectsField() {

}

void PopObjectsField::onParticlePopped(Event *ev) {
	Particle::ParticleEvent* event = static_cast<Particle::ParticleEvent*>(ev);
	++_particlesPoppedCount;

	unsigned int whichColor = 2863311530;
	_container->addParticle(event->particle->getPosition(), Vector2(0.0f, 0.0f), 0, whichColor, "test_particle", 1.0f, 1.0f, event->particle->r, false);

	// animate this particle to specified position
	// animate this 
}