#include "TileBuilder.h"
#include "Content.h"

TileBuilder::TileBuilder()
{
	_bottom = "Bottom";
	_top = "Top";
}


TileBuilder::~TileBuilder()
{
}

spTileField TileBuilder::build(spActor parent, spAnimalModel model, VectorArray<spActor>& sortArray, const std::string& level) {
	_numOfTiles = getNumberOfTiles(parent);
	spTileField tileField = createTileField(parent, model, level);
	createSortElements(tileField, parent, model, sortArray, level);

	return tileField;
}

spTileField TileBuilder::createTileField(spActor actor, spAnimalModel model, const std::string& level) {
	spTileField tileField = new TileField(_numOfTiles);
	tileField->setData(model->animalName(), level);
	tileField->setScale(actor->getHeight() / tileField->getHeight());
	tileField->setName("tajle");
	tileField->setAnchor(0.0f, 0.0f);
	tileField->setPosition(0.0f, 0.0f);
	tileField->setPriority(-500);
	actor->addChild(tileField);
	return tileField;
}

void TileBuilder::createSortElements(spTileField tileField, spActor parent, spAnimalModel model, VectorArray<spActor>& sortArray, const std::string& level) {
	// set first level as default
	pugi::xml_node animalParameters = Content::instance.getAnimalFarmParametersNode(model->animalName()).child(level.c_str()).child("sort");
	pugi::xml_node parameter = animalParameters.first_child();

	while (parameter) {
		const char *name = parameter.name();
		int i = parameter.first_attribute().as_int();
		int j = parameter.first_attribute().next_attribute().as_int();

		if (_numOfTiles.x > i && _numOfTiles.y > j) {
			if (!strcmp(name, "coniferGreen")) { // todo maybe make map for all objects and set tileBuilder as singleton or don't give a fuck and make loader on creating whole view
				createTree(tileField, parent, "coniferGreen", sortArray, i, j);
			}
			else if (!strcmp(name, "coniferLightGreen")) {
				createTree(tileField, parent, "coniferLightGreen", sortArray, i, j);
			}
			else if (!strcmp(name, "coniferRed")) {
				createTree(tileField, parent, "coniferRed", sortArray, i, j);
			}
			else if (!strcmp(name, "treeLightGreen")) {
				createTree(tileField, parent, "treeLightGreen", sortArray, i, j);
			}
			else if (!strcmp(name, "treeGreen")) {
				createTree(tileField, parent, "treeGreen", sortArray, i, j);
			}
			else if (!strcmp(name, "treeRed")) {
				createTree(tileField, parent, "treeRed", sortArray, i, j);
			}
			else {
				spSprite tileSprite = tileField->createTileSprite(parameter.name(), Vector2(TILE_SIZE_X, TILE_SIZE_Y), Point(i, j), 1);
				tileSprite->setAnchor(0.0f, 1.0f);
				tileSprite->setScale(tileSprite->getScale() * tileField->getScaleX());
				tileSprite->setY(tileSprite->getY() + TILE_SIZE_Y);
				sortArray.push(tileSprite);
				parent->addChild(tileSprite);
			}
		}
		parameter = parameter.next_sibling();
	}
}

void TileBuilder::createTree(spTileField tileField, spActor parent, const std::string& name, VectorArray<spActor>& sortArray, int i, int j) {
	spActor actor = tileField->createTileActor(Vector2(TILE_SIZE_X, TILE_SIZE_Y * 2.0f), Point(i, j), 1);
	actor->setScale(tileField->getScaleX());
	actor->setAnchor(0.0f, 1.0f);
	actor->setY(actor->getY() + TILE_SIZE_Y);
	attachSprite(actor, name + _bottom, Vector2(0.0f, actor->getHeight() / 2.0f));
	attachSprite(actor, name + _top, Vector2(0.0f, 0.0f));
	sortArray.push(actor);
	parent->addChild(actor);
}

void TileBuilder::attachSprite(spActor actor, const std::string& resAnim, const Vector2& position) {
	spSprite sprite = new Sprite();
	sprite->setAnchor(0.0f, 0.0f);
	sprite->setResAnim(tilesResources.getResAnim(resAnim));
	sprite->setPosition(position);
	sprite->setScale((actor->getHeight() / 2.0f) / sprite->getHeight());
	sprite->attachTo(actor);
}

Point TileBuilder::getNumberOfTiles(spActor actor) {
	float ratio = (actor->getSize().x / 32.0f) / (actor->getSize().y / 32.0f);
	return Point(int(ratio * 6.0f), 6);
}