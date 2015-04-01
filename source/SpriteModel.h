#ifndef _SPRITEMODEL_
#define _SPRITEMODEL_
#include "oxygine_include.h"

namespace oxygine
{	
	class SpriteModel
	{
	public:
		float scaleX; // it's scale to screen for example it will cover 5% of the screen size in X 
		float scaleY;
		float positionX;// it's position in percent 0 - 100 %
		float positionY;
		int zPriority;
		char spriteName[20]; // sprite name

		SpriteModel(float scaleX, float scaleY, float positionX, float positionY, int zPriority, const char *spriteName);
	};

}
#endif