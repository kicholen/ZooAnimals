#ifndef _SWIPEACTOR_
#define _SWIPEACTOR_

#include "oxygine_include.h"
#include "Actor.h"
#include "math/vector2.h"

namespace oxygine
{
	DECLARE_SMART(SwipeActor, spSwipeActor);

	class SwipeActor : public Actor
	{
	public:
		class SwipeEvent : public Event
		{
		public:
			enum EV
			{
				LEFT = makefourcc('S', 'A', 'L', 'E'),
				RIGHT = makefourcc('S', 'A', 'R', 'I'),
				UP = makefourcc('S', 'A', 'D', 'O'),
				DOWN = makefourcc('S', 'A', 'U', 'P')
			};

			SwipeEvent(EV ev) :Event(ev) {}
		};

		SwipeActor(float minSwipeSpeed = 5.0f, float maxFlingDelay = 0.15f);
		~SwipeActor();

	protected:
		void handleEvent(Event *event);
		void fling(Vector2 velocity);

		void onTouchEvent(Event *event);

		Vector2 calculateVelocity();

	private:
		timeMS _downTime;
		Vector2 _downPosition;

		timeMS _upTime;
		Vector2 _upPosition;

		int _currentIndex;
		long _maxFlingDelay;
		float _minSwipeSpeed;
	};
}


#endif