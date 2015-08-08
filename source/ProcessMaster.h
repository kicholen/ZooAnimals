#ifndef _PROCESSMASTER_
#define _PROCESSMASTER_

#include "FlashUtils.h"
#include "ProcessSlave.h"
#include "Timer.h"

#define PROCESS_DELAY 500

using namespace FlashUtils;

DECLARE_SMART(ProcessMaster, spProcessMaster);

class ProcessMaster : public Timer
{
public:
	ProcessMaster();
	~ProcessMaster();

	void addProcess(spProcessSlave slave);
	void start(spActor parent);
	void clear();

protected:
	virtual void updateTimer(const UpdateState &us);
	virtual void complete();

	void removeLastProcess();

	void updater(Event* event);
private:
	spTimer _timer;
	VectorArray<spProcessSlave> _slaves;
};

#endif