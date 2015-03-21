#include "RegionAnimalsContainer.h"
#include "AnimalsManager.h"
#include "FlashUtils.h"

using namespace FlashUtils;

RegionAnimalsContainer::RegionAnimalsContainer(const Vector2& size) {
	setSize(size);
	setTouchEnabled(false);
	_previousCount = -1;
}

RegionAnimalsContainer::~RegionAnimalsContainer() {

}

void RegionAnimalsContainer::setRegion(const string& regionName) {
	if (_regionName == regionName) {
		return;
	}
	_regionName = regionName;
	createContainerIfDoesntExist();

	int count = 0;
	for (auto const &iterator : AnimalsManager::instance.getAnimalRegionMap(regionName)) {
		string name = CMath::intToString(count);
		spAnimalSlot animalSlot = _stackContainer->getChildT<AnimalSlot>(name, oxygine::ep_ignore_error);
		if (!animalSlot) {
			animalSlot = new AnimalSlot(iterator.first);
			_stackContainer->addChild(animalSlot);
			animalSlot->setName(name);
		}
		else {
			animalSlot->switchAnimalSprite(iterator.first);
		}
		count++;
	}

	if (_previousCount != -1 && _previousCount > count - 1) {
		for (int i = count; i <= _previousCount; i++) {
			_stackContainer->removeChild(_stackContainer->getChild(CMath::intToString(i)));
		}
	}
	_previousCount = count - 1;
}

void RegionAnimalsContainer::createContainerIfDoesntExist() {
	if (!_stackContainer) {
		_stackContainer = new StackContainer(getSize(), 1);
		_stackContainer->attachTo(this);
		_stackContainer->setTouchEnabled(false);
	}
}