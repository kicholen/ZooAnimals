#ifndef _TILEFIELD_
#define _TILEFIELD_

#include "oxygine-framework.h"
#include "FlashUtils.h"
#include "SharedResources.h"

using namespace FlashUtils;
using namespace oxygine;

DECLARE_SMART(TileField, spTileField);

#define TILE_SIZE_X 32
#define TILE_SIZE_Y 32

class TileField : public Actor
{
public:
	TileField(Point numberOfFields);
	~TileField();

	void setData(string animalName);
	spSprite createTileSprite(string resourceName, Vector2 spriteSize, Point spritePosition, string spriteName, short priority = -1);
private:
	VectorArray<int> getFarmParameters(string animalName);
	Point getCellIndex(Vector2 position);
	Vector2	getCellPosition(int i, int j);
	
	Point _numberOfFields;
	string _animalName;
};

#endif