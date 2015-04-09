#include "AnimalModel.h"
#include "Content.h"
#include "FlashUtils.h"

using namespace FlashUtils;

AnimalModel::AnimalModel(const string& animalName, int happiness, int hunger, int count, int lastFeedS) {
	_name = animalName;
	_happinessValue = happiness;
	_hungerValue = hunger;
	_count = count;
	_lastFeedS = lastFeedS;
}

AnimalModel::~AnimalModel() {

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
			_jumpDelay.x = (float)attribute.as_int();
		}
		else if (!strcmp(name, "delay_max")) {
			_jumpDelay.y = (float)attribute.as_int();
		}
		else if (!strcmp(name, "isWater")) {
			_isWaterAnimal = attribute.as_int() == 0 ? false : true;
		}
		attribute = attribute.next_attribute();
	}

	animalParameters = Content::instance.getAnimalGamePreferenceNode(_name);
	attribute = animalParameters.first_attribute();

	while (!attribute.empty()) {
		const char *name = attribute.name();

		if (!strcmp(name, "top")) {
			_topGame = attribute.as_string();
		}
		else if (!strcmp(name, "top_value")) {
			_topGameValue = attribute.as_int();
		}
		else if (!strcmp(name, "mid")) {
			_midGame = attribute.as_string();
		}
		if (!strcmp(name, "mid_value")) {
			_midGameValue = attribute.as_int();
		}
		else if (!strcmp(name, "low")) {
			_lowGame = attribute.as_string();
		}
		if (!strcmp(name, "low_value")) {
			_lowGameValue = attribute.as_int();
		}
		attribute = attribute.next_attribute();
	}
}