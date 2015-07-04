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

		FileSaver();
		~FileSaver();

		void setPath(const std::string &path);
		void init(const std::string &version);
		void reset();
		void save();
		void load();

		pugi::xml_attribute addValue(const std::string &name);
		pugi::xml_attribute getValue(const std::string &name);

		pugi::xml_node getFirstNodeByName(const std::string &nodeName);

		pugi::xml_attribute addAttribute(pugi::xml_node child, const std::string &attributeName);
		pugi::xml_node addChild(pugi::xml_node parent, const std::string &childName);
	protected:
		std::string _version;
		std::string _path;
		pugi::xml_document _doc;
	};
}

#endif