#ifndef _ZOOSETTINGS_
#define _ZOOSETTINGS_

#include "FileSaver.h"

class ZooSettings : public FileSaver
{
public:

	static ZooSettings instance;

	ZooSettings();
	~ZooSettings();

	void init(const std::string& version);
	void reset();
	void finishStartAnimalChoice();
	void save();

	pugi::xml_attribute addPlayerValue(const std::string &name);
	pugi::xml_attribute getPlayerValue(const std::string &name);

	pugi::xml_node getAnimal(const std::string& regionName, const std::string& animalName);
	void setAnimal(const std::string& regionName, const std::string& name, int happiness, int hunger, int count, int lastFeedS, int level, int lastCleanS);

	pugi::xml_node getHatsNode();
	pugi::xml_node getFreeHatsNode();
	pugi::xml_node getHatByAnimalName(const std::string& animalName);
	void setHat(const std::string& animalName, const std::string& hatName, int count);
	void setFreeHate(const std::string& hatName, int count);

	pugi::xml_node getAchievementsNode();
	void setAchievement(const std::vector<int> achievements);

	pugi::xml_node getMessagesNode();
	void clearMessagesNode();
	void setMessage(int type, int lockitTitle, int lockitDesc, const std::string& resourceName, int dateMS, const std::vector<std::string>& rewards);

	bool shouldShowChooseAnimalPopup() { return _shouldShowChooseAnimalPopup; }

private:
	void appendCheckIfNeeded();
	pugi::xml_node setAnimalByRegionNode(pugi::xml_node regionNode, const std::string& name, int happiness, int hunger, int count, int lastFeedS, int level, int lastCleanS);

	bool _shouldShowChooseAnimalPopup;
};

#endif