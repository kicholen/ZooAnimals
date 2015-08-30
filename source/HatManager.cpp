#include "HatManager.h"
#include "Content.h"
#include "ZooSettings.h"

HatManager HatManager::instance;

HatManager::HatManager()
{
	_freeHatsCount = 0;
	addRef();
}

HatManager::~HatManager()
{
	_hatList._vector.clear();
	_hatsMap.clear();
	_freeHats.clear();
}

void HatManager::init() {
	createHatList();
	parseSavedState();
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

const std::map<std::string, int >& HatManager::getFreeHats() {
	return _freeHats;
}

void HatManager::addWearableToAnimal(const std::string& animalName, const std::string& wearableName) {
	_hatsMap[animalName][wearableName] += 1;
	dispatchHatAttached(animalName, wearableName);
	removeHatFromFreeList(wearableName);
}

void HatManager::addWearableToFreeHats(const std::string& wearableName, int count) {
	if (_freeHats.count(wearableName) > 0) {
		_freeHats[wearableName] += count;
	}
	else {
		_freeHats[wearableName] = count;
	}
	_freeHatsCount += 1;
	dispatchCountChanged();
}


std::string HatManager::getWearable(const std::string& animalName, int hatIndex) {
	int value = -1;
	for (int i = 0; i < _hatList.length(); i++) {
		int prevValue = value;
		if (_hatsMap[animalName].count(_hatList[i]) > 0) {
			value += _hatsMap[animalName][_hatList[i]];
		}
		if (hatIndex <= value && hatIndex > prevValue) {
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

void HatManager::removeHatFromFreeList(const std::string& hatName) {
	if (_freeHats.count(hatName) > 0) {
		_freeHats[hatName] -= 1;
		_freeHatsCount -= 1;
		dispatchCountChanged();

		if (_freeHats[hatName] == 0) {
			_freeHats.erase(hatName);
		}
	}
}

void HatManager::dispatchCountChanged() {
	HatEvent ev(HatEvent::COUNT_CHANGED, _freeHatsCount);
	dispatchEvent(&ev);
}

void HatManager::dispatchHatAttached(const std::string& animalName, const std::string& wearableName) {
	HatEvent ev(HatEvent::ATTACHED, animalName, wearableName);
	dispatchEvent(&ev);
}

void HatManager::parseSavedState() {
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

	pugi::xml_node freeHatsNode = ZooSettings::instance.getFreeHatsNode();

	if (freeHatsNode) {
		pugi::xml_node freeHat = freeHatsNode.first_child();

		while (!freeHat.empty()) {
			pugi::xml_attribute hatAttribute = freeHat.first_attribute();
			if (!hatAttribute.empty()) {
				int freeHatCount = hatAttribute.as_int();
				_freeHats[freeHat.name()] = freeHatCount;
				_freeHatsCount += freeHatCount;
			}

			freeHat = freeHat.next_sibling();
		}
	}
}

void HatManager::store() {
	for (map<string, hatsMap >::iterator outerIterator = _hatsMap.begin(); outerIterator != _hatsMap.end(); ++outerIterator) {
		for (hatsMap::iterator innerIterator = outerIterator->second.begin(); innerIterator != outerIterator->second.end(); ++innerIterator) {
			ZooSettings::instance.setHat(outerIterator->first, innerIterator->first, innerIterator->second);
		}
	}

	for (map<string, int >::iterator innerIterator = _freeHats.begin(); innerIterator != _freeHats.end(); ++innerIterator) {
		ZooSettings::instance.setFreeHate(innerIterator->first, innerIterator->second);
	}
}