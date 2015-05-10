#ifndef _LANGUAGEMANAGER_
#define _LANGUAGEMANAGER_

#include "oxygine-framework.h"
#include "Lockit.h"

using namespace oxygine;

typedef enum { lmEnglish, lmPolish } Language;

class LanguageManager
{
public:
	static LanguageManager instance;

	LanguageManager();
	~LanguageManager();
	void init(Language type);

	string getText(int lockitNumber);

	void setLanguage(Language type);

	//void save(Event *ev);
private:
	const char* getLanguageCode(Language type);

private:
	Lockit* _lockit;
};

#endif
