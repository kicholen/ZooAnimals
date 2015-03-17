#ifndef _SETTINGS_
#define _SETTINGS_

#include "FileSaver.h"

namespace oxygine
{
	class Settings : public FileSaver
	{
	public:
		static Settings instance;

		Settings();
		~Settings();

		void init(const string &version);
		void reset();

		pugi::xml_attribute addValue(const string &name);
		pugi::xml_attribute getValue(const string &name);

	};

}

#endif