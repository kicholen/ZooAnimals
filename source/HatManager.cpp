#include "HatManager.h"
#include "Content.h"

HatManager HatManager::instance;

HatManager::HatManager()
{
}

HatManager::~HatManager()
{
	_hatList._vector.clear();
}

void HatManager::init() {
	createHatList();
}

bool HatManager::canPlaceHat() {
	return true;
}

std::string HatManager::getRandomHatResource(const std::string spriteName) {
	return _hatList[CMath::random(0, _hatList.length())];
}

void HatManager::createHatList() {
	pugi::xml_node hatNode = Content::instance.getHatsNode().first_child();

	while (!hatNode.empty()) {
		_hatList.push(hatNode.name());
		hatNode = hatNode.next_sibling();
	}
}

HatManager::hatParams* HatManager::getHatParametersForAnimal(const std::string hat, const std::string spriteName) {
	pugi::xml_node hatNode = Content::instance.getHatsNode().child(hat.c_str()); // todo must optimize !!!
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
