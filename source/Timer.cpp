#include "Timer.h"

Timer::Timer(int delayMS, int repeatCount, bool detachUponCompletion, bool shouldAttachToRoot) {
	if (shouldAttachToRoot) {
		attachTo(getRoot());
	}
	setVisible(false);
	_paused = false;
	_currentCount = 1;
	_delay = delayMS;
	_repeatCount = repeatCount;
	_detachUponCompletion = detachUponCompletion;
	_timePassed = 0;
}

Timer::~Timer() {

}

void Timer::setCompleteCallback(EventCallback cb) {
	_cbComplete = cb;
}

void Timer::setTimerCallback(EventCallback cb) {
	_cbTimer = cb;
}

bool Timer::isRunning() {
	return !_paused;
}

void Timer::reset() {
	_currentCount = 0;
	_timePassed = 0;
}

void Timer::start() {
	setCallbackDoUpdate(CLOSURE(this, &Timer::updateTimer));
}

void Timer::pause() {
	_paused = true;
}

void Timer::resume() {
	_paused = false;
}

void Timer::updateTimer(const UpdateState &us) {
	if (!_paused) {
		_timePassed += us.dt;

		if (_timePassed >= _delay) {
			_timePassed = 0;
			++_currentCount;
			interval();
		}

		if (_currentCount >= _repeatCount && _repeatCount != -1) {
			complete();
		}
	}
}

void Timer::complete() {
	if (_cbComplete) {
		TimerEvent ev(TimerEvent::TIMER_COMPLETE, this);
		_cbComplete(&ev);
	}
	if (_detachUponCompletion) {
		detach();
	}
}

void Timer::interval() {
	if (_cbTimer) {
		TimerEvent ev(TimerEvent::TIMER, this);
		_cbTimer(&ev);
	}
}