#include "ZooSettings.h"
#include "StartGameConfig.h"
#include "s3eTimer.h"
// TODO don't have better idea for filling blanks here

ZooSettings ZooSettings::instance;

ZooSettings::ZooSettings() {
	_path = "ZooSettings.xml";
	_shouldShowChooseAnimalPopup = false;
}

ZooSettings::~ZooSettings() {
}

void ZooSettings::init(const std::string &version) {
	_version = version;
	load();

	if (_doc.child("check").attribute("version").value() != _version) {
		reset();
	}
}

void ZooSettings::reset() {
	_doc.reset();
	int utcSeconds = (int)s3eTimerGetUTC() / 1000;
	int level = 0;

	addPlayerValue("money").set_value(START_MONEY);
	addPlayerValue("exp").set_value(START_EXP);

	pugi::xml_node mainNode = _doc.append_child("animals");

	_shouldShowChooseAnimalPopup = true;
	/*int regionArraySize = sizeof(START_REGIONS) / sizeof(START_REGIONS[0]);

	for (int i = 0; i < regionArraySize; i++) {
		pugi::xml_node regionNode = mainNode.append_child(START_REGIONS[i].c_str());
		if (START_REGIONS[i] == "farm") {
			int animalsArraySize = sizeof(START_FARM) / sizeof(START_FARM[0]);
			for (int j = 0; j < animalsArraySize; j++) {
				setAnimalByRegionNode(regionNode, START_FARM[j], 0, 0, START_ANIMALS_COUNT, utcSeconds, level++);
			}
		}
		else if (START_REGIONS[i] == "winter") {
			int animalsArraySize = sizeof(START_WINTER) / sizeof(START_WINTER[0]);
			for (int j = 0; j < animalsArraySize; j++) {
				setAnimalByRegionNode(regionNode, START_WINTER[j], 0, 0, START_ANIMALS_COUNT, utcSeconds, level++);
			}
		}
		else if (START_REGIONS[i] == "underwater") {
			int animalsArraySize = sizeof(START_UNDERWATER) / sizeof(START_UNDERWATER[0]);
			for (int j = 0; j < animalsArraySize; j++) {
				setAnimalByRegionNode(regionNode, START_UNDERWATER[j], 0, 0, START_ANIMALS_COUNT, utcSeconds, level++);
			}
		}
		else if (START_REGIONS[i] == "steppe") {
			int animalsArraySize = sizeof(START_STEPPE) / sizeof(START_STEPPE[0]);
			for (int j = 0; j < animalsArraySize; j++) {
				setAnimalByRegionNode(regionNode, START_STEPPE[j], 0, 0, START_ANIMALS_COUNT, utcSeconds, level++);
			}
		}
		else if (START_REGIONS[i] == "asia") {
			int animalsArraySize = sizeof(START_ASIA) / sizeof(START_ASIA[0]);
			for (int j = 0; j < animalsArraySize; j++) {
				setAnimalByRegionNode(regionNode, START_ASIA[j], 0, 0, START_ANIMALS_COUNT, utcSeconds, level++);
			}
		}
		else if (START_REGIONS[i] == "australia") {
			int animalsArraySize = sizeof(START_AUSTRALIA) / sizeof(START_AUSTRALIA[0]);
			for (int j = 0; j < animalsArraySize; j++) {
				setAnimalByRegionNode(regionNode, START_AUSTRALIA[j], 0, 0, START_ANIMALS_COUNT, utcSeconds, level++);
			}
		}
	}
	finishStartAnimalChoice();
	*/
}

void ZooSettings::finishStartAnimalChoice() {
	_shouldShowChooseAnimalPopup = false;
}

void ZooSettings::save() {
	appendCheckIfNeeded();
	FileSaver::save();
}

pugi::xml_node ZooSettings::getAnimal(const std::string& regionName, const std::string& animalName) {
	pugi::xml_node node = _doc.child("animals").child(regionName.c_str());
	if (!node) {
		return node;
	}
	node = node.child(animalName.c_str());

	return node;
}

void ZooSettings::setAnimal(const std::string& regionName, const std::string& animalName, int happiness, int hunger, int count, int lastFeedS, int level, int lastCleanS) {
	pugi::xml_node mainNode = _doc.child("animals");
	pugi::xml_node regionNode = mainNode.child(regionName.c_str());
	if (!regionNode) {
		regionNode = mainNode.append_child(regionName.c_str());
	}
	
	setAnimalByRegionNode(regionNode, animalName, happiness, hunger, count, lastFeedS, level, lastCleanS);
}

pugi::xml_node ZooSettings::getHatsNode() {
	pugi::xml_node hatsNode = _doc.child("hats");

	return hatsNode;
}

pugi::xml_node ZooSettings::getFreeHatsNode() {
	pugi::xml_node freeHatsNode = _doc.child("free_hats");

	return freeHatsNode;
}

pugi::xml_node ZooSettings::getHatByAnimalName(const std::string& animalName) {
	pugi::xml_node node = _doc.child("hats").child(animalName.c_str());
	if (!node) {
		return node;
	}

	node = node.child(animalName.c_str());

	return node;
}

void ZooSettings::setHat(const std::string& animalName, const std::string& hatName, int count) {
	pugi::xml_node hatsNode = _doc.child("hats");
	if (hatsNode.empty()) {
		hatsNode = _doc.append_child("hats");
	}
	pugi::xml_node animalNode = hatsNode.child(animalName.c_str());

	if (animalNode.empty()) {
		animalNode = hatsNode.append_child(animalName.c_str());
	}

	const pugi::char_t* pugiName = hatName.c_str();
	
	pugi::xml_attribute animalAttribute = animalNode.first_attribute();
	bool doesHatAttributeExists = false;

	while (!animalAttribute.empty()) {
		if (!strcmp(animalAttribute.name(), pugiName)) {
			animalAttribute.set_value(count);
			doesHatAttributeExists = true;
			break;
		}

		animalAttribute = animalAttribute.next_attribute();
	}

	if (!doesHatAttributeExists) {
		animalNode.append_attribute(pugiName).set_value(count);
	}
}

void ZooSettings::setFreeHate(const std::string& hatName, int count) {
	pugi::xml_node hatsNode = _doc.child("free_hats");
	if (hatsNode.empty()) {
		hatsNode = _doc.append_child("free_hats");
	}

	pugi::xml_node freeHatNode = hatsNode.child(hatName.c_str());

	if (freeHatNode.empty()) {
		freeHatNode = hatsNode.append_child(hatName.c_str());
	}

	pugi::xml_attribute hatAttribute = freeHatNode.first_attribute();

	if (hatAttribute.empty()) {
		freeHatNode.append_attribute("count").set_value(count);
	}
	else {
		hatAttribute.set_value(count);
	}
}

pugi::xml_node ZooSettings::getAchievementsNode() {
	return _doc.child("achievements");
}

void ZooSettings::setAchievement(const std::vector<int> achievements) {
	pugi::xml_node node = getAchievementsNode();

	if (!node.empty()) {
		_doc.remove_child(node);
	}
	node = _doc.append_child("achievements");

	for (uint i = 0; i < achievements.size(); i++) {
		node.append_attribute("p").set_value(achievements[i]);
	}
}

pugi::xml_node ZooSettings::getMessagesNode() {
	return _doc.child("messages");
}

void ZooSettings::clearMessagesNode() {
	pugi::xml_node mainNode = getMessagesNode();
	pugi::xml_node node = mainNode.first_child();
	
	while (!node.empty()) {
		mainNode.remove_child(node);

		node = mainNode.first_child();
	}
}

void ZooSettings::setMessage(int type, int lockitTitle, int lockitDesc, const std::string& resourceName, int dateMS, const std::vector<std::string>& rewards) {
	pugi::xml_node node = getMessagesNode();

	if (node.empty()) {
		node = _doc.append_child("messages");
	}

	pugi::xml_node messageNode = node.append_child("asd");

	messageNode.append_attribute("ty").set_value(type); // do not change order!!!
	messageNode.append_attribute("ti").set_value(lockitTitle);
	messageNode.append_attribute("de").set_value(lockitDesc);
	messageNode.append_attribute("re").set_value(resourceName.c_str());
	messageNode.append_attribute("da").set_value(dateMS);

	for (uint i = 0; i < rewards.size(); i++) {
		messageNode.append_attribute("re").set_value(rewards[i].c_str());
	}
}

pugi::xml_attribute ZooSettings::addPlayerValue(const std::string &name) {
	pugi::xml_node root = _doc.child("player");
	if (!root)
		root = _doc.append_child("player");
	pugi::xml_node child = root.child(name.c_str());
	if (!child)
		child = root.append_child(name.c_str());

	pugi::xml_attribute attr = child.attribute("value");
	if (!attr)
		attr = child.append_attribute("value");

	return attr;
}

pugi::xml_attribute ZooSettings::getPlayerValue(const std::string &name) {
	pugi::xml_node root = _doc.child("player");
	OX_ASSERT(root);
	pugi::xml_node child = root.child(name.c_str());
	OX_ASSERT(child);
	pugi::xml_attribute attr = child.attribute("value");
	OX_ASSERT(attr);

	return attr;
}

void ZooSettings::appendCheckIfNeeded() {
	if (!_doc.child("check")) {
		pugi::xml_node checkNode = _doc.append_child("check");
		checkNode.append_attribute("version").set_value(_version.c_str());
	}
}

/**
*	Reminder. If u add anything here after release, check for attribute.
**/
pugi::xml_node ZooSettings::setAnimalByRegionNode(pugi::xml_node regionNode, const std::string& name, int happiness, int hunger, int count, int lastFeedS, int level, int lastCleanS) {
	pugi::xml_node animalNode = regionNode.child(name.c_str());
	if (!animalNode) {
		animalNode = regionNode.append_child(name.c_str());
		animalNode.append_attribute("h"); // happiness
		animalNode.append_attribute("g"); // hunger
		animalNode.append_attribute("c"); // count
		animalNode.append_attribute("lf"); // last feed time
		animalNode.append_attribute("l"); // level
		animalNode.append_attribute("lc"); // last clean time
	}

	animalNode.attribute("h").set_value(happiness);
	animalNode.attribute("g").set_value(hunger);
	animalNode.attribute("c").set_value(count);
	animalNode.attribute("lf").set_value(lastFeedS);
	animalNode.attribute("l").set_value(level);
	animalNode.attribute("lc").set_value(lastCleanS);

	return animalNode;
}