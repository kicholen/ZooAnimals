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

		void init(const std::string &version);
		void reset();

		pugi::xml_attribute addValue(const std::string &name);
		pugi::xml_attribute getValue(const std::string &name);

	};

}

#endif