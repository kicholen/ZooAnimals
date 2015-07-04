#include "DiscoverImageField.h"
#include "SharedResources.h"

DiscoverImageField::DiscoverImageField(Vector2 size, const std::string& assetName, int numberOfElements) {
	setSize(size);
	fillField(assetName);
	reset(assetName, numberOfElements);
}

DiscoverImageField::~DiscoverImageField() {
}

void DiscoverImageField::reset(const std::string& assetName, int numberOfElements) {
	_imageElement->reset(assetName, numberOfElements);
}

void DiscoverImageField::fillField(const std::string& assetName) {
	if (!_imageElement) {
		_imageElement = new DiscoverImageElement(getSize());
	}
	addChild(_imageElement);
	_imageElement->setTouchChildrenEnabled(false);
	_imageElement->setTouchEnabled(false);

}

void DiscoverImageField::discoverNextElement() {
	_imageElement->discoverNextElement();
	
	if (_imageElement->isImageDiscovered()) {
		// make magic happen
	}
}