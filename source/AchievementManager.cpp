#include "AchievementManager.h"
#include "Content.h"
#include "ZooSettings.h"

AchievementManager AchievementManager::instance;

AchievementManager::AchievementManager() {

}

AchievementManager::~AchievementManager() {
	_achievementList.clear();
	_achievementMap.clear();
}

void AchievementManager::init() {
	parseContent();
	parseSavedState();
}

void AchievementManager::store() {
	for (map<string, spAchievementModel >::iterator outerIterator = _achievementMap.begin(); outerIterator != _achievementMap.end(); ++outerIterator) {
		ZooSettings::instance.setAchievement(outerIterator->first, outerIterator->second);
	}
}

void AchievementManager::increaseProgress(AchievementState state) {
	spAchievementModel model = _achievementList[state];
	if (model->increaseProgress()) {
		dispatchAchievementGainedEvent(model);
		model->revalidate();
	}
}

const std::vector<spAchievementModel>& AchievementManager::getAchievements() const {
	return _achievementList._vector;
}

void AchievementManager::parseContent() {
	pugi::xml_node achievement = Content::instance.getAchievementNode().first_child();
	std::string resource = "";
	int lockitTitle = 0;
	int lockitDescription = 0;
	std::string state = "";

	while (!achievement.empty()) {
		pugi::xml_attribute attribute = achievement.first_attribute();

		while (!attribute.empty()) {
			if (!strcmp(attribute.name(), "resource")) {
				resource = attribute.as_string();
			}
			else if (!strcmp(attribute.name(), "title")) {
				lockitTitle = attribute.as_int();
			}
			else if (!strcmp(attribute.name(), "description")) {
				lockitDescription = attribute.as_int();
			}
			else if (!strcmp(attribute.name(), "state")) {
				state = attribute.as_string();
			}

			attribute = attribute.next_attribute();
		}

		spAchievementModel model = new AchievementModel(resource, lockitTitle, lockitDescription, state);
		

		pugi::xml_node part = achievement.first_child();
		pugi::xml_attribute attributePart = part.first_attribute();

		while (!attributePart.empty()) {
			model->addPart(attributePart.as_int());

			attributePart = attributePart.next_attribute();
		}

		_achievementList.push(model);
		_achievementMap[achievement.name()] = model;

		achievement = achievement.next_sibling();
	}
}

void AchievementManager::parseSavedState() {
	pugi::xml_node achievements = ZooSettings::instance.getAchievementsNode();
	
	if (!achievements.empty()) {
		pugi::xml_node achievement = achievements.first_child();
		pugi::xml_attribute attribute = achievement.first_attribute();
		spAchievementModel model = _achievementMap[achievement.name()];

		while (!attribute.empty()) {
			model->addPart(attribute.as_int());

			attribute = attribute.next_attribute();
		}
		model->revalidate();
	}
}

void AchievementManager::dispatchAchievementGainedEvent(spAchievementModel model) {
	AchievementEvent ev(AchievementEvent::ACHIEVEMENT_GAINED, model);
	dispatchEvent(&ev);
}