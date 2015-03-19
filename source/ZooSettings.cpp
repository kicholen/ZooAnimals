#include "ZooSettings.h"
#include "StartGameConfig.h"

// TODO don't have better idea for filling blanks here

ZooSettings ZooSettings::instance;

ZooSettings::ZooSettings() {
	_path = "ZooSettings.xml";
}


ZooSettings::~ZooSettings() {
}

void ZooSettings::init(const string &version) {
	_version = version;
	load();

	if (_doc.child("check").attribute("version").value() != _version) {
		reset();
	}
}

void ZooSettings::reset() {
	_doc.reset();

	pugi::xml_node mainNode = _doc.append_child("animals");
	int regionArraySize = sizeof(START_REGIONS) / sizeof(START_REGIONS[0]);

	for (int i = 0; i < regionArraySize; i++) {
		pugi::xml_node regionNode = mainNode.append_child(START_REGIONS[i].c_str());
		if (START_REGIONS[i] == "farm") {
			int animalsArraySize = sizeof(START_FARM) / sizeof(START_FARM[0]);
			for (int j = 0; j < animalsArraySize; j++) {
				setAnimalByRegionNode(regionNode, START_FARM[j], 100, 100, START_ANIMALS_COUNT);
			}
		}
		else if (START_REGIONS[i] == "winter") {
			int animalsArraySize = sizeof(START_WINTER) / sizeof(START_WINTER[0]);
			for (int j = 0; j < animalsArraySize; j++) {
				setAnimalByRegionNode(regionNode, START_WINTER[j], 100, 100, START_ANIMALS_COUNT);
			}
		}
		else if (START_REGIONS[i] == "underwater") {
			int animalsArraySize = sizeof(START_UNDERWATER) / sizeof(START_UNDERWATER[0]);
			for (int j = 0; j < animalsArraySize; j++) {
				setAnimalByRegionNode(regionNode, START_UNDERWATER[j], 100, 100, START_ANIMALS_COUNT);
			}
		}
		else if (START_REGIONS[i] == "steppe") {
			int animalsArraySize = sizeof(START_STEPPE) / sizeof(START_STEPPE[0]);
			for (int j = 0; j < animalsArraySize; j++) {
				setAnimalByRegionNode(regionNode, START_STEPPE[j], 100, 100, START_ANIMALS_COUNT);
			}
		}
		else if (START_REGIONS[i] == "asia") {
			int animalsArraySize = sizeof(START_ASIA) / sizeof(START_ASIA[0]);
			for (int j = 0; j < animalsArraySize; j++) {
				setAnimalByRegionNode(regionNode, START_ASIA[j], 100, 100, START_ANIMALS_COUNT);
			}
		}
		else if (START_REGIONS[i] == "australia") {
			int animalsArraySize = sizeof(START_AUSTRALIA) / sizeof(START_AUSTRALIA[0]);
			for (int j = 0; j < animalsArraySize; j++) {
				setAnimalByRegionNode(regionNode, START_AUSTRALIA[j], 100, 100, START_ANIMALS_COUNT);
			}
		}
	}

	pugi::xml_node checkNode = _doc.append_child("check");
	checkNode.append_attribute("version").set_value(_version.c_str());
}

pugi::xml_node ZooSettings::getAnimal(const string& regionName, const string& animalName) {
	pugi::xml_node node = _doc.child("animals").child(regionName.c_str());
	if (!node) {
		return node;
	}
	node = node.child(animalName.c_str());

	return node;
}

void ZooSettings::setAnimal(const string& regionName, const string& animalName, int happiness, int hunger, int count) {
	pugi::xml_node mainNode = _doc.child("animals");
	pugi::xml_node regionNode = mainNode.child(regionName.c_str());
	if (!regionNode) {
		regionNode = mainNode.append_child(regionName.c_str());
	}
	
	setAnimalByRegionNode(regionNode, animalName, happiness, hunger, count);
}

pugi::xml_attribute ZooSettings::addValue(const string &name) {
	pugi::xml_node root = _doc.child("options");
	if (!root)
		root = _doc.append_child("options");
	pugi::xml_node child = root.child(name.c_str());
	if (!child)
		child = root.append_child(name.c_str());

	pugi::xml_attribute attr = child.attribute("value");
	if (!attr)
		attr = child.append_attribute("value");
	else
		attr = pugi::xml_attribute();

	return attr;
}

pugi::xml_attribute ZooSettings::getValue(const string &name) {
	pugi::xml_node root = _doc.child("options");
	OX_ASSERT(root);
	pugi::xml_node child = root.child(name.c_str());
	OX_ASSERT(child);
	pugi::xml_attribute attr = child.attribute("value");
	OX_ASSERT(attr);

	return attr;
}

pugi::xml_node ZooSettings::setAnimalByRegionNode(pugi::xml_node regionNode, const string& name, int happiness, int hunger, int count) {
	pugi::xml_node animalNode = regionNode.child(name.c_str());
	if (!animalNode) {
		animalNode = regionNode.append_child(name.c_str());
		animalNode.append_attribute("h"); // happiness
		animalNode.append_attribute("g"); // hunger
		animalNode.append_attribute("c"); // count
	}

	animalNode.attribute("h").set_value(happiness);
	animalNode.attribute("g").set_value(hunger);
	animalNode.attribute("c").set_value(count);

	return animalNode;
}