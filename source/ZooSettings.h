#ifndef _ZOOSETTINGS_
#define _ZOOSETTINGS_

#include "FileSaver.h"

class ZooSettings : public FileSaver
{
public:
	
	static ZooSettings instance;

	ZooSettings();
	~ZooSettings();

	void init(const string& version);
	void reset();

	pugi::xml_attribute addValue(const string &name);
	pugi::xml_attribute getValue(const string &name);

	pugi::xml_node getAnimal(const string& regionName, const string& animalName);
	void setAnimal(const string& regionName, const string& name, int happiness, int hunger, int count, int lastFeedMS);

private:
	pugi::xml_node setAnimalByRegionNode(pugi::xml_node regionNode, const string& name, int happiness, int hunger, int count, int lastFeedMS);
};

#endif