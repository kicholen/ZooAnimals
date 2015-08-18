#ifndef _ACHIEVEMENTMANAGER_
#define _ACHIEVEMENTMANAGER_

#include "oxygine-framework.h"
#include "FlashUtils.h"
#include "AchievementModel.h"

using namespace FlashUtils;
using namespace oxygine;

typedef enum { amFeed, amCollect, amPlay, amZoo, amProtect, amClean, amCollectFarm, amCollectSteppe, amCollectWinter, amCollectAsia, amCollectUnder, amCollectAustralia } AchievementType;

class AchievementManager : public EventDispatcher
{
public:
	static AchievementManager instance;

	class AchievementEvent : public Event
	{
	public:
		enum EV
		{
			ACHIEVEMENT_GAINED = makefourcc('A', 'M', 'A', 'G')
		};

		AchievementEvent(EV ev, spAchievementModel model_) : Event(ev), model(model_) {}

		spAchievementModel model;
	};

	AchievementManager();
	~AchievementManager();

	void init();
	void store();

	void increaseProgress(AchievementType type);
	void increaseSpeciesPossesed(const std::string& region);

	bool isCompleted(AchievementType type);

	const std::vector<spAchievementModel>& getAchievements() const;
private:
	void parseContent();
	void parseSavedState();
	AchievementType getAchievementTypeByRegion(const std::string& region);


	void dispatchAchievementGainedEvent(spAchievementModel model);

private:
	VectorArray<spAchievementModel> _achievementList;
	std::map <std::string, spAchievementModel> _achievementMap;
};

#endif