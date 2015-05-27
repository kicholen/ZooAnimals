#ifndef _ANIMALCARDELEMENT_
#define _ANIMALCARDELEMENT_

#include "oxygine-framework.h"
#include "AnimalModel.h"
#include "AnimalSlot.h"
#include "TouchBlock.h"

using namespace oxygine;

#define ANIMALSLOT_WIDTH_PERCENT 40.0f

DECLARE_SMART(AnimalCardElement, spAnimalCardElement);

typedef enum { acsZoomed, acsUnzoomed, acsAnimating } AnimalCardState;

class AnimalCardElement : public Actor
{
public:
	class AnimalCardEvent : public Event {
	public:
		enum EV {
			PHOTO_CLICKED = makefourcc('A', 'C', 'P', 'C')
		};

		AnimalCardEvent(EV ev) : Event(PHOTO_CLICKED) {}
	};

	AnimalCardElement(const Vector2& size, spAnimalModel model, ResAnim* thumbRes);
	~AnimalCardElement();

	void switchAnimalModel(spAnimalModel model);
private:
	void setData(spAnimalModel model);

	void setAnimalSlot(const string& spriteName);

	void createBackground();

	spTextField createText(int lockitId, const Vector2& boundries, bool multiline = false);

	void onPhotoClicked(Event *ev);
	void onZoomEnded(Event *ev);
	void onUnzoomEnded(Event *ev);
	void onResourceLoaded(Event *ev);
private:
	Vector2 _baseScale;
	Vector2 _basePosition;
	spSprite _photo;

	spAnimalSlot _animalSlot;
	spTouchBlock _touchBlocker;

	Resources _realAnimalResource;

	AnimalCardState _state;
	ResAnim* _thumbRes;
};

#endif