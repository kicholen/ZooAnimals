#ifndef _PROCESSSLAVE_
#define _PROCESSSLAVE_

#include "oxygine-framework.h"

using namespace oxygine;

DECLARE_SMART(ProcessSlave, spProcessSlave);

class ProcessSlave : public Object
{
public:
	ProcessSlave();
	~ProcessSlave();

	virtual void process();
	virtual bool completed();
	virtual bool canProcess();
protected:
	bool _canProcess;
	bool _completed;
	int _part;
};

#endif