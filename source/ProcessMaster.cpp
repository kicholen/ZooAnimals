#include "ProcessMaster.h"

ProcessMaster::ProcessMaster() {

}

ProcessMaster::~ProcessMaster() {
	_slaves.clear();
	if (_timer) {
		_timer->detach();
	}
}

void ProcessMaster::addProcess(spProcessSlave slave) {
	_slaves.push(slave);
}

void ProcessMaster::start() {
	if (!_timer) {
		_timer = new Timer(PROCESS_DELAY, -1, false);
		_timer->setTimerCallback(CLOSURE(this, &ProcessMaster::updater));
		_timer->start();
	}
}

void ProcessMaster::clear() {
	_slaves.clear();
}

void ProcessMaster::removeLastProcess() {
	spProcessSlave slave = _slaves._vector.back();
	_slaves._vector.pop_back();
}

void ProcessMaster::updater(Event* event) {
	if (_slaves.length() == 0) {
		// dispatch event, stop timer
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