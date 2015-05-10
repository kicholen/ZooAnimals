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
	void init(int type);

	string getText(int lockitNumber);

	void setLanguage(int type);

	//void save(Event *ev);
private:
	string getLanguageCode(int type);

private:
	Lockit* _lockit;
};

#endif
