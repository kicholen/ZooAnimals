#include "AchievementManager.h"
#include "Content.h"
#include "ZooSettings.h"
#include "MessageCenterManager.h"

AchievementManager AchievementManager::instance;

AchievementManager::AchievementManager() {

}

AchievementManager::~AchievementManager() {
	_achievementList.clear();
}

void AchievementManager::init() {
	parseContent();
	parseSavedState();
}

void AchievementManager::store() {
	std::vector < int > achievements;
	for (int i = 0; i < _achievementList.length(); i++) {
		achievements.push_back(_achievementList[i]->getProgress());
	}
	ZooSettings::instance.setAchievement(achievements);
	achievements.clear();
}

void AchievementManager::increaseProgress(AchievementType type) {
	spAchievementModel model = _achievementList[type];
	
	if (model->isCompleted()) {
		oxygine::log::messageln("Achievement already completed");
		return;
	}
	if (model->increaseProgress()) {
		//dispatchAchievementGainedEvent(); can't dispatch event through static, not added too root
		MessageCenterManager::instance.onAchievementsGained(model);
		model->revalidate();
	}
}

void AchievementManager::increaseSpeciesPossesed(const std::string& region) {
	increaseProgress(static_cast<AchievementType>(getAchievementTypeByRegion(region)));
}

bool AchievementManager::isCompleted(AchievementType type) {
	return _achievementList[type]->isCompleted();
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
		int partNum = 0;

		while (!attributePart.empty()) {
			model->addPart(attributePart.as_int());

			pugi::xml_node reward = part.first_child();

			while (!reward.empty()) {
				model->addReward(partNum, reward.name());
				reward = reward.next_sibling();
			}

			attributePart = attributePart.next_attribute();
		}

		_achievementList.push(model);
		model->revalidate();

		achievement = achievement.next_sibling();
	}
}

void AchievementManager::parseSavedState() {
	pugi::xml_node achievements = ZooSettings::instance.getAchievementsNode();
	pugi::xml_attribute attribute = achievements.first_attribute();
	int achievementNum = 0;

	while (!attribute.empty()) {
		spAchievementModel model = _achievementList[achievementNum];
		model->setProgress(attribute.as_int());
		model->revalidate();
		achievementNum += 1;

		attribute = attribute.next_attribute();
	}
}

AchievementType AchievementManager::getAchievementTypeByRegion(const std::string& region) {
	if (region == "farm") {
		return amCollectFarm;
	}
	else if (region == "winter") {
		return amCollectWinter;
	}
	else if (region == "underwater") {
		return amCollectUnder;
	}
	else if (region == "steppe") {
		return amCollectSteppe;
	}
	else if (region == "asia") {
		return amCollectAsia;
	}
	else { //if (region == "australia")
		return amCollectAustralia;
	}
}

void AchievementManager::dispatchAchievementGainedEvent(spAchievementModel model) {
	AchievementEvent ev(AchievementEvent::ACHIEVEMENT_GAINED, model);
	dispatchEvent(&ev);
}