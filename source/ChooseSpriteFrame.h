#ifndef _CHOOSESPRITEFRAME_
#define _CHOOSESPRITEFRAME_

#include "Main.h"
#include "SlidingActor.h"
#include "Box9Sprite.h"
#include "CustomFrame.h"

DECLARE_SMART(ChooseSpriteFrame, spChooseSpriteFrame);

class ChooseSpriteFrame : public CustomFrame
{
public:
	class SpawnSpriteEvent: public Event
	{
	public:
		enum EV
		{
			SPAWN_SPRITE = makefourcc('S', 'S', 'F', 'E'),
		};

		SpawnSpriteEvent(EV ev):Event(ev) {}
	};
	ChooseSpriteFrame();

	Action loop();

	void setData();
protected:
	void selectTransitions();
	void _postHiding(Event *);
	void _preShowing(Event *);

	void onSpriteTouch(Event *event);

private:
	spSlidingActor _slidingActor;
};

#endif