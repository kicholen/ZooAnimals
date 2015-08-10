#include "SpectatorsArea.h"
#include "SharedResources.h"
#include "StartGameConfig.h"

SpectatorsArea::SpectatorsArea()
{
	_spectatorsArray._vector.resize(0);
	_spectatorsArray._vector.reserve(20);
	_spectatorsInPool._vector.resize(0);
	_spectatorsInPool._vector.reserve(10);
	_lastZSortTime = 0.0f;
	_offsetX = 100.0f;
	_offsetY = 50.0f;
	setTouchEnabled(false);
	setTouchChildrenEnabled(false);
}

SpectatorsArea::~SpectatorsArea()
{
	_spectatorsArray._vector.resize(0);
	_spectatorsInPool._vector.resize(0);
}

void SpectatorsArea::init(int count) {
	float spectatorHeight = getRoot()->getHeight() * 0.07f;
	_unavailableOffset = spectatorHeight / 2.0f;

	VectorArray<std::string> humanResAnims;

	for (int i = 1; i <= HUMAN_CHARS_COUNT; i++) {
		humanResAnims.push("human_" + CMath::intToString(i));
	}

	for (int i = 0; i < count; i++) {
		unsigned int whichAnim = humanResAnims.length() > 1 ? CMath::random(0, humanResAnims.length()) : 0;
		spWalkingSpectator spectator = addSpectator(humanResAnims[whichAnim], spectatorHeight);
		spectator->setPosition(CMath::random(_unavailableOffset, getWidth() - _unavailableOffset), CMath::random(0.0f, getHeight() - _unavailableOffset));
	}
}

void SpectatorsArea::removeSpectators() {
	for (int i = _spectatorsArray.length() - 1; i >= 0; i--) {
		_spectatorsArray[i]->detach();
	}
	_spectatorsArray.clear();
	_spectatorsInPool.clear();
}

void SpectatorsArea::doUpdate(const UpdateState& us) {
	if (_lastZSortTime > 0.0f) {
		_lastZSortTime -= us.dt;
	}
	else {
		_lastZSortTime = SORT_Z_DELAY;

		for (int i = _spectatorsArray.length() - 1; i >= 0; i -= 1) {
			spWalkingSpectator spectator = _spectatorsArray[i];
			spectator->setPriority(int(spectator->getY()));

			int spectatorNumber = spectator->getNumber();
			if (spectatorNumber == 0) {
				spectator->setNumber(1);
				setTmpVectorToRandomPosition(spectator->getPosition());
				spectator->animateMoveToPosition(_tmpVector);
			}
			else if (spectatorNumber > 2) {
				spectator->setNumber(--spectatorNumber);
			}
			else {
				spectator->setNumber(0);
			}
		}
	}
}

void SpectatorsArea::addTooltipToSpectator(spActor tooltip, spActor spectator) {
	tooltip->setScale(Vector2(1.0f / spectator->getScaleX(), 1.0f / spectator->getScaleY()));
	tooltip->setX(spectator->getWidth() / 2.0f);
	spectator->addChild(tooltip);
}

spWalkingSpectator SpectatorsArea::addSpectator(const std::string &resAnim, float height) {
	spWalkingSpectator spectator = getSpectator();
	spectator->setResAnim(tilesResources.getResAnim(resAnim));
	float scale = height / spectator->getHeight();
	spectator->setScale(spectator->getScaleX() * scale, scale);
	addChild(spectator);

	return spectator;
}

int SpectatorsArea::getSpectatorsInUseCount() {
	return _spectatorsArray.length() - _spectatorsInPool.length();
}

spWalkingSpectator SpectatorsArea::getSpectator() {
	if (_spectatorsInPool.length() > 0) {
		return getSpectatorFromPool();
	}
	else {
		return createSpectator();
	}
}

spWalkingSpectator SpectatorsArea::getSpectatorFromPool() {
	spWalkingSpectator spectator = _spectatorsArray[_spectatorsInPool._vector.back()];
	spectator->removeAllEventListeners();
	_spectatorsInPool._vector.pop_back();

	return spectator;
}

void SpectatorsArea::setTmpVectorToRandomPosition(const Vector2& position) {
	float offsetX = 0.0f;
	float offsetY = 0.0f;

	if (position.x == 0.0f) {
		offsetX = CMath::Rand(0.0f, _offsetX);
	}
	else if (position.x == getWidth()) {
		offsetX = CMath::Rand(-_offsetX, 0.0f);
	}
	else {
		offsetX = CMath::Rand(-_offsetX, _offsetX);
	}

	if (position.y == 0.0f) {
		offsetY = CMath::Rand(0.0f, _offsetY);
	}
	else if (position.y == getHeight()) {
		offsetY = CMath::Rand(-_offsetX, 0.0f);
	}
	else {
		offsetY = CMath::Rand(-_offsetX, _offsetX);
	}

	_tmpVector.x = min(max(position.x + offsetX, _unavailableOffset), getWidth() - _unavailableOffset);
	_tmpVector.y = min(max(position.y + offsetY, 0.0f), getHeight() - _unavailableOffset);
}

spWalkingSpectator SpectatorsArea::createSpectator() {
	spWalkingSpectator spectator = new WalkingSpectator();
	_spectatorsArray.push(spectator);

	return spectator;
}