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
	void init(const string &version);
	void reset();
	void load();

	int getParameterAsInt(const string &name);
	bool getParameterAsBool(const string &name);
	float getParameterAsFloat(const string &name);
	string getParameterAsString(const string &name);

	pugi::xml_attribute getParameterValue(const string &name);

	pugi::xml_node getMatchTwoLevelNode(uint number);

	pugi::xml_node getAnimalJumpParametersNode(const string &name);
	pugi::xml_node getAnimalGamePreferenceNode(const string &name);
	pugi::xml_node getAnimalFarmParametersNode(const string &name);
	pugi::xml_node getAnimalFarmSortParametersNode(const string &name);
	pugi::xml_node getAnimalFarmNoSortParametersNode(const string &name);
	pugi::xml_node getShopFirstChildNode(const string& shopType);
	
	int getGameLevelsCount(const string &gameName);
private:
	void setPath(const string &path);

private:
	string _version;
	string _path;
	pugi::xml_document _contentDocument;
};

#endif