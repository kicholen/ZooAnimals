#ifndef _TIMER_
#define _TIMER_

#include "oxygine-framework.h"

using namespace oxygine;

DECLARE_SMART(Timer, spTimer);

class Timer : public Actor
{
public:
	class TimerEvent : public Event
	{
	public:
		enum EV
		{
			TIMER = makefourcc('T', 'E', 'T', 'I'),
			TIMER_COMPLETE = makefourcc('T', 'E', 'T', 'C'),
		};

		TimerEvent(EV ev, Timer *timer_) : Event(ev), timer(timer_) {}

		Timer *timer;
		const UpdateState *us;
	};

	Timer(int delayMS, int repeatCount = -1, bool detachUponCompletion = true, bool shouldAttachToRoot = true);
	~Timer();

	bool isRunning();
	void reset();
	void start();
	void pause();
	void resume();
	void setCompleteCallback(EventCallback cb);
	void setTimerCallback(EventCallback cb);

protected:
	virtual void updateTimer(const UpdateState &us);
	virtual void complete();
	void interval();

protected:
	timeMS _timePassed;
	int _delay;
	bool _detachUponCompletion;

private:
	int _currentCount;
	int _repeatCount;
	bool _paused;

	EventCallback _cbComplete;
	EventCallback _cbTimer;
};

#endif