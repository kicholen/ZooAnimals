#include "ProcessSlave.h"


ProcessSlave::ProcessSlave() {
	_completed = false;
	_canProcess = false;
	_part = 0;
}

ProcessSlave::~ProcessSlave() {

}

void ProcessSlave::process() {

}

bool ProcessSlave::completed() {
	return _completed;
}

bool ProcessSlave::canProcess() {
	return _canProcess;
}