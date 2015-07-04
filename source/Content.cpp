#include "Content.h"
#include <vector>
#include "s3e.h"
#include "FlashUtils.h"

Content Content::instance;

Content::Content(): _path("content.xml") {

}

Content::~Content() {
	_contentDocument.reset();
}

void Content::setPath(const std::string &path) {
	_path = path;
}

void Content::init(const std::string &version) {
	_version = version;
	load();
	s3eDebugOutputString("settings::init");

	if (_contentDocument.child("data").attribute("version").value() != _version) {
		oxygine::log::warning("Wrong version of content");
	}
}

void Content::load() {
	_contentDocument.reset();
	file::buffer fb;
	file::read(_path.c_str(), fb);
	if (fb.getSize()) {
		_contentDocument.load_buffer(fb.getData(), fb.getSize());
	}
}

int Content::getParameterAsInt(const std::string &name) {
	return getParameterValue(name).as_int();
}

bool Content::getParameterAsBool(const std::string &name) {
	return getParameterValue(name).as_bool();
}

float Content::getParameterAsFloat(const std::string &name) {
	return getParameterValue(name).as_float();
}

std::string Content::getParameterAsString(const std::string &name) {
	return getParameterValue(name).as_string();
}

pugi::xml_attribute Content::getParameterValue(const std::string &name) {
	pugi::xml_node data = _contentDocument.child("data");
	OX_ASSERT(data);
	pugi::xml_node child = data.child("parameters");
	OX_ASSERT(child);
	pugi::xml_node parameter = child.child(name.c_str());
	OX_ASSERT(parameter);

	return parameter.attribute("value");
}

pugi::xml_node Content::getMatchTwoLevelNode(uint number) {
	pugi::xml_node data = _contentDocument.child("data");
	OX_ASSERT(data);
	pugi::xml_node games = data.child("games");
	OX_ASSERT(games);
	pugi::xml_node game = games.child("match_two");
	OX_ASSERT(game);
	pugi::xml_node gameLevel = game.first_child();

	uint countToZero = number;
	while (!gameLevel.empty()) {
		if (countToZero == 0) {
			return gameLevel;
		}
		gameLevel = gameLevel.next_sibling();
		countToZero--;
	}

	pugi::xml_node emptyNode;
	return emptyNode;
}

int Content::getGameLevelsCount(const std::string &gameName) {
	pugi::xml_node data = _contentDocument.child("data");
	OX_ASSERT(data);
	pugi::xml_node games = data.child("games");
	OX_ASSERT(games);
	pugi::xml_node game = games.child(gameName.c_str());
	OX_ASSERT(game);
	pugi::xml_node gameLevel = game.first_child();
	int counter = 0;
	while(!gameLevel.empty()) {
		counter++;
		gameLevel = gameLevel.next_sibling();
	}

	return counter;
}

pugi::xml_node Content::getAnimalJumpParametersNode(const std::string &name) {
	pugi::xml_node data = _contentDocument.child("data");
	OX_ASSERT(data);
	pugi::xml_node zoo = data.child("zoo");
	OX_ASSERT(zoo);
	pugi::xml_node animal = zoo.child(name.c_str());
	OX_ASSERT(animal);
	pugi::xml_node parameter = animal.first_child();
	const pugi::char_t* pugiName = "jump_parameters";

	while (!parameter.empty()) {
		if (strcmp(pugiName, parameter.name()) == 0) {
			return parameter;
		}

		parameter = parameter.next_sibling();
	}

	return pugi::xml_node();
}

pugi::xml_node Content::getAnimalGamePreferenceNode(const std::string &name) {
	pugi::xml_node data = _contentDocument.child("data");
	OX_ASSERT(data);
	pugi::xml_node zoo = data.child("zoo");
	OX_ASSERT(zoo);
	pugi::xml_node animal = zoo.child(name.c_str());
	OX_ASSERT(animal);
	pugi::xml_node parameter = animal.first_child();
	const pugi::char_t* pugiName = "game_preference";

	while (!parameter.empty()) {
		if (strcmp(pugiName, parameter.name()) == 0) {
			return parameter;
		}

		parameter = parameter.next_sibling();
	}

	return pugi::xml_node();
}

pugi::xml_node Content::getAnimalInfoNode(const std::string &name) {
	pugi::xml_node data = _contentDocument.child("data");
	OX_ASSERT(data);
	pugi::xml_node zoo = data.child("zoo");
	OX_ASSERT(zoo);
	pugi::xml_node animal = zoo.child(name.c_str());
	OX_ASSERT(animal);
	pugi::xml_node parameter = animal.first_child();
	const pugi::char_t* pugiName = "info";

	while (!parameter.empty()) {
		if (strcmp(pugiName, parameter.name()) == 0) {
			return parameter;
		}

		parameter = parameter.next_sibling();
	}

	return pugi::xml_node();
}

pugi::xml_node Content::getAnimalFarmParametersNode(const std::string &name) {
	pugi::xml_node data = _contentDocument.child("data");
	OX_ASSERT(data);
	pugi::xml_node zoo = data.child("zoo");
	OX_ASSERT(zoo);
	pugi::xml_node animal = zoo.child(name.c_str());
	OX_ASSERT(animal);
	pugi::xml_node parameter = animal.first_child();
	const pugi::char_t* pugiName = "farm";

	while (!parameter.empty()) {
		if (strcmp(pugiName, parameter.name()) == 0) {
			return parameter;
		}

		parameter = parameter.next_sibling();
	}

	return pugi::xml_node();
}

pugi::xml_node Content::getAnimalFarmSortParametersNode(const std::string &name) {
	return getAnimalFarmParametersNode(name).child("sort");
}

pugi::xml_node Content::getAnimalFarmNoSortParametersNode(const std::string &name) {
	return getAnimalFarmParametersNode(name).child("no_sort");
}

pugi::xml_node Content::getShopFirstChildNode(const std::string& shopType) {
	pugi::xml_node data = _contentDocument.child("data");
	OX_ASSERT(data);
	pugi::xml_node shop = data.child("shop");
	OX_ASSERT(shop);
	pugi::xml_node specificShop = shop.child(shopType.c_str());
	OX_ASSERT(specificShop);
	

	return specificShop.first_child();
}