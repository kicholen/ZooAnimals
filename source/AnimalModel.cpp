#include "AnimalModel.h"
#include "Content.h"
#include "FlashUtils.h"

using namespace FlashUtils;

AnimalModel::AnimalModel(const string& animalName) {
	_name = animalName;
}

void AnimalModel::fromContent() {
	pugi::xml_node animalParameters = Content::instance.getAnimalJumpParametersNode(_name);
	pugi::xml_attribute attribute = animalParameters.first_attribute();

	while (!attribute.empty()) {
		const char *name = attribute.name();

		if (!strcmp(name, "range")) {
			_jumpRange = attribute.as_int();
		}
		else if (!strcmp(name, "height")) {
			_jumpHeight = attribute.as_int();
		}
		else if (!strcmp(name, "time")) {
			_jumpTime = attribute.as_int();
		}
		else if (!strcmp(name, "delay_min")) {
			_jumpDelay.x = attribute.as_int();
		}
		else if (!strcmp(name, "delay_max")) {
			_jumpDelay.y = attribute.as_int();
		}
		else if (!strcmp(name, "isWater")) {
			_isWaterAnimal = attribute.as_int() == 0 ? false : true;
		}
		attribute = attribute.next_attribute();
	}
}


AnimalModel::~AnimalModel() {

}
