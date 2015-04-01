#include "DiscoverImageElement.h"
#include "SharedResources.h"

DiscoverImageElement::DiscoverImageElement(Vector2 size) {
	setSize(size);
}

DiscoverImageElement::~DiscoverImageElement() {

}

void DiscoverImageElement::reset(const string& assetName, int numberOfElements) {
	setNumberOfElementsLeft(numberOfElements);
	setNumberOfElements(numberOfElements);
	setData(assetName);
}

void DiscoverImageElement::discoverNextElement() {
	if (isImageDiscovered()) {
		return;
	}

	setNumberOfElementsLeft(getNumberOfElementsLeft() - 1);

	animateNextElementDisappear();
}

bool DiscoverImageElement::isImageDiscovered() {
	return getNumberOfElementsLeft() == 0;
}

void DiscoverImageElement::setData(const string& assetName) {
	if (!_progressImage) {
		_progressImage = createProgressBar();
		_progressImage->setAnchor(0.5f, 0.5f);
		_progressImage->setPosition(getSize() / 2);
		_progressImage->setPriority(1);
		_progressImage->setColor(Color(0x000000FF));
		addChild(_progressImage);
	}
	_progressImage->setResAnim(animalsResources.getResAnim(assetName));
	setActorScaleBySize(safeSpCast<Actor>(_progressImage), getSize());

	if (!_image) {
		_image = new Sprite();
		_image->setAnchor(0.5f, 0.5f);
		_image->setPosition(getSize() / 2);
		_image->setPriority(-1);
		addChild(_image);
	}
	_image->setResAnim(animalsResources.getResAnim(assetName));
	setActorScaleBySize(safeSpCast<Actor>(_image), getSize());
}

spProgressBar DiscoverImageElement::createProgressBar() {
	spProgressBar bar = new ProgressBar();
	bar->setDirection(bar->dir_radial_cw);
	return bar;
}

void DiscoverImageElement::animateNextElementDisappear() {
	float progress = float(getNumberOfElementsLeft()) / float(getNumberOfElements());
	_progressImage->addTween(ProgressBar::TweenProgress(progress), ELEMENT_DISAPPEAR_ANIMATION_TIME);
}