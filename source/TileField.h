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
	TileField(Point numberOfFields, bool enableCullingOnTiles = false);
	~TileField();

	void setData(const std::string& animalName, const std::string& level);
	spSprite createTileSprite(const std::string& resourceName, const Vector2& spriteSize, const Point& spritePosition, short priority = -1);
	spActor createTileActor(const Vector2& actorSize, const Point& actorPosition, short priority = -1);
private:
	VectorArray<int> getFarmParameters(std::string& animalName);
	Point getCellIndex(Vector2 position);
	Vector2	getCellPosition(int i, int j);
	
	Point _numberOfFields;
	std::string _animalName;
	bool _enableCullingOnTiles;
};

#endif