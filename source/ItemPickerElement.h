#ifndef _ITEMPICKERELEMENT_
#define _ITEMPICKERELEMENT_

#include "oxygine-framework.h"
#include "AnimatableElementContainer.h"
#include "FlashUtils.h"

using namespace oxygine;
using namespace FlashUtils;

#define CONTAINER_HEIGHT 0.7f

typedef enum {ipeAnimating, ipeNotSelected, ipeSelected} ItemPickerElementState;

DECLARE_SMART(ItemPickerElement, spItemPickerElement);


/*
**	Dispatches event every time player change his money, and element is picked.
**	Currently supports only 1 select, can be easily rebuild in future
*/
class ItemPickerElement : public Actor
{
public:
	class ItemPickerElementEvent : public Event
	{
	public:
		enum EV
		{
			ITEM_CHOOSEN = makefourcc('I', 'P', 'E', 'C'),
		};

		ItemPickerElementEvent(EV ev, std::string itemName_) :Event(ev), itemName(itemName_) {}

		std::string itemName;
	};

	ItemPickerElement(const Vector2& size);
	~ItemPickerElement();

	void setData(VectorArray<spActor>& children);
private:
	void createContainerIfDoesntExist();
	bool createSelectionIfDoesntExist();

	void onChildClicked(Event *ev);

	void drawSelectionOnChild(spSprite sprite);

private:
	spAnimatableElementContainer _animatableContainer;
	spBox9Sprite _selectionSprite;
	ItemPickerElementState _state;

};

#endif