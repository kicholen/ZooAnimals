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
	virtual void clear();

	spParticle addParticle(Vector2 position, Vector2 velocity, float angle, unsigned int ncol, const std::string &resAnim, float lifetime, float friction, float radius, bool shouldKillOnTouch);

protected:
	virtual  void onParticleDie(Event *event);

	VectorArray<spParticle> _particlesArray;
private:
	spParticle createParticle(Vector2 position, Vector2 velocity, float angle, unsigned int ncol);

	spParticle getParticle(Vector2 position, Vector2 velocity, float angle, unsigned int ncol);
	spParticle getParticleFromPool(Vector2 position, Vector2 velocity, float angle, unsigned int ncol);
	void addEventListenersToParticle(spParticle particle);

	void onParticleEvent(Event *event);

	VectorArray<uint> _particlesInPool;
};

#endif