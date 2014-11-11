#include "Particle.h"
#include "FlashUtils.h"
#include <vector>

using namespace std;

spActor Particle::TheParticleSprite;
VectorArray<spParticle> Particle::TheParticleArray;

ResAnim *particleResAnim = 0;

void Particle::initialize(const string &particleName) {
	TheParticleSprite = new Actor();
	TheParticleArray._vector.resize(0);
	TheParticleArray._vector.reserve(100);

	particleResAnim = gameResources.getResAnim(particleName);
} 

void Particle::cleanup() {
	if (TheParticleSprite) {
		TheParticleSprite->detach();
	}
	TheParticleSprite = 0;
	TheParticleArray._vector.resize(0);
} 

unsigned int Particle::think_threads = 1;
unsigned int Particle::think_cur = 0;

void Particle::thinkParticles(float dt) {
	for(int i = TheParticleArray.length() - 1 - think_cur; i >= 0; i -= static_cast<int>(think_threads)) {
		TheParticleArray[i]->think(dt, think_threads);
	}
	think_cur += 1;
	if(think_cur > (think_threads - 1)) {
		think_cur = 0;
	}
}

void Particle::removeParticles() {
	for(int i = TheParticleArray.length() - 1; i >= 0; i--) {
		spParticle b = TheParticleArray[i];
		TheParticleSprite->removeChild(b);
	}
	TheParticleArray._vector.resize(0);
}

Particle::Particle(float new_x, float new_y, float new_vx, float new_vy, float new_angle, unsigned int ncol) {
	friction = 0.01f;
	lifetime = 10;
	dead = false;
	
	setPosition(Vector2(new_x, new_y));
	setRotation(new_angle);
	r = 2;
	col = ncol;
	vel = Vector2(new_vx, new_vy);

	TheParticleSprite->addChild(this);
	TheParticleArray.push(this);

	draw();

	setAnimFrame(particleResAnim);
	setAnchor(Vector2(0.5f, 0.5f));

	birthday = float(getTimeMS() / 1000);
	setAnchor(Vector2(0.5f, 0.5f));
	setInputEnabled(false);
}

void Particle::think(float dt, unsigned int numThreads) {
	draw();
	doVelocity(dt, numThreads);

	/*
	if(playerGravity) {
		doPlayerGravity(dt);
	}
	*/

	if((lifetime > 0) && (age() >= lifetime)) {
		die();
	}
}

void Particle::draw() {
	float velo = (vel.x * vel.x + vel.y * vel.y);

	float sx = r / 3.5f;
	float sy = r / 3.5f;
	setScale(Vector2(sx, sy));
	
	if (col != 1) {
		Color32 c = CMath::HexRGBToColor32(col);
		setColor(c);
	}
	float ageRatio = (1 - age() / lifetime) * 255;
	setAlpha(ageRatio);
}

void Particle::doVelocity(float dt, unsigned int numThreads) {
	last_x = getX();
	last_y = getY();
	setPosition(getPosition() + vel * dt);
	vel.x *= (1 - friction);
	vel.y *= (1 - friction);
}

float Particle::age() {
	return float(((getTimeMS()/1000.0) - birthday));
}

void Particle::die() {
	if(!dead) {
		detach();

		dead = true;
		for(int i = TheParticleArray.length() - 1; i >= 0; i--) {
			if(TheParticleArray[i] == this) {
				TheParticleArray.splice(i, true);
			}
		}
	}
}
