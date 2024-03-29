#ifndef _HATMANAGER_
#define _HATMANAGER_

#include "oxygine-framework.h"
#include "FlashUtils.h"

using namespace FlashUtils;
using namespace oxygine;

typedef std::map<std::string, int > hatsMap;

class HatManager : public EventDispatcher
{
public:
	static HatManager instance;

	struct hatParams {
		int scale;
		int offsetX;
		int offsetY;
	};

	class HatEvent : public Event
	{
	public:
		enum EV
		{
			COUNT_CHANGED = makefourcc('H', 'M', 'C', 'C'),
			ATTACHED = makefourcc('H', 'M', 'E', 'A')
		};

		HatEvent(EV ev, int count_) : Event(ev), count(count_) {}
		HatEvent(EV ev, const std::string& animalName_, const std::string& hatName_) : Event(ev), animalName(animalName_), hatName(hatName_) {}

		int count;
		std::string animalName;
		std::string hatName;
	};

	HatManager();
	~HatManager();

	void init();
	void store();

	int getHatsCountPerAnimal(const std::string& animalName);
	void addWearableToAnimal(const std::string& animalName, const std::string& wearableName);
	void addWearableToFreeHats(const std::string& wearableName, int count);
	std::string getWearable(const std::string& animalName, int hatIndex);
	std::string getRandomHatResource(const std::string& spriteName);
	hatParams* getHatParametersForAnimal(const std::string& hat, const std::string& spriteName);

	const std::map<std::string, int >& getFreeHats();


	const int getFreeHatsCount() const {
		return _freeHatsCount;
	}
private:
	void createHatList();
	void parseSavedState();

	void removeHatFromFreeList(const std::string& hatName);
	void dispatchCountChanged();
	void dispatchHatAttached(const std::string& animalName, const std::string& wearableName);
private:
	VectorArray<std::string> _hatList;
	std::map<std::string, hatsMap > _hatsMap;

	std::map<std::string, int > _freeHats;
	int _freeHatsCount;
};

#endif