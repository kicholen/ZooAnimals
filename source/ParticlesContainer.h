#ifndef _PARTICLESCONTAINER_
#define _PARTICLESCONTAINER_

#include "FlashUtils.h"
#include "Particle.h"

using namespace FlashUtils;

DECLARE_SMART(ParticlesContainer, spParticlesContainer);

class ParticlesContainer : public Actor
{
public:
	ParticlesContainer();
	~ParticlesContainer();

	void thinkParticles(float dt);
	void removeParticles();
	
	spParticle addParticle(Vector2 position, Vector2 velocity, float angle, unsigned int ncol, const string &resAnim, float lifetime, float friction, float radius, bool shouldKillOnTouch);

private:
	spParticle createParticle(Vector2 position, Vector2 velocity, float angle, unsigned int ncol);

	spParticle getParticle(Vector2 position, Vector2 velocity, float angle, unsigned int ncol);
	spParticle getParticleFromPool(Vector2 position, Vector2 velocity, float angle, unsigned int ncol);
	void addEventListenersToParticle(spParticle particle);

	void onParticleEvent(Event *event);
	void onParticleDie(Event *event);

	VectorArray<spParticle> _particlesArray;
	VectorArray<uint> _particlesInPool;
};

#endif