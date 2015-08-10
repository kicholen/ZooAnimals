#ifndef _TILEBUILDER_
#define _TILEBUILDER_

#include "oxygine-framework.h"
#include "TileField.h"
#include "AnimalModel.h"

using namespace oxygine;

DECLARE_SMART(TileBuilder, spTileBuilder);

class TileBuilder : public Object
{
public:
	TileBuilder();
	~TileBuilder();

	spTileField build(spActor actor, spAnimalModel model, VectorArray<spActor>& sortArray, const std::string& level);

private:
	spTileField createTileField(spActor actor, spAnimalModel model, const std::string& level);
	void createSortElements(spTileField tileField, spActor parent, spAnimalModel model, VectorArray<spActor>& sortArray, const std::string& level);
	void createTree(spTileField tileField, spActor parent, const std::string& name, VectorArray<spActor>& sortArray, int i, int j);

	void attachSprite(spActor actor, const std::string& resAnim, const Vector2& position);

	Point getNumberOfTiles(spActor actor);

private:
	Point _numOfTiles;

	std::string _bottom;
	std::string _top;
};

#endif