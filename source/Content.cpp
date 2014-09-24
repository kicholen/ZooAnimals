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

void Content::setPath(const string &path) {
	_path = path;
}

void Content::init(const string &version) {
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

int Content::getParameterAsInt(const string &name) {
	return getParameterValue(name).as_int();
}

bool Content::getParameterAsBool(const string &name) {
	return getParameterValue(name).as_bool();
}

float Content::getParameterAsFloat(const string &name) {
	return getParameterValue(name).as_float();
}

string Content::getParameterAsString(const string &name) {
	return getParameterValue(name).as_string();
}

pugi::xml_attribute Content::getParameterValue(const string &name) {
	pugi::xml_node data = _contentDocument.child("data");
	OX_ASSERT(data);
	pugi::xml_node child = data.child("parameters");
	OX_ASSERT(child);
	pugi::xml_attribute attr = child.attribute("value");
	OX_ASSERT(attr);

	return attr;
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

int Content::getGameLevelsCount(const string &gameName) {
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