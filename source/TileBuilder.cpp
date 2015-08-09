#include "TileBuilder.h"
#include "Content.h"

TileBuilder::TileBuilder()
{
}


TileBuilder::~TileBuilder()
{
}

spTileField TileBuilder::build(spActor parent, spAnimalModel model, VectorArray<spActor>& sortArray) {
	_numOfTiles = getNumberOfTiles(parent);
	spTileField tileField = createTileField(parent, model);
	createSortElements(tileField, parent, model, sortArray);

	return tileField;
}

spTileField TileBuilder::createTileField(spActor actor, spAnimalModel model) {
	spTileField tileField = new TileField(_numOfTiles);
	tileField->setData(model->animalName());
	tileField->setScale(actor->getHeight() / tileField->getHeight());
	tileField->setName("tajle");
	tileField->setAnchor(0.0f, 0.0f);
	tileField->setPosition(0.0f, 0.0f);
	tileField->setPriority(-500);
	actor->addChild(tileField);
	return tileField;
}

void TileBuilder::createSortElements(spTileField tileField, spActor parent, spAnimalModel model, VectorArray<spActor>& sortArray) {
	pugi::xml_node animalParameters = Content::instance.getAnimalFarmParametersNode(model->animalName()).child("sort");
	pugi::xml_node parameter = animalParameters.first_child();

	while (parameter) {
		const char *name = parameter.name();
		int i = parameter.first_attribute().as_int();
		int j = parameter.first_attribute().next_attribute().as_int();

		if (!strcmp(name, "coniferGreen")) {
			createMergedElement(tileField, parent, sortArray, i, j);
		}
		else {
			spSprite tileSprite = tileField->createTileSprite(parameter.name(), Vector2(TILE_SIZE_X, TILE_SIZE_Y), Point(i, j), 1);
			tileSprite->setAnchor(0.0f, 1.0f);
			tileSprite->setY(tileSprite->getY() + TILE_SIZE_Y);
			sortArray.push(tileSprite);
			parent->addChild(tileSprite);
		}
		parameter = parameter.next_sibling();
	}
}

void TileBuilder::createMergedElement(spTileField tileField, spActor parent, VectorArray<spActor>& sortArray, int i, int j) {
	spActor actor = tileField->createTileActor(Vector2(TILE_SIZE_X, TILE_SIZE_Y), Point(i, j), 1);
	actor->setAnchor(0.0f, 1.0f);
	actor->setY(actor->getY() + TILE_SIZE_Y);
	sortArray.push(actor);
	parent->addChild(actor);
}

Point TileBuilder::getNumberOfTiles(spActor actor) {
	float ratio = (actor->getSize().x / 32.0f) / (actor->getSize().y / 32.0f);
	return Point(int(ratio * 6.0f), 6);
}