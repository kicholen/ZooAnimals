#include "AnimalsFarmAnimations.h"
#include "math.h"

AnimalsFarmAnimations::AnimalsFarmAnimations(const Vector2& fieldSize) {
	_playFieldSize = fieldSize;
	_lastZSortTime = 0.0f;
	_nextSnakeUpdate = -1.0f;
	_snakeLength = 0;
	_canAnimalsUpdate = true;
	_animalsArray._vector.resize(0);
	_animalsArray._vector.reserve(10);
}

AnimalsFarmAnimations::~AnimalsFarmAnimations() {
	_animalsArray._vector.resize(0);
}

void AnimalsFarmAnimations::addAnimal(spAnimalInFarmElement animal) {
	_animalsArray.push(animal);
}

spAnimalInFarmElement AnimalsFarmAnimations::removeAnimal() {
	spAnimalInFarmElement element = _animalsArray._vector.back();
	_animalsArray._vector.pop_back();
	return element;
}

int AnimalsFarmAnimations::getAnimalsCount() {
	return _animalsArray.length();
}

spAnimalInFarmElement AnimalsFarmAnimations::getAnimalByNumber(int value) {
	return _animalsArray[value];
}

void AnimalsFarmAnimations::animateAnimalsJump(Vector2 position) {
	for (int i = _animalsArray.length() - 1; i >= 0; i -= 1) {
		_animalsArray[i]->animateJump(position);
	}
}

void AnimalsFarmAnimations::doUpdate(const UpdateState& us, bool isOnScreen) {
	if (_lastZSortTime > 0.0f) {
		_lastZSortTime -= us.dt;
	}
	else {
		_lastZSortTime = FlashUtils::CMath::Rand(SORT_Z_DELAY_MIN, SORT_Z_DELAY_MAX);

		if (isOnScreen) {
			setAnimalsPriorityByY();

			if (!_canAnimalsUpdate) {
				_canAnimalsUpdate = true;
				for (int i = _animalsArray.length() - 1; i >= 0; i -= 1) {
					_animalsArray[i]->_canUpdate = _canAnimalsUpdate;
				}
			}
		}
		else if (_canAnimalsUpdate) {
			_canAnimalsUpdate = false;
			for (int i = _animalsArray.length() - 1; i >= 0; i -= 1) {
				_animalsArray[i]->_canUpdate = _canAnimalsUpdate;
			}
		}
	}
	/*if (_nextSnakeUpdate > 0.0f) {
		_nextSnakeUpdate -= us.dt;
		if (_nextSnakeUpdate < 0.0f) {
			_nextSnakeUpdate = 0.0f;
		}
	}
	else if (_nextSnakeUpdate == 0.0f) {
		if (!isSnakeCompleted()) {
			if (_snakeLength == 0 || canSnakeMoveToNextAnimal()) {
				moveSnakeToNextAnimalPosition();
			}
		}
	}*/
}

void AnimalsFarmAnimations::attachElementToRandomAnimal(spActor actor) {
	int max = _animalsArray.length() > 1 ? _animalsArray.length() - 1 : 1;
	_animalsArray[CMath::random(0, max)]->addTooltipToAnimal(actor);
}

void AnimalsFarmAnimations::playAnimalsAnimation(AnimalsFarmAnimationType animationType) {
	switch (animationType) {
	case afaVertical:
		playVerticalAnimation();
		break;
	case afaHorizontal:
		playHorizontalAnimation();
		break;
	case afaCircle:
		playCircleAnimation();
		break;
	case afaSmile:
		playSmileAnimation();
		break;
	case afaSnake:
		playSnakeAnimation();
		break;
	default:
		stopJumpingAnimation();
		log::warning("Incorrect animals farm animation type.");
	}
}

void AnimalsFarmAnimations::playVerticalAnimation() {
	float spaceX = _playFieldSize.x / (_animalsArray.length() + 1);
	float offset = 0.0f;

	for (int i = _animalsArray.length() - 1; i >= 0; i -= 1) {
		offset += spaceX;
		_animalsArray[i]->jumpToExactPosition(Vector2(offset, _playFieldSize.y / 2));
	}
}

void AnimalsFarmAnimations::playHorizontalAnimation() {
	float spaceY = _playFieldSize.y / (_animalsArray.length() + 1);
	float offset = 0.0f;
	
	for (int i = _animalsArray.length() - 1; i >= 0; i -= 1) {
		_animalsArray[i]->jumpToExactPosition(Vector2(_playFieldSize.x / 2, offset));
		offset += spaceY;
	}
}

void AnimalsFarmAnimations::playCircleAnimation() {
	float startAngle = 0.0f;
	float angleOffset = 360.0f / (float)_animalsArray.length();
	float childDistanceFromCenter = _playFieldSize.y / 2 * 0.8f;

	for (int i = _animalsArray.length() - 1; i >= 0; i -= 1) {
		_animalsArray[i]->jumpToExactPosition(getPositionFromCenter(FlashUtils::CMath::DegToRad(startAngle), _playFieldSize / 2, childDistanceFromCenter));
		startAngle += angleOffset;
	}
}

void AnimalsFarmAnimations::playSmileAnimation() {
	int counter = 0;
	float spaceX = _playFieldSize.x / (_animalsArray.length() + 1);
	float spaceY = _playFieldSize.y / (_animalsArray.length() + 1);
	float offset = spaceY * _animalsArray.length() / 3;
	float secondOffset = _playFieldSize.x / 2 - spaceX * 4;
	float thirdOffset = 0.0f;
	bool firstTime = true;
	float startAngleSecond = -90.0f;
	float angleOffset = 360.0f / (float)_animalsArray.length();
	float childDistanceFromCenter = _playFieldSize.y / 2 * 0.8f;

	for (int i = _animalsArray.length() - 1; i >= 0; i -= 1) {
		if (counter >  _animalsArray.length() / 2) {
			if (counter > _animalsArray.length() / 4 * 3) {
				if (counter > _animalsArray.length() / 8 * 7) {
					if (firstTime) {
						thirdOffset = _playFieldSize.x - secondOffset;
						firstTime = false;
					}
					thirdOffset += spaceX;
					_animalsArray[i]->jumpToExactPosition(Vector2(thirdOffset, _playFieldSize.y * 0.3f));
				}
				else {
					_animalsArray[i]->jumpToExactPosition(Vector2(secondOffset, _playFieldSize.y * 0.3f));
					secondOffset += spaceX;
				}
			}
			else {
				offset += spaceY;
				_animalsArray[i]->jumpToExactPosition(Vector2(_playFieldSize.x / 2, offset));
			}
		}
		else {
			_animalsArray[i]->jumpToExactPosition(getPositionFromCenter(FlashUtils::CMath::DegToRad(startAngleSecond), _playFieldSize / 2, childDistanceFromCenter));
			startAngleSecond += angleOffset;
		}
		counter++;
	}
}

void AnimalsFarmAnimations::playSnakeAnimation() {
	/*bool wasLeaderSet = false;
	for (int i = _animalsArray.length() - 1; i >= 0; i -= 1) {
		if (!wasLeaderSet) {
			_nextSnakeUpdate = _animalsArray[i]->getJumpTime();
			_animalsArray[i]->setAsLeader();
			wasLeaderSet = true;
		}
		_animalsArray[i]->stopJumping();
		//_animalQueueUpdateTime = _animalsArray[i]->getJumpTime();
	}*/
}

void AnimalsFarmAnimations::moveSnakeToNextAnimalPosition() {
	++_snakeLength;
	for (int i = _animalsArray.length() - 1; i >= _animalsArray.length() - _snakeLength; i -= 1) {
		_animalsArray[i]->resumeJumping();
		_targetPosition = Vector2(_animalsArray[i - 1]->getShadowX(), _animalsArray[i - 1]->getShadowY());
		_animalsArray[i]->jumpToExactPosition(_targetPosition);
	}
}

bool AnimalsFarmAnimations::isSnakeCompleted() {
	return _snakeLength == _animalsArray.length();
}

bool AnimalsFarmAnimations::canSnakeMoveToNextAnimal() {
	Vector2 position = Vector2(_animalsArray[_animalsArray.length() -1]->getShadowX(), _animalsArray[_animalsArray.length() -1]->getShadowY());
	return _targetPosition.x == position.x && _targetPosition.y == position.y;//_animalsArray[_animalsArray.length() - 1]->getShadowX() == _animalsArray[_animalsArray.length() - 2]->getShadowX() && _animalsArray[_animalsArray.length() - 1]->getShadowY() == _animalsArray[_animalsArray.length() - 2]->getShadowY();
}

void AnimalsFarmAnimations::stopJumpingAnimation() {
	for (int i = _animalsArray.length() - 1; i >= 0; i -= 1) {
		_animalsArray[i]->stopJumpingExact();
	}
}

void AnimalsFarmAnimations::setAnimalsPriorityByY() {
	for (int i = _animalsArray.length() - 1; i >= 0; i -= 1) {
		_animalsArray[i]->setPriority(int(_animalsArray[i]->getShadowY()));
	}
}

Vector2 AnimalsFarmAnimations::getPositionFromCenter(float angle, Vector2 center, float distanceFromCenter) {
	return Vector2(center + Vector2(sin(angle) * distanceFromCenter, cos(angle) * distanceFromCenter));
}
