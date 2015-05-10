#include "LockitFactory.h"

Lockit* LockitFactory::getInstance(const char* languageCode) {
	Lockit *lockit = new Lockit();
	string path = "lockit_";
	path += languageCode;
	path += ".xml";
	lockit->setPath(path);
	lockit->load();

	return lockit;
}

