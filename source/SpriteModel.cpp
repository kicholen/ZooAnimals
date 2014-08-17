#include "SpriteModel.h"

namespace oxygine
{

	SpriteModel::SpriteModel(float scaleX, float scaleY, float positionX, float positionY, int zPriority, const char *spriteName) {
		this->scaleX = scaleX;
		this->scaleY = scaleY;
		this->positionX = positionX;
		this->positionY = positionY;
		this->zPriority = zPriority;
		strcpy(this->spriteName, spriteName);
	}

}