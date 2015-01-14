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

		uint particleNumber;

		ParticleEvent(uint number):Event(DIE_EVENT), particleNumber(number) {}
	};
	
	Particle(Vector2 position, Vector2 velocity, float angle, unsigned int ncol, uint number);

	//Vars:
	float last_x;
	float last_y;

	float r;
	unsigned int col;
	Vector2 vel;
	float friction;
	float lifetime;

private:
	float _birthday;
	uint _number;
	bool _dead;
	bool _shouldDieOnTouch;

public:
	void setDieOnTouch(bool shouldDie);

	void think(float dt);

	void draw();
private:
	void doVelocity(float dt);

public:
	float age();

	void die(Event *ev = NULL);
	void revive(Vector2 position, Vector2 velocity, float angle, unsigned int ncol);
};

#endif