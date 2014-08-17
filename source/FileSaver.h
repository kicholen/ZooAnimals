#ifndef _FILESAVER_
#define _FILESAVER_
#include <string>
#include "core/STDFileSystem.h"
#include "pugixml/pugixml.hpp"
#include "core/file.h"
#include "FlashUtils.h"

namespace oxygine
{
	using namespace std;
	using namespace FlashUtils;

	class FileSaver
	{
	public:
		struct allow_remote_predicate {
			bool operator()(pugi::xml_attribute attr) const {
				return strcmp(attr.name(), "AllowRemote") == 0;
			}

			bool operator()(pugi::xml_node node) const {
				return node.attribute("AllowRemote").as_bool();
			}
		}; 

		static FileSaver instance;

		FileSaver();
		~FileSaver();

		void setPath(const string &path);
		void init(const string &version);
		void reset();
		void save();
		void load();

		pugi::xml_attribute addValue(const string &name);

		pugi::xml_node getFirstNodeByName(const string &nodeName);
		pugi::xml_attribute addAttribute(pugi::xml_node child, const string &attributeName);
		pugi::xml_node addChild(pugi::xml_node parent, const string &childName);
	private:
		string _version;
		string _path;
		pugi::xml_document _doc;
	};
}

#endif