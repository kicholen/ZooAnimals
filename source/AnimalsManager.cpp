#include "AnimalsManager.h"

AnimalsManager AnimalsManager::instance;

AnimalsManager::AnimalsManager() {

}


AnimalsManager::~AnimalsManager() {

}

void AnimalsManager::init() {
	string animalsInside[] = {"tiger", "koala", "goat", "girafee", "octopus", "penguin"};
	int sizeOfArray = sizeof(animalsInside) / sizeof( animalsInside[0]);

	for (int i = 0; i < sizeOfArray; i++) {
		spAnimalModel model = new AnimalModel(animalsInside[i]);
		model->fromContent();
		_animalsMap[animalsInside[i]] = model;
	}
}

spAnimalModel AnimalsManager::getAnimalModel(const string& name) {
	return _animalsMap[name];
}