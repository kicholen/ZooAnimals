#include "TileField.h"
#include "Content.h"

TileField::TileField(Point numberOfFields, bool enableCullingOnTiles) {
	_numberOfFields = numberOfFields;
	setTouchEnabled(false);
	setTouchChildrenEnabled(false);
	_enableCullingOnTiles = enableCullingOnTiles;
	setSize(float(TILE_SIZE_X * _numberOfFields.x), float(TILE_SIZE_Y * _numberOfFields.y));
}

TileField::~TileField() {

}

void TileField::setData(const std::string& animalName) {
	_animalName = animalName;
	pugi::xml_node animalFarmParameters = Content::instance.getAnimalFarmParametersNode(animalName);
	std::string baseTile = animalFarmParameters.first_attribute().as_string();
	pugi::xml_node parameter = animalFarmParameters.child("no_sort").first_child();
	
	while (parameter) {
		int i = parameter.first_attribute().as_int();
		int j = parameter.first_attribute().next_attribute().as_int();
		addChild(createTileSprite(parameter.name(), Vector2(TILE_SIZE_X + 0.5f, TILE_SIZE_Y + 0.5f), Point(i, j), 1));

		parameter = parameter.next_sibling();
	}

	pugi::xml_node pathNode = animalFarmParameters.child("path");

	parameter = pathNode.first_child();
	const std::string& resAnim = pathNode.first_attribute().as_string();

	while (parameter) {
		int i = parameter.first_attribute().as_int();
		int j = parameter.first_attribute().next_attribute().as_int();
		addChild(createTileSprite("roadLeftRight", Vector2(TILE_SIZE_X + 0.5f, TILE_SIZE_Y + 0.5f), Point(i, j), 1));

		parameter = parameter.next_sibling();
	}

	if (baseTile != "none") {
		for (int i = 0; i < _numberOfFields.x; i++) {
			for (int j = _numberOfFields.y - 1; j >= 0; j--) {
				addChild(createTileSprite(baseTile, Vector2(TILE_SIZE_X + 0.5f, TILE_SIZE_Y + 0.5f), Point(i, j)));
			}
		}
	}
}

spSprite TileField::createTileSprite(const std::string& resourceName, Vector2 spriteSize, Point spritePosition, short priority) {
	spSprite sprite = new Sprite();
	sprite->setAnchor(0.0f, 0.0f);
	sprite->setResAnim(tilesResources.getResAnim(resourceName));
	sprite->setPosition(getCellPosition(spritePosition.x, spritePosition.y));
	setSpriteScaleBySize(sprite, spriteSize);
	sprite->setPriority(priority);
	if (_enableCullingOnTiles) {
		sprite->setCull(true);
	}
	return sprite;
}

spActor TileField::createTileActor(Vector2 spriteSize, Point spritePosition, short priority) {
	spActor sprite = new Actor();
	sprite->setAnchor(0.0f, 0.0f);
	sprite->setPosition(getCellPosition(spritePosition.x, spritePosition.y));
	setActorScaleBySize(sprite, spriteSize);
	sprite->setPriority(priority);
	if (_enableCullingOnTiles) {
		sprite->setCull(true);
	}
	return sprite;
}

VectorArray<int> TileField::getFarmParameters(std::string& animalName) {
	pugi::xml_node animalParameters = Content::instance.getAnimalFarmParametersNode(animalName);
	pugi::xml_attribute attribute = animalParameters.first_attribute();
	VectorArray<int> attributesArray;
	attributesArray._vector.resize(0);
	attributesArray._vector.reserve(5);

	while (!attribute.empty()) {
		attributesArray.push(attribute.as_int());
		attribute = attribute.next_attribute();
	}

	return attributesArray;
}

Point TileField::getCellIndex(Vector2 position) {
	return Point((int)position.x / TILE_SIZE_X, (int)position.y / TILE_SIZE_Y);
}

Vector2	TileField::getCellPosition(int i, int j) {
	return Vector2(float(i * TILE_SIZE_X), float(j * TILE_SIZE_Y));
}