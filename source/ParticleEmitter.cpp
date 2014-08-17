#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter(Vector2 xPosition, Vector2 yPosition, Vector2 xVelocity, Vector2 yVelocity, Vector2 lifeTime, Vector2 particlesPerSecond) {
	_xPosition = xPosition;
	_yPosition = yPosition;
	_yVelocity = yVelocity;
	_xVelocity = xVelocity;
	_lifeTime = lifeTime;
	_particlesPerSecond = particlesPerSecond;
	_colors._vector.resize(0);
	_colors._vector.reserve(1);
	_colors.push(1);
	_friction = 0.002;
	_radius = Vector2(1.0f, 2.0f);

	_frameTime = 0.0f;

	_rotation = Vector2(1 * MATH_PI / 180, 360 * MATH_PI / 180);

	addChild(Particle::TheParticleSprite);
}

ParticleEmitter::~ParticleEmitter() {
	Particle::cleanup();
	_colors._vector.resize(0);
}

void ParticleEmitter::doUpdate(const UpdateState &us) {
	_frameTime += us.dt;
	int newParticles = int(CMath::Rand(_particlesPerSecond.x, _particlesPerSecond.y) * _frameTime / 1000);
	if (newParticles != 0) {
		_frameTime = 0.0f;
	}
	for (int i = 0; i < newParticles; i++) {
		spawnParticle();
	}
	Particle::thinkParticles(float(us.dt));
}

void ParticleEmitter::spawnParticle() {
	unsigned int whichColor = _colors.length() > 1 ? CMath::random(0, _colors.length()) : 0;
	Particle *part = new Particle(CMath::Rand(_xPosition.x, _xPosition.y), CMath::Rand(_yPosition.x, _yPosition.y) , CMath::Rand(_xVelocity.x, _xVelocity.y), CMath::Rand(_yVelocity.x, _yVelocity.y), CMath::Rand(_rotation.x, _rotation.y), _colors[whichColor]);
	part->lifetime = CMath::Rand(_lifeTime.x, _lifeTime.y);
	part->friction = _friction;
	part->r = CMath::Rand(_radius.x, _radius.y);
}

void ParticleEmitter::setFriction(float friction) {
	_friction = friction;
}

void ParticleEmitter::setRadius(Vector2 radius) {
	_radius = radius;
}

void ParticleEmitter::setLifetime(Vector2 lifetime) {
	_lifeTime = lifetime;
}

void ParticleEmitter::pushColor(unsigned int color, bool shouldClear) {
	if (shouldClear) {
		_colors._vector.resize(0);
	}
	_colors.push(color);
}

void ParticleEmitter::setRotation(Vector2 rotation) {
	_rotation = rotation;
}