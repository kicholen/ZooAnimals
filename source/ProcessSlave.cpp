#include "ProcessSlave.h"


ProcessSlave::ProcessSlave() {
	_completed = false;
}

ProcessSlave::~ProcessSlave() {

}

void ProcessSlave::process() {

}

bool ProcessSlave::completed() {
	return _completed;
}