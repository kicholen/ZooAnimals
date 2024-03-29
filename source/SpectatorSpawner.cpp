#include "SpectatorSpawner.h"


SpectatorSpawner::SpectatorSpawner(int spectatorsOnScreen, float spectatorsHeight) {
	_tracks._vector.resize(0);
	_tracks._vector.reserve(spectatorsOnScreen);
	_resAnims._vector.resize(0);
	_resAnims._vector.reserve(spectatorsOnScreen);
	_spectatorsHeight = spectatorsHeight;
	_spectatorsOnScreen = spectatorsOnScreen;
	_frameTime = 0.0f;
	_container = new SpectatorsContainer();
	addChild(_container);
}

SpectatorSpawner::~SpectatorSpawner() {
	_container->removeSpectators();
	_tracks._vector.resize(0);
	_resAnims._vector.resize(0);
}

void SpectatorSpawner::addResAnim(const std::string &resAnim, bool shouldClear) {
	if (shouldClear) {
		_resAnims._vector.resize(0);
	}
	_resAnims.push(resAnim);
}

void SpectatorSpawner::addTrack(const VectorArray<Vector2>& track, bool shouldClear) {
	if (shouldClear) {
		_tracks._vector.resize(0);
	}
	_tracks.push(track);
}

void SpectatorSpawner::doUpdate(const UpdateState &us) {
	if (_container->getSpectatorsInUseCount() < _spectatorsOnScreen) {
		spawnSpectator();
	}
}

void SpectatorSpawner::spawnSpectator() {
	unsigned int whichTrack = _tracks.length() > 1 ? CMath::random(0, _tracks.length()) : 0;
	unsigned int whichAnim = _resAnims.length() > 1 ? CMath::random(0, _resAnims.length()) : 0;

	_container->addSpectator(_tracks[whichTrack], _resAnims[whichAnim], _spectatorsHeight);
}
