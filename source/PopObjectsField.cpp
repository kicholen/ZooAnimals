#include "PopObjectsField.h"

PopObjectsField::PopObjectsField(const Vector2& size, const std::string& difficulty) {
	_difficulty = difficulty;
	setTouchEnabled(false);
	setSize(size);
	setData();
	_particlesPoppedCount = 0;
}

PopObjectsField::~PopObjectsField() {

}

bool PopObjectsField::isCorrect() {
	return _equation->getResult() == _container->count();
}

void PopObjectsField::reset() {
	_equation->reset(10, "+", 2);
	_container->clear();
}

void PopObjectsField::setData() {
	createEquation();
	createContainer();
	createEmitter();
}

void PopObjectsField::createContainer() {
	_container = new PopObjectsContainer(Vector2(getWidth() / 2.0f, getHeight() * 0.66f));
	_container->setPosition(0.0f, getHeight() * 0.33f);
	addChild(_container);
}

void PopObjectsField::createEmitter() {
	_emitter = new ParticleEmitter(Vector2(getWidth() / 2.0f, getWidth()), Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f), Vector2(0.1f, 0.2f), Vector2(20.0f, 20.0f), Vector2(1.0f, 3.0f), "test_particle");
	_emitter->setDestroyParticleOnTouch(true);
	_emitter->setDispatchEventOnParticleDeadByTouch(true);
	_emitter->setRadius(Vector2(20.0f, 25.0f));
	_emitter->addEventListener(Particle::ParticleEvent::DIE_EVENT, CLOSURE(this, &PopObjectsField::onParticlePopped));
	addChild(_emitter);
}

void PopObjectsField::createEquation() {
	_equation = new EquationElement(Vector2(getWidth() / 2.0f, getHeight() * 0.33f), 8, "-", 6);
	_equation->setPosition(0.0f, 0.0f);
	_equation->setTouchChildrenEnabled(false);
	_equation->setTouchEnabled(false);
	_equation->attachTo(this);
}

void PopObjectsField::onParticlePopped(Event *ev) {
	if (_container->isFreeSpace()) {
		Particle::ParticleEvent* event = static_cast<Particle::ParticleEvent*>(ev);
		++_particlesPoppedCount;
		spParticle particle = event->particle;
		unsigned int whichColor = 2863311530;
		_container->addParticle(Vector2(particle->getX(), particle->getY() - _container->getY()), particle->getRotation(), whichColor, "test_particle", particle->r);
	}
}