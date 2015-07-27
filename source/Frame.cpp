#include "Frame.h"
#include "Blocking.h"
#include "Transition.h"

#ifdef __S3E__
#include "s3eKeyboard.h"
#include "s3eDevice.h"
#else
#include "SDL.h"
#endif

#define LOGD(...) log::messageln(__VA_ARGS__)
//#define LOGD(...) {}
namespace oxygine
{
	bool _processing = false;

	spTransition Transition::defaultTransition;
	spFrame Frame::currentFrame;
	spTransition Frame::currentTransition;
	spEventDispatcher Frame::_dispatcher;

	const char* Frame::ACTION_BACK = "_btn_back_";

	void Frame::initialize()
	{
		Transition::defaultTransition = new TransitionMove;		
		_dispatcher = new EventDispatcher;
	}

	void Frame::free()
	{
		Transition::defaultTransition = 0;
		Frame::currentFrame = 0;
		Frame::currentTransition = 0;
		_dispatcher = 0;
	}

	bool Frame::isInFrameLoop()
	{
		return _inframeloop;
	}

	
	spEventDispatcher Frame::getDispatcher()
	{		
		return _dispatcher;
	}


	Frame::Frame():_inframeloop(false)
	{
		_holder = new Actor;
		_holder->setSize(RootActor::instance->getSize());

		//_tIn = Transition::defaultTransition;
		//_tOut = Transition::defaultTransition;
	}

	Frame::~Frame()
	{
		_holder->detach();
	}

	spTransition Frame::getTransitionIn() 
	{
		return _tIn;
	}

	spTransition Frame::getTransitionOut() 
	{
		return _tOut;
	}

	void Frame::setTransitionIn(spTransition t)
	{
		_tIn = t;
	}

	void Frame::setTransitionOut(spTransition t)
	{
		_tOut = t;
	}

	void Frame::generateActionEvent(Event *event)
	{
		const std::string &name = event->target->getName();
		generateAction(name);
	}

	bool Frame::generateAction(const std::string &action, void *cdata, spObject data)
	{
		OX_ASSERT(!action.empty());
		_action.id = action;
		_action.cdata = cdata;
		_action.data = data;

		return true;
	}

	void Frame::handleClick(spActor actor)
	{
		actor->addEventListener(TouchEvent::CLICK, CLOSURE(this, &Frame::generateActionEvent));
	}

	void Frame::preShowing()
	{
		if (!_inframeloop)
		{
			LOGD("frame.entering%s", getName().c_str());
			Event ev(EVENT_ENTERING);
			dispatchEvent(&ev);
		}
		LOGD("frame.preShowing %s", getName().c_str());
		Event ev(EVENT_PRESHOWING);
		dispatchEvent(&ev);
	}

	void Frame::postShowing()
	{
		LOGD("frame.postShowing %s", getName().c_str());
		Event ev(EVENT_POSTSHOWING);
		dispatchEvent(&ev);
	}

	void Frame::preHiding()
	{
		LOGD("frame.preHiding %s", getName().c_str());
		Event ev(EVENT_PREHIDING);
		dispatchEvent(&ev);
	}

	void Frame::postHiding()
	{
		LOGD("frame.postHiding %s", getName().c_str());
		Event ev(EVENT_POSTHIDING);
		dispatchEvent(&ev);

		if (!_inframeloop)
		{
			LOGD("frame.leaving %s", getName().c_str());
			Event ev(EVENT_LEAVING);
			dispatchEvent(&ev);
		}
	}
	

	class AutoSwitch
	{
	public:
		AutoSwitch(bool &b):_b(b){}
		~AutoSwitch()
		{
			_b = !_b;
		}

		bool &_b;
	};
	
	int Frame::yield()
	{
		if (!_processing && isModalUpdateAllowed())
		{
			_processing = true;
			AutoSwitch q(_processing);
			Event ev(EVENT_MODALUPDATE);
			_dispatcher->dispatchEvent(&ev);
		}

		blocking::yield();
		return 0;
	}

	Action Frame::loop()
	{
		return waitAction();
	}

	Action Frame::frameloop()
	{
		_inframeloop = true;
		AutoSwitch q(_inframeloop);
		Action a = loop();
		return a;
	}


	bool Frame::checkQuit()
	{
		bool back = false;
#ifdef __S3E__
		if (s3eDeviceCheckQuitRequest()|| 
			(s3eKeyboardGetState(s3eKeyBackspace) & S3E_KEY_STATE_DOWN) || 
			(s3eKeyboardGetState(s3eKeyAbsBSK) & S3E_KEY_STATE_DOWN)) 
		{
			back = true;
		}
#else
		const Uint8* data  = SDL_GetKeyboardState(0);
		
		if (data[SDL_GetScancodeFromKey(SDLK_BACKSPACE)] || 
			data[SDL_GetScancodeFromKey(SDLK_ESCAPE)] ||
			data[SDL_GetScancodeFromKey(SDLK_AC_BACK)])
		{
			back = true;
		}

#endif
		return back;
	}

	Action Frame::waitAction()
	{
		_action = Action();
		while (!_action)
		{
			int v = yield();
			if (v == 1)
				throw Exit();

			bool back = checkQuit();

			if (back)
			{
				_action = Action();
				_action.id = ACTION_BACK;
				_action.key = true;
				break;
			}
		}

		_lastAction = _action;

		_action = Action();

		return _lastAction;
	}

	Action showFrame(spFrame frame, spTransition tIn)
	{
		log::messageln("showFrame %s", frame->getName().c_str());
		if (!tIn)
			tIn = frame->getTransitionIn();
		if (!tIn)
			tIn = Transition::defaultTransition;

		tIn->start(Frame::currentFrame, frame, true, false);
		Frame::currentTransition = tIn;
		Frame::currentFrame = frame;		
		return frame->frameloop();
	}

	Action transitionShowFrameAsDialog(spFrame next, spTransition tIn, spTransition tOut)
	{
		log::messageln("transitionShowFrameAsDialog %s", next->getName().c_str());
		if (!tIn)
			tIn = next->getTransitionIn();
		
		//if (!tOut)
		//	tOut = next->getTransitionOut();

		if (!tIn)
			tIn = Transition::defaultTransition;
		if (!tOut)
			tOut = Transition::defaultTransition;

		spFrame prev = Frame::currentFrame;
		spTransition prevT = Frame::currentTransition;
		Frame::currentFrame = next;
		tIn->start(prev, next, true, false);
		Frame::currentTransition = tIn;
		Action a = next->frameloop();

		tOut = next->getTransitionOut();

		tOut->start(next, prev, true, true);
		Frame::currentTransition = prevT;
		Frame::currentFrame = prev;
		return a;
	}

	Action transitionShowFrame(spFrame next, spTransition tIn, spTransition tOut)
	{
		log::messageln("transitionShowFrame %s", next->getName().c_str());
		spFrame prev = Frame::currentFrame;
		Action a = transitionSwitchFrame(next, tIn);		
		transitionRestoreFrame(prev, tOut);
		return a;
	}

	Action transitionSwitchFrame(spFrame next, spTransition tIn)
	{
		log::messageln("transitionSwitchFrame %s", next->getName().c_str());
		spFrame prev = Frame::currentFrame;
		Frame::currentFrame = next;

		if (!tIn)
			tIn = next->getTransitionIn();
		if (!tIn)
			tIn = Transition::defaultTransition;
		tIn->start(prev, next, false, false);
		Frame::currentTransition = tIn;
		prev = 0;//dont hold
		Action a = next->frameloop();
		return a;
	}

	void transitionRestoreFrame(spFrame next, spTransition tOut)
	{
		log::messageln("transitionRestoreFrame %s", next->getName().c_str());
		spFrame prev = Frame::currentFrame;				
		spTransition prevT = Frame::currentTransition;

		if (!tOut)
			tOut = prev->getTransitionOut();
		if (!tOut)
			tOut = Transition::defaultTransition;
		tOut->start(prev, next, false, true);
		Frame::currentTransition = prevT;
		Frame::currentFrame = next;
	}
}