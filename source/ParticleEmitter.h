#ifndef _PARTICLEEMITTER_
#define _PARTICLEEMITTER_

#include "Actor.h"
#include "ParticlesContainer.h"
#include "FlashUtils.h"

using namespace FlashUtils;
using namespace oxygine;

DECLARE_SMART(ParticleEmitter, spParticleEmitter);

class ParticleEmitter : public Actor
{
public:
	ParticleEmitter(Vector2 xPosition, Vector2 yPosition, Vector2 xVelocity, Vector2 yVelocity, Vector2 lifeTime, Vector2 particlesPerSecond, const string &resAnim);
	~ParticleEmitter();
	
	void setFriction(float friction);
	void setRadius(Vector2 radius);
	void setLifetime(Vector2 lifetime);
	void setRotation(Vector2 rotation);
	void pushColor(unsigned int color, bool shouldClear = false);
	void pushResAnim(const string &resAnim, bool shouldClear = false);

	void setDestroyParticleOnTouch(bool shouldDestroy) {_flags &= ~flag_destroyOnTouch; if (shouldDestroy) _flags |= flag_destroyOnTouch;}
	void setDispatchEventOnParticleDead(bool shouldDispatch) {_flags &= ~flag_dispatchOnDead; if (shouldDispatch) _flags |= flag_dispatchOnDead;}

protected:
	bool getIsParticleDestroyedOnTouch() {return (_flags & flag_destroyOnTouch) != 0;}
	bool getIsEventDispatchedOnParticleDead() {return (_flags & flag_dispatchOnDead) != 0;}

	virtual void doUpdate(const UpdateState &us);
	void spawnParticle();
	void onParticleDie(Event *ev);

private:
	mutable unsigned short _flags;

	enum particle_flags
	{
		flag_destroyOnTouch	= 1,
		flag_dispatchOnDead	= 1 << 1
	};
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
	VectorArray<string> _resAnims;

	float _frameTime;

	spParticlesContainer _container;
};

#endif