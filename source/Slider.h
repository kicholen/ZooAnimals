#ifndef _SLIDER_
#define _SLIDER_

#include "oxygine_include.h"
#include "SlidingActor.h"
#include "Actor.h"
#include "Button.h"

namespace oxygine
{
	DECLARE_SMART(Slider, spSlider);

	class Slider : public SlidingActor
	{
	public:
		class CompletedEvent: public Event
		{
		public:
			enum EV
			{
				COMPLETED = makefourcc('S', 'C', 'O', 'M'),
			};

			CompletedEvent(EV ev):Event(ev) {}
		};

		Slider(float buttonHeight);
		void reset();
		void startDrag();
		void addChildToClip(spActor actor);
	protected:
		void _newEvent(Event *event);
	private:
		float _buttonHeight;
	};
}
#endif