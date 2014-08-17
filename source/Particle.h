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
	static spActor TheParticleSprite;
	static Array<spParticle> TheParticleArray;

	static void initialize(const string &particleName);
	static void cleanup();

	static unsigned int think_threads;
	static unsigned int think_cur;

	static void thinkParticles(float dt);

	static void removeParticles();

	Particle(float new_x, float new_y, float new_vx, float new_vy, float angle, unsigned int ncol);

	//Vars:
	float last_x;
	float last_y;

	float r;
	unsigned int col;
	Vector2 vel;
	float friction;
	float lifetime;
	private:
	float birthday;
public:
	bool dead;

	bool blur;
	bool deathPaint;

	//Funcs:
	void think(float dt, unsigned int numThreads);

	void draw();

	//Internal Stuff:
private:
	void doVelocity(float dt, unsigned int numThreads);

public:
	float age();

	void die();
};

#endif