#include "Particle.h"

Particle::Particle(Vector2 position, Vector2 velocity, float angle, unsigned int ncol, uint number) {
	revive(position, velocity, angle, ncol);
	_number = number;
	friction = 0.01f;

	setAnchor(Vector2(0.5f, 0.5f));
	setTouchEnabled(false);
	draw();
}

void Particle::think(float dt) {
	if (_dead) {
		return;
	}
	draw();
	doVelocity(dt);
	/*float velo = (vel.x * vel.x + vel.y * vel.y);
	
	if (fabsf(vel.x) > 1.0 || fabsf(vel.y) > 1.0) {//(fabsf(getPosition().x) <= 30 && fabsf(getPosition().y) <= 30) {
		setColor(CMath::HexRGBToColor32(0xF9008D));
	}
	else {
		setColor(CMath::HexRGBToColor32(0x49FF45));
	}*/
	if((lifetime > 0) && (age() >= lifetime)) {
		die();
	}
}

void Particle::draw() {

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

void Particle::doVelocity(float dt) {
	_lastX = getX();
	_lastY = getY();
	setPosition(getPosition() + vel * dt);
	vel.x *= (1 - friction);
	vel.y *= (1 - friction);
}

float Particle::age() {
	return float(((getTimeMS() / 1000.0) - _birthday));
}

void Particle::dieByTouch(Event *ev) {
	die(true);
	removeEventListener(TouchEvent::OVER, CLOSURE(this, &Particle::dieByTouch));
}

void Particle::die(bool wasTouched) {
	if(!_dead) {
		detach();
		_dead = true;
		
		ParticleEvent particleEvent(_number, wasTouched);
		dispatchEvent(&particleEvent);
	}
}

void Particle::revive(Vector2 position, Vector2 velocity, float angle, unsigned int ncol) {
	lifetime = 10;
	_dead = false;
	
	setPosition(position);
	setRotation(angle);
	r = 2;
	col = ncol;
	vel = velocity;

	_birthday = float(getTimeMS() / 1000);
}

void Particle::setDieOnTouch(bool shouldDie) {
	if (_shouldDieOnTouch) {
		removeEventListener(TouchEvent::OVER, CLOSURE(this, &Particle::dieByTouch));
	}
	_shouldDieOnTouch = shouldDie;
	if (_shouldDieOnTouch) {
		addEventListener(TouchEvent::OVER, CLOSURE(this, &Particle::dieByTouch));
		setTouchEnabled(true);
	}
	else {
		setTouchEnabled(false);
	}
}