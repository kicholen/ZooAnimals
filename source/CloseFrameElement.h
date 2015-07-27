#ifndef _CLOSEFRAMEELEMENT_
#define _CLOSEFRAMEELEMENT_

#include "oxygine-framework.h"
#include "TweenButton.h"

using namespace oxygine;

typedef enum { cfeAnimating, cfeHidden, cfeShown } CloseFrameElementState;
typedef enum { cfeLeft, cfeRght } CloseFrameElementDisplayState;

DECLARE_SMART(CloseFrameElement, spCloseFrameElement);

class CloseFrameElement : public Actor
{
public:
	class CloseFrameEvent : public Event {
	public:
		enum EV {
			CLOSE = makefourcc('C', 'C', 'F', 'E')
		};

		CloseFrameEvent(EV ev) :Event(ev) {}
	};
	CloseFrameElement();
	~CloseFrameElement();

	void show();
	void hide();

private:
	void setDisplayState();
	void create();

	void onAnimationFinished();
	void dispatchCloseEvent(Event *ev);

	Vector2 _showPosition;
	Vector2 _hidePosition;

	spTweenButton _button;
	CloseFrameElementState _state;
	CloseFrameElementDisplayState _displayState;
};

#endif