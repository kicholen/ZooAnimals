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

	spTileField build(spActor actor, spAnimalModel model, VectorArray<spActor>& sortArray);

private:
	spTileField createTileField(spActor actor, spAnimalModel model);
	void createSortElements(spTileField tileField, spActor parent, spAnimalModel model, VectorArray<spActor>& sortArray);
	void createMergedElement(spTileField tileField, spActor parent, VectorArray<spActor>& sortArray, int i, int j);

	Point getNumberOfTiles(spActor actor);

private:
	Point _numOfTiles;
};

#endif