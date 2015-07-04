#ifndef DISCOVER_IMAGE_ELEMENT
#define DISCOVER_IMAGE_ELEMENT

#include "oxygine-framework.h"

using namespace oxygine;

#define ELEMENT_DISAPPEAR_ANIMATION_TIME 10000

DECLARE_SMART(DiscoverImageElement , spDiscoverImageElement);
class DiscoverImageElement : public Actor
{
public:
	DiscoverImageElement(Vector2 size);
	~DiscoverImageElement();

	void reset(const std::string& assetName, int numberOfElements);
	void discoverNextElement();
	bool isImageDiscovered();
private:
	void animateNextElementDisappear();

	void setData(const std::string& assetName);
	spProgressBar createProgressBar();
	
	void setNumberOfElements(int value) { _numberOfElements = value; }
	int getNumberOfElements() { return _numberOfElements; }

	void setNumberOfElementsLeft(int value) { _numberOfElementsLeft = value; }
	int getNumberOfElementsLeft() { return _numberOfElementsLeft; }
	
private:
	int _numberOfElements;
	int _numberOfElementsLeft;
	spProgressBar _progressImage;
	spSprite _image;
};

#endif
