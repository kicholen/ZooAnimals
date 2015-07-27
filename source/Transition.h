#pragma once
#include "core/Object.h"
#include "RootActor.h"
#include "Frame.h"
#include "ColorRectSprite.h"
#include "STDRenderer.h"

namespace oxygine
{
	DECLARE_SMART(Frame, spFrame);	
	DECLARE_SMART(Tween, spTween);
	DECLARE_SMART(Transition, spTransition);
	class Transition: public Object
	{
	public:
		static spTransition defaultTransition;

		Transition();
		~Transition();

		void start(spFrame prev, spFrame next, bool dialog, bool back);
		void end();

	protected:
		struct trans
		{
			spFrame prev;
			spFrame next;
			bool dialog;
			bool back;
		};
		
		virtual void _start(trans &t){}
		virtual void _end(){}
	};

	DECLARE_SMART(TransitionSimple, spTransitionSimple);
	class TransitionSimple: public Transition
	{
	public:
		TransitionSimple():_duration(500), _singleDirection(false){}

		timeMS getDuration() const {return _duration;}
		void setDuration(timeMS dur){_duration = dur;}

	protected:
		timeMS _duration;//transition time
		bool _singleDirection;

		void _start(trans &t);
		void middle(trans &t);
		void attach(trans &t);
		void detach(trans &t);

		virtual void blockingPhase(spFrame, bool){}
		virtual void setup(spFrame, bool){}
		virtual void reset(spFrame, bool){}
	};

	DECLARE_SMART(TransitionMove, spTransitionMove);
	class TransitionMove: public TransitionSimple
	{
	public:
		TransitionMove();
		void setSrc(const Vector2 &pos);
		void setDest(const Vector2 &pos);		

	protected:
		spColorRectSprite _fade;

		void setup(spFrame frame, bool back);		
		void blockingPhase(spFrame frame, bool back);
		void reset(spFrame frame, bool back);
		void _end();

		Vector2 _src;
		Vector2 _dest;
	};

	DECLARE_SMART(TransitionScale, spTransitionScale);
	class TransitionScale: public TransitionSimple
	{
	protected:
		void setup(spFrame frame, bool back);
		void blockingPhase(spFrame frame, bool back);
		void reset(spFrame frame, bool back);
	};

	DECLARE_SMART(TransitionFade, spTransitionFade);
	class TransitionFade: public TransitionSimple
	{
	protected:
		void setup(spFrame frame, bool back);
		void blockingPhase(spFrame frame, bool back);
		void reset(spFrame frame, bool back);
	};

	DECLARE_SMART(TransitionScaleRotate, spTransitionFadeRotate);
	class TransitionScaleRotate: public TransitionSimple
	{
	public:
		TransitionScaleRotate();		

	protected:
		void setup(spFrame frame, bool back);
		void blockingPhase(spFrame frame, bool back);
		void reset(spFrame frame, bool back);
	};

	DECLARE_SMART(TransitionQuads, spTransitionQuads);
	class TransitionQuads: public TransitionSimple
	{
	public:		
		TransitionQuads();
		~TransitionQuads();

	private:		
		void clicked(Event *);
		Vector2 _center;

		void setup(spFrame frame, bool back);
		void blockingPhase(spFrame frame, bool back);
		void reset(spFrame frame, bool back);
	};

	DECLARE_SMART(TransitionInstant, spTransitionInstant);
	class TransitionInstant : public TransitionSimple
	{
	protected:
		void setup(spFrame frame, bool back);
		void blockingPhase(spFrame frame, bool back);
		void reset(spFrame frame, bool back);
	};
}