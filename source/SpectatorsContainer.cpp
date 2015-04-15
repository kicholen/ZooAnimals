#include "SpectatorsContainer.h"


SpectatorsContainer::SpectatorsContainer() {
	_spectatorsArray._vector.resize(0);
	_spectatorsArray._vector.reserve(20);
	_spectatorsInPool._vector.resize(0);
	_spectatorsInPool._vector.reserve(10);
}

SpectatorsContainer::~SpectatorsContainer() {
	_spectatorsArray._vector.resize(0);
	_spectatorsInPool._vector.resize(0);
}

void SpectatorsContainer::removeSpectators() {
	for (int i = _spectatorsArray.length() - 1; i >= 0; i--) {
		_spectatorsArray[i]->detach();
	}
	_spectatorsArray.clear();
	_spectatorsInPool.clear();
}

spWalkingSpectator SpectatorsContainer::addSpectator(const VectorArray<Vector2>& trackPoints, const string &resAnim, float height) {
	spWalkingSpectator spectator = getSpectator(trackPoints);
	spectator->setResAnim(tilesResources.getResAnim(resAnim));
	float scale = height / spectator->getHeight();
	spectator->setScale(spectator->getScaleX() * scale, scale);
	addChild(spectator);

	return spectator;
}

int SpectatorsContainer::getSpectatorsInUseCount() {
int asd = _spectatorsArray.length();
int asd2 = _spectatorsInPool.length();
	return _spectatorsArray.length() - _spectatorsInPool.length();
}

spWalkingSpectator SpectatorsContainer::getSpectator(const VectorArray<Vector2>& trackPoints) {
	if (_spectatorsInPool.length() > 0) {
		return getSpectatorFromPool(trackPoints);
	}
	else {
		return createSpectator(trackPoints);
	}
}

spWalkingSpectator SpectatorsContainer::getSpectatorFromPool(const VectorArray<Vector2>& trackPoints) {
	spWalkingSpectator spectator = _spectatorsArray[_spectatorsInPool._vector.back()];
	spectator->removeAllEventListeners();
	_spectatorsInPool._vector.pop_back();
	spectator->revive(trackPoints);
	addEventListenersToSpectator(spectator);

	return spectator;
}

spWalkingSpectator SpectatorsContainer::createSpectator(const VectorArray<Vector2>& trackPoints) {
	spWalkingSpectator spectator = new WalkingSpectator(trackPoints, (uint)_spectatorsArray.length());
	addEventListenersToSpectator(spectator);
	_spectatorsArray.push(spectator);

	return spectator;
}

void SpectatorsContainer::addEventListenersToSpectator(spWalkingSpectator spectator) {
	spectator->addEventListener(WalkingSpectator::WalkingSpectatorEvent::DIE_EVENT, CLOSURE(this, &SpectatorsContainer::onSpectatorDie));
}

void SpectatorsContainer::onSpectatorDie(Event *ev) {
	WalkingSpectator::WalkingSpectatorEvent* event = static_cast<WalkingSpectator::WalkingSpectatorEvent*>(ev);
	_spectatorsInPool.push(event->spectatorNumber);
}