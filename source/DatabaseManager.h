#ifndef _DATABASEMANAGER_
#define _DATABASEMANAGER_

class DatabaseManager
{
public:
	static DatabaseManager instance;

	DatabaseManager();
	~DatabaseManager();

	void save();
};

#endif