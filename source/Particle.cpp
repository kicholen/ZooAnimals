#include "Particle.h"

Particle::Particle(Vector2 position, Vector2 velocity, float angle, unsigned int ncol, uint number) {
	revive(position, velocity, angle, ncol);
	_number = number;

	setAnchor(Vector2(0.5f, 0.5f));
	setInputEnabled(false);
	draw();
}

void Particle::think(float dt) {
	if (_dead) {
		return;
	}
	draw();
	doVelocity(dt);

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

void Particle::doVelocity(float dt) {
	last_x = getX();
	last_y = getY();
	setPosition(getPosition() + vel * dt);
	vel.x *= (1 - friction);
	vel.y *= (1 - friction);
}

float Particle::age() {
	return float(((getTimeMS()/1000.0) - _birthday));
}

void Particle::die(Event *ev) {
	if(!_dead) {
		detach();

		_dead = true;

		removeEventListener(TouchEvent::OVER, CLOSURE(this, &Particle::die));
		ParticleEvent particleEvent(_number);
		dispatchEvent(&particleEvent);
	}
}

void Particle::revive(Vector2 position, Vector2 velocity, float angle, unsigned int ncol) {
	friction = 0.01f;
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
		removeEventListener(TouchEvent::OVER, CLOSURE(this, &Particle::die));
	}
	_shouldDieOnTouch = shouldDie;
	if (_shouldDieOnTouch) {
		addEventListener(TouchEvent::OVER, CLOSURE(this, &Particle::die));
		setInputEnabled(true);
	}
	else {
		setInputEnabled(false);
	}
}