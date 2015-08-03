#include "PopObjectsContainer.h"

PopObjectsContainer::PopObjectsContainer(const Vector2& size) {
	createBackground(size);
	setTouchEnabled(false);
	_radius = size.y / 10.0f;
	_offset = (size.x - (getColumns() - 1) * (_radius * 2.0f) - _radius) / 2.0f;
	_count = 0;
}

PopObjectsContainer::~PopObjectsContainer() {

}

void PopObjectsContainer::clear() {
	_freeSlots.clear();
	ParticlesContainer::clear();
	_count = 0;
}

int PopObjectsContainer::count() {
	return _count;
}

bool PopObjectsContainer::isFreeSpace() {
	return _count <= (getRows() - 1) * getColumns();
}

spParticle PopObjectsContainer::addParticle(const Vector2& position, float angle, unsigned int ncol, const std::string& resAnim, float radius) {
	spParticle particle = ParticlesContainer::addParticle(position, Vector2(0.0f, 0.0f), angle, ncol, resAnim, -1.0f, 0.0f, radius, false);
	_count++;
	animateToPosition(particle, getNextObjectPosition());
	return particle;
}

void PopObjectsContainer::animateToPosition(spActor object, const Vector2& position) {
	object->addTween(TweenScale(_radius * 2.0f / object->getHeight()), 500, 1, false, 100);
	object->addTween(TweenPosition(position), 1000, 1, false, 100, Tween::ease_outBack)->addDoneCallback(CLOSURE(this, &PopObjectsContainer::enableTouchOnAnimationFinish));
}

void PopObjectsContainer::onParticleDie(Event *ev) {
	ParticlesContainer::onParticleDie(ev);
	Particle::ParticleEvent* event = static_cast<Particle::ParticleEvent*>(ev);
	Point slot = Point(int((event->particle->getX() + _radius / 2.0f + 10.0f) / (_radius * 2.0f)), int((event->particle->getY() + _radius / 2.0f + 10.0f) / (_radius * 2.0f)));

	_freeSlots.push(slot);
	_count--;
}

void PopObjectsContainer::enableTouchOnAnimationFinish(Event *ev) {
	spTween tween = safeSpCast<Tween>(ev->currentTarget);
	Particle *particle = safeCast<Particle*>(tween->getClient());

	particle->setDieOnTouch(true);
}

int PopObjectsContainer::getRows() {
	return (int)floorf((_background->getHeight() - _radius / 2.0f) / (_radius * 2.0f));
}

int PopObjectsContainer::getColumns() {
	return (int)floorf((_background->getWidth() - _radius / 2.0f) / (_radius * 2.0f));
}

Vector2 PopObjectsContainer::getNextObjectPosition() {
	if (_freeSlots.length() > 0) {
		Point freeSlot = _freeSlots._vector.back();
		_freeSlots._vector.pop_back();
		return getObjectPosition(freeSlot.x, freeSlot.y);
	}
	else {
		int row = (int)ceil((double)_count / (double)getRows());
		int column = _count - (row - 1) * (getColumns() - 1);
		return getObjectPosition(column, row);
	}
}

Vector2 PopObjectsContainer::getObjectPosition(int i, int j) {
	return Vector2(float(i * (_radius * 2.0f) - _radius / 2.0f + _offset), float(j * (_radius * 2.0f) - _radius / 2.0f + 10.0f));
}

void PopObjectsContainer::createBackground(const Vector2& size) {
	_background = new Box9Sprite;
	_background->setTouchEnabled(false);
	_background->setHorizontalMode(Box9Sprite::STRETCHING);
	_background->setVerticalMode(Box9Sprite::STRETCHING);
	_background->setAnchor(0.0f, 0.0f);
	_background->setResAnim(gameResources.getResAnim("greyBox9"));
	_background->setSize(size);
	_background->setGuides(9, 20, 9, 15);
	_background->attachTo(this);
	_background->setPriority(-1);
}
