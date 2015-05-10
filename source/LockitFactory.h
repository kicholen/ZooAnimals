#ifndef _LOCKITFACTORY_
#define _LOCKITFACTORY_

#include "Lockit.h"

class LockitFactory
{
public:
	static Lockit* getInstance(const char* languageCode);

};

#endif