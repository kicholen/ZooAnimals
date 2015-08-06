#include "FeedAnimationProcess.h"

FeedAnimationProcess::FeedAnimationProcess(spAnimalFarmField farm, spWalkingSpectator spectator) {
	_farm = farm;
	_spectator = spectator;
}

FeedAnimationProcess::~FeedAnimationProcess() {

}

void FeedAnimationProcess::process() {
	_farm->playAnimalsAnimation(2);
	_completed = true;
}

bool FeedAnimationProcess::completed() {
	return _completed;
}