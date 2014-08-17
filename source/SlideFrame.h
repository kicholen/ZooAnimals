#ifndef _SLIDEFRAME_
#define _SLIDEFRAME_

#include "Main.h"
#include "LandingPageFrame.h"
#include "Slider.h"
#include "SlidingActor.h"
#include "Box9Sprite.h"

DECLARE_SMART(SlideFrame, spSlideFrame);

class SlideFrame : public CustomFrame
{
public:
	class GoBackToPreviousFrameEvent: public Event
	{
	public:
		enum EV
		{
			GO_BACK = makefourcc('G', 'B', 'T', 'P'),
		};

		GoBackToPreviousFrameEvent(EV ev):Event(ev) {}
	};
	SlideFrame();

	Action loop();

	void setData();
protected:
	void selectTransitions();
	void _postHiding(Event *);
	void _preShowing(Event *);

private:
	void onSliderTouchDown(Event *event);
	void onSliderTouchUp(Event *event);
	void onSliderCompleted(Event *event);

	spSlider _slider;
	spSlidingActor _slidingActor;

};

#endif