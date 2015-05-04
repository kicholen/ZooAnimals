#ifndef _ROTATINGCONTAINER_
#define _ROTATINGCONTAINER_

#include "oxygine_include.h"
#include "Actor.h"
#include "math/vector2.h"
#include "Draggable.h"
#include "ClipRectActor.h"

namespace oxygine
{
	DECLARE_SMART(RotatingContainer, spRotatingContainer);
	class RotatingContainer : public Actor
	{
	public:
		static void setDefaultTouchThreshold(float val);

		class RotatingEvent: public Event
		{
		public:
			enum EV {
				BEGIN = makefourcc('R', 'C', 'B', 'G'),
				SLIDING = makefourcc('R', 'C', 'S', 'L'),
				END = makefourcc('R', 'C', 'E', 'D')
			};

			RotatingEvent(EV ev):Event(ev), speed(0, 0) {}
			Vector2 speed;
		};

		RotatingContainer(void);
		~RotatingContainer(void);

		spActor getContent() const { return _content; }
		const RectF& getDragBounds() const { return _drag.getDragBounds(); }

		/**max allowed radius of touch move when content could be clicked*/
		void setTouchThreshold(float rad);
		void setContent(spActor content);
		void setLocked(bool locked);
		void snap();

	protected:
		void destroy();

		void handleEvent(Event *event);

		void sizeChanged(const Vector2 &size);
		void doUpdate(const UpdateState &us);
		void updateDragBounds();
		

		void _newEvent(Event *event);

		bool _sliding;
		float _rad;
		float _maxSpeed;
		timeMS _downTime;

		Vector2 _downPos;
		Vector2 _speed;

		Draggable _drag;

		spActor _content;
		spClipRectActor _clip;

		spEventDispatcher _holded;

		struct  iter {
			Vector2 pos;
			timeMS tm;
		};

		timeMS _lastTime;
		enum { NUM = 11 };
		iter _prev[NUM];
		int _current;

		timeMS _lastIterTime;
	};

	/*class Element {
		public:
			spActor actor;
			float z;
			float positionDegrees;
			
			Element();
			~Element();
			//public function Element(child:DisplayObject, positionDegrees:Number) {
			//	this.child = child;
			//	this.positionDegrees = positionDegrees;
			//}
	};*/

}

#endif