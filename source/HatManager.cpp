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

std::string HatManager::getRandomHatResource(spAnimalModel model) {
	return _hatList[CMath::random(0, _hatList.length())];
}

void HatManager::createHatList() {
	pugi::xml_node hatNode = Content::instance.getHatsNode();

	while (!hatNode.empty()) {
		pugi::xml_attribute attribute = hatNode.first_attribute();

		while (!attribute.empty()) {
			const char *name = attribute.name();

			if (!strcmp(name, "name")) {
				_hatList.push(attribute.as_string());
			}

			attribute = attribute.next_attribute();
		}

		hatNode = hatNode.next_sibling();
	}
}

void HatManager::getHatParametersForAnimal(const std::string hat, spAnimalModel model) {
	pugi::xml_node hatNode = Content::instance.getHatsNode().child(hat.c_str()).child(model->animalName().c_str()); // todo must optimize !!!
	pugi::xml_attribute attribute = hatNode.first_attribute();
	
	float scale = attribute.as_float();
	attribute = attribute.next_attribute();
	float offsetX = attribute.as_float();
	attribute = attribute.next_attribute();
	float offsetY = attribute.as_float();
}
