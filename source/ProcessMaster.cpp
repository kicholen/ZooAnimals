#include "ProcessMaster.h"

ProcessMaster::ProcessMaster() : Timer(PROCESS_DELAY, -1, true, false) {
	
}

ProcessMaster::~ProcessMaster() {
	_slaves.clear();
}

void ProcessMaster::addProcess(spProcessSlave slave) {
	_slaves.push(slave);
}

void ProcessMaster::setCompleteCallback(EventCallback cb) {
	_cbComplete = cb;
}

void ProcessMaster::clear() {
	_slaves.clear();
}

void ProcessMaster::start(spActor parent) {
	Timer::start();
	attachTo(parent);
}

void ProcessMaster::removeLastProcess() {
	spProcessSlave slave = _slaves._vector.back();
	_slaves._vector.pop_back();
}

void ProcessMaster::updateTimer(const UpdateState &us) {
	_timePassed += us.dt;

	if (_timePassed >= _delay) {
		_timePassed = 0;

		if (_slaves.length() == 0) {
			complete();
		}
		else {
			spProcessSlave slave = _slaves[_slaves.length() - 1];
			if (slave->completed()) {
				removeLastProcess();
			}
			else if (slave->canProcess()) {
				slave->process();
			}
		}
	}
}

void ProcessMaster::complete() {
	if (_cbComplete) {
		Event ev(Event::COMPLETE, this);
		_cbComplete(&ev);
	}

	if (_detachUponCompletion) {
		detach();
	}
}