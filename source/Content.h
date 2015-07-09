#ifndef _CONTENT_
#define _CONTENT_

#include <string>
#include "core/STDFileSystem.h"
#include "pugixml/pugixml.hpp"
#include "core/file.h"
#include "FlashUtils.h"

class Content
{
public:
	static Content instance;

	Content();
	~Content();
	void init(const std::string &version);
	void reset();
	void load();

	int getParameterAsInt(const std::string &name);
	bool getParameterAsBool(const std::string &name);
	float getParameterAsFloat(const std::string &name);
	std::string getParameterAsString(const std::string &name);

	pugi::xml_attribute getParameterValue(const std::string &name);

	pugi::xml_node getMatchTwoLevelNode(uint number);
	pugi::xml_node getGameNode(const std::string &gameName);

	pugi::xml_node getAnimalJumpParametersNode(const std::string &name);
	pugi::xml_node getAnimalGamePreferenceNode(const std::string &name);
	pugi::xml_node getAnimalInfoNode(const std::string &name);

	pugi::xml_node getAnimalFarmParametersNode(const std::string &name);
	pugi::xml_node getAnimalFarmSortParametersNode(const std::string &name);
	pugi::xml_node getAnimalFarmNoSortParametersNode(const std::string &name);
	pugi::xml_node getShopFirstChildNode(const std::string& shopType);
	
	int getGameLevelsCount(const std::string &gameName);
private:
	void setPath(const std::string &path);

private:
	std::string _version;
	std::string _path;
	pugi::xml_document _contentDocument;
};

#endif