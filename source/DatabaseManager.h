#ifndef _DATABASEMANAGER_
#define _DATABASEMANAGER_

#include "oxygine-framework.h"

using namespace oxygine;

class DatabaseManager
{
public:
	static DatabaseManager instance;

	DatabaseManager();
	~DatabaseManager();
	void init();

	void save(Event *ev);
};

#endif