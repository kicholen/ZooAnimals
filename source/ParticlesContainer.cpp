#include "ParticlesContainer.h"

ParticlesContainer::ParticlesContainer() {
	_particlesArray._vector.resize(0);
	_particlesArray._vector.reserve(100);
	_particlesInPool._vector.resize(10);
}

ParticlesContainer::~ParticlesContainer() {
	_particlesArray._vector.resize(0);
	_particlesInPool._vector.resize(0);
}

void ParticlesContainer::removeParticles() {
	for(int i = _particlesArray.length() - 1; i >= 0; i--) {
		_particlesArray[i]->detach();
	}
	_particlesArray.clear();
	_particlesInPool.clear();
}

void ParticlesContainer::thinkParticles(float dt) {
	for(int i = _particlesArray.length() - 1; i >= 0; i--) {
		_particlesArray[i]->think(dt);
	}
}

void ParticlesContainer::addParticle(Vector2 position, Vector2 velocity, float angle, unsigned int ncol, const string &resAnim, float lifetime, float friction, float radius) {
	spParticle particle = getParticle(position, velocity, angle, ncol);
	particle->lifetime = lifetime;
	particle->r = radius;
	particle->friction = friction;
	particle->setResAnim(gameResources.getResAnim(resAnim));

	addChild(particle);
	_particlesArray.push(particle);
}

spParticle ParticlesContainer::getParticle(Vector2 position, Vector2 velocity, float angle, unsigned int ncol) {
	if (_particlesInPool.length() > 0) {
		return getParticleFromPool(position, velocity, angle, ncol);
	}
	else {
		return createParticle(position, velocity, angle, ncol);
	}
}

spParticle ParticlesContainer::getParticleFromPool(Vector2 position, Vector2 velocity, float angle, unsigned int ncol) {
	spParticle particle = _particlesArray[_particlesInPool._vector[_particlesInPool.length() - 1]];
	_particlesInPool._vector.pop_back();
	particle->revive(position, velocity, angle, ncol);

	return particle;
}

spParticle ParticlesContainer::createParticle(Vector2 position, Vector2 velocity, float angle, unsigned int ncol) {
	spParticle particle = new Particle(position, velocity, angle, ncol);
	particle->addEventListener(Particle::ParticleEvent::DIE_EVENT, CLOSURE(this, &ParticlesContainer::onParticleDie));

	return particle;
}

void ParticlesContainer::onParticleDie(Event *ev) {
	Particle::ParticleEvent* event = static_cast<Particle::ParticleEvent*>(ev);
	_particlesInPool.push(event->particleNumber);
}