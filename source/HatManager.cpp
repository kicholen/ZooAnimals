#include "HatManager.h"
#include "Content.h"
#include "ZooSettings.h"

HatManager HatManager::instance;

HatManager::HatManager()
{
}

HatManager::~HatManager()
{
	_hatList._vector.clear();
	_hatsMap.clear();
}

void HatManager::init() {
	createHatList();
	parseSettings();
}

int HatManager::getHatsCountPerAnimal(const std::string& animalName) {
	if (_hatsMap.count(animalName) == 0) {
		return 0;
	}
	else {
		int value = 0;
		for (int i = 0; i < _hatList.length(); i++) {
			if (_hatsMap[animalName].count(_hatList[i]) > 0) {
				value += _hatsMap[animalName][_hatList[i]];
			}
		}
		return value;
	}
}

void HatManager::addWearableToAnimal(const std::string& animalName, const std::string& wearableName) {
	_hatsMap[animalName][wearableName] += 1;
}

std::string HatManager::getWearable(const std::string& animalName, int hatIndex) {
	int value = 0;
	for (int i = 0; i < _hatList.length(); i++) {
		if (_hatsMap[animalName].count(_hatList[i]) > 0) {
			value += _hatsMap[animalName][_hatList[i]];
		}
		if (hatIndex <= value) {
			return _hatList[i];
		}
	}

	OX_LOG_ERROR("HatManager::getWearable, oops");
	return "chef_hat";
}

std::string HatManager::getRandomHatResource(const std::string& spriteName) {
	return _hatList[CMath::random(0, _hatList.length())];
}

void HatManager::createHatList() {
	pugi::xml_node hatNode = Content::instance.getHatsNode().first_child();

	while (!hatNode.empty()) {
		_hatList.push(hatNode.name());
		hatNode = hatNode.next_sibling();
	}
}

HatManager::hatParams* HatManager::getHatParametersForAnimal(const std::string& hat, const std::string& spriteName) {
	pugi::xml_node hatNode = Content::instance.getHatsNode().child(hat.c_str());
	pugi::xml_node eeNode = hatNode.child(spriteName.c_str());
	pugi::xml_attribute attribute = eeNode.first_attribute();
	
	hatParams *params = new hatParams;
	params->scale = attribute.as_int();
	attribute = attribute.next_attribute();
	params->offsetY = attribute.as_int();
	attribute = attribute.next_attribute();
	params->offsetX = attribute.as_int();

	return params;
}

void HatManager::parseSettings() {
	pugi::xml_node hatsNode = ZooSettings::instance.getHatsNode();

	if (hatsNode) {
		pugi::xml_node animalNode = hatsNode.first_child();

		while (!animalNode.empty()) {
			pugi::xml_attribute hatAttribute = animalNode.first_attribute();
			while (!hatAttribute.empty()) {
				_hatsMap[animalNode.name()].insert(make_pair(hatAttribute.name(), hatAttribute.as_int()));
				hatAttribute = hatAttribute.next_attribute();
			}

			animalNode = animalNode.next_sibling();
		}
	}
}

void HatManager::store() {
	for (map<string, hatsMap >::iterator outerIterator = _hatsMap.begin(); outerIterator != _hatsMap.end(); ++outerIterator) {
		for (hatsMap::iterator innerIterator = outerIterator->second.begin(); innerIterator != outerIterator->second.end(); ++innerIterator) {
			ZooSettings::instance.setHat(outerIterator->first, innerIterator->first, innerIterator->second);
		}
	}
}