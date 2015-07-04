#pragma once
#include "EventDispatcher.h"
#include "Actor.h"
#include "RootActor.h"
#include "Blocking.h"

namespace oxygine
{
	DECLARE_SMART(Transition, spTransition);
	class Action
	{
	public:
		Action():cdata(0), key(false){}

		std::string id;
		spObject data;
		void *cdata;
		bool key;

		operator bool() const {return !id.empty();}
	};

	class Exit
	{

	};

	DECLARE_SMART(Transition, spTransition);
	DECLARE_SMART(Frame, spFrame);
	class Frame: public EventDispatcher
	{
	public:
		enum 
		{
			EVENT_ENTERING = makefourcc('F','r','E','N'),
			EVENT_LEAVING = makefourcc('F','r','L','V'),
			EVENT_PRESHOWING = makefourcc('F','r','S','H'),
			EVENT_POSTSHOWING = makefourcc('F','o','S','H'),
			EVENT_PREHIDING = makefourcc('F','r','H','D'),
			EVENT_POSTHIDING = makefourcc('F','o','H','D'),
			EVENT_MODALUPDATE = makefourcc('F','M','D', 'U')
		};
		static const char* ACTION_BACK;

		static spFrame currentFrame;
		static spTransition currentTransition;

		static void initialize();
		static void free();
		static bool checkQuit();
		
		static spEventDispatcher getDispatcher();

		Frame();
		~Frame();

		spTransition	getTransitionIn();
		spTransition	getTransitionOut();
		spActor			getHolder(){return _holder;}
		bool			isInFrameLoop();

		void setTransitionIn(spTransition t);
		void setTransitionOut(spTransition t);

		void handleClick(spActor actor);

		void generateActionEvent(Event *event);
		bool generateAction(const std::string &action, void *cdata = 0, spObject data = 0);

		virtual Action waitAction();

		void preShowing();
		void postShowing();
		void preHiding();
		void postHiding();

		Action frameloop();		

	protected:
		virtual Action loop();

		int yield();
		virtual bool isModalUpdateAllowed() const {return true;}

		static spEventDispatcher _dispatcher;

		bool _inframeloop;
		

		spTransition _tIn;
		spTransition _tOut;
		spActor _holder;

		Action _action;
		Action _lastAction;
	};
	


	class singleModalUpdate: public Object
	{
	public: 
		void fire(spEventDispatcher dispatcher = 0)
		{
			if (!dispatcher)
				dispatcher = Frame::getDispatcher();
			dispatcher->addEventListener(Frame::EVENT_MODALUPDATE, CLOSURE(this, &singleModalUpdate::_event));
		}

	protected:
		virtual bool event(Event *ev)
		{
			return true;
		}

		void deleteMe(Event *ev)
		{
			ev->currentTarget->removeEventListeners(this);
			delete this;
		}

		void _event(Event *ev)
		{
			bool del = event(ev);
			if (del)
				deleteMe(ev);
		}
	};


	Action	transitionSwitchFrame(spFrame frame, spTransition tIn = 0);
	void	transitionRestoreFrame(spFrame frame, spTransition tOut = 0);
	Action	transitionShowFrameAsDialog(spFrame frame, spTransition tIn = 0, spTransition tOut = 0);
	Action	transitionShowFrame(spFrame frame, spTransition tIn = 0, spTransition tOut = 0);	
}
