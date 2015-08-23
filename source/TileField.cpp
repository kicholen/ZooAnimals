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

void TileField::setData(const std::string& animalName, const std::string& level) {
	_animalName = animalName;
	pugi::xml_node animalFarmParameters = Content::instance.getAnimalFarmParametersNode(animalName);
	std::string baseTile = animalFarmParameters.first_attribute().as_string();
	pugi::xml_node parameter = animalFarmParameters.child(level.c_str()).child("no_sort").first_child();
	Vector2 tileSize = Vector2(TILE_SIZE_X + 0.5f, TILE_SIZE_Y + 0.5f);
	Point position;

	while (parameter) {
		position.x = parameter.first_attribute().as_int();
		position.y = parameter.first_attribute().next_attribute().as_int();
		addChild(createTileSprite(parameter.name(), tileSize, position, 1));

		parameter = parameter.next_sibling();
	}

	if (baseTile != "none") {
		for (int i = 0; i < _numberOfFields.x; i++) {
			position.x = i;
			for (int j = _numberOfFields.y - 1; j >= 0; j--) {
				position.y = j;
				addChild(createTileSprite(baseTile, tileSize, position));
			}
		}
	}
}

spSprite TileField::createTileSprite(const std::string& resourceName, const Vector2& spriteSize, const Point& spritePosition, short priority) {
	spSprite sprite = new Sprite();
	sprite->setTouchEnabled(false);
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

spActor TileField::createTileActor(const Vector2& actorSize, const Point& actorPosition, short priority) {
	spActor actor = new Actor();
	actor->setTouchChildrenEnabled(false);
	actor->setTouchEnabled(false);
	actor->setSize(actorSize);
	actor->setAnchor(0.0f, 0.0f);
	actor->setPosition(getCellPosition(actorPosition.x, actorPosition.y));
	actor->setPriority(priority);
	if (_enableCullingOnTiles) {
		actor->setCull(true);
	}
	return actor;
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