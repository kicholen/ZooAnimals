#ifndef _PARTICLEEMITTER_
#define _PARTICLEEMITTER_

#include "Actor.h"
#include "Particle.h"
#include "FlashUtils.h"

using namespace FlashUtils;
using namespace oxygine;

DECLARE_SMART(ParticleEmitter, spParticleEmitter);

class ParticleEmitter : public Actor
{
public:
	ParticleEmitter(Vector2 xPosition, Vector2 yPosition, Vector2 xVelocity, Vector2 yVelocity, Vector2 lifeTime, Vector2 particlesPerSecond);
	~ParticleEmitter();
	
	void setFriction(float friction);
	void setRadius(Vector2 radius);
	void setLifetime(Vector2 lifetime);
	void setRotation(Vector2 rotation);
	void pushColor(unsigned int color, bool shouldClear = false);
protected:
	//virtual void doUpdate(const UpdateState &us);
	virtual void doUpdate(const UpdateState &us);
	void spawnParticle();
private:
	Vector2 _xPosition;
	Vector2 _yPosition;
	Vector2 _yVelocity;
	Vector2 _xVelocity;
	Vector2 _lifeTime;
	Vector2 _particlesPerSecond;
	Vector2 _rotation;

	Vector2 _radius;
	float _friction;
	VectorArray<unsigned int> _colors;

	float _frameTime;
};

#endif