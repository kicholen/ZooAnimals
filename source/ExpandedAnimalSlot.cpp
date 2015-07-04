#include "ExpandedAnimalSlot.h"
#include "FlashUtils.h"

ExpandedAnimalSlot::ExpandedAnimalSlot(spAnimalModel model) {
	_model = model;
	setAnimalCount();
}

ExpandedAnimalSlot::ExpandedAnimalSlot(const std::string& id) {

}

ExpandedAnimalSlot::~ExpandedAnimalSlot() {

}

void ExpandedAnimalSlot::switchAnimalModel(spAnimalModel model) {
	_model = model;
	switchAnimalSprite(model->animalName());
	setAnimalCount();
}

void ExpandedAnimalSlot::setAnimalCount() {
	if (_animalCountElement) {
		_animalCountElement->detach();
	}
	_animalCountElement = new ConnectDotElement();
	_animalCountElement->attachTo(this);
	_animalCountElement->setPosition(getWidth() - _animalCountElement->getWidth() / 2, getHeight() + _animalCountElement->getHeight() / 2);
	_animalCountElement->addText(FlashUtils::CMath::intToString(_model->animalsCount()));
}
