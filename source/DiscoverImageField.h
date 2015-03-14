#ifndef DISCOVER_IMAGE_FIELD
#define DISCOVER_IMAGE_FIELD

#include "oxygine-framework.h"
#include "DiscoverImageElement.h"

using namespace oxygine;

DECLARE_SMART(DiscoverImageField , spDiscoverImageField);

class DiscoverImageField : public Actor
{
public:
	class DiscoverImageFieldEvent: public Event {
	public:
		enum EV {
			FINISHED = makefourcc('D', 'I', 'F', 'F')
		};

		DiscoverImageFieldEvent(EV ev):Event(ev) {}
	};

	DiscoverImageField(Vector2 size, const string& assetName, int numberOfElements = 4);
	~DiscoverImageField();

	void reset(const string& assetName, int numberOfElements);
	void discoverNextElement();
private:
	bool isImageDiscovered();
	void animateNextElementDisappear();

	void fillField(const string& assetName);
private:
	spDiscoverImageElement _imageElement;
};

#endif