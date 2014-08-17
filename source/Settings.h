#ifndef _SETTINGS_
#define _SETTINGS_

#include <string>
#include "core/STDFileSystem.h"
#include "pugixml/pugixml.hpp"
#include "core/file.h"
#include "FlashUtils.h"

namespace oxygine
{
	using namespace std;

	class Settings
	{
	public:
		static Settings instance;

		Settings();
		~Settings();
		void setPath(const string &path);
		void init(const string &version);
		void reset();
		void load();
		void save();

		pugi::xml_attribute addValue(const string &name);
		pugi::xml_attribute getValue(const string &name);

	private:
		string _version;
		string _path;
		pugi::xml_document _doc;
	};

}

#endif