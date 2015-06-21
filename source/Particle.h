#ifndef _PARTICLE_
#define _PARTICLE_
#include "FlashUtils.h"
#include <cmath>
#include "SharedResources.h"

using namespace FlashUtils;

DECLARE_SMART(Particle, spParticle);

class Particle : public Sprite
{
public:

	class ParticleEvent : public Event {
	public:
		enum EV {
			DIE_EVENT = makefourcc('P', 'D', 'E', 'E')
		};

		Particle* particle;
		bool wasTouched;

		ParticleEvent(Particle* particle_, bool byTouch) :Event(DIE_EVENT), particle(particle_), wasTouched(byTouch) {}
	};
	
	Particle(Vector2 position, Vector2 velocity, float angle, unsigned int ncol, uint number);

	//Vars:
	float r;
	unsigned int col;
	Vector2 vel;
	float friction;
	float lifetime;

	uint getNumber() {
		return _number;
	}
private:
	float _birthday;
	uint _number;
	bool _dead;
	bool _shouldDieOnTouch;

	float _lastX;
	float _lastY;
public:
	void setDieOnTouch(bool shouldDie);

	void think(float dt);

	void draw();
private:
	void doVelocity(float dt);
	void dieByTouch(Event *ev);

public:
	float age();

	void die(bool wasTouched = false);
	void revive(Vector2 position, Vector2 velocity, float angle, unsigned int ncol);
};

#endif