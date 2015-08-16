#include "DatabaseManager.h"
#include "AnimalsManager.h"
#include "MoneyManager.h"
#include "ExpManager.h"
#include "ZooSettings.h"
#include "Settings.h"
#include "HatManager.h"
#include "AchievementManager.h"
#include "MessageCenterManager.h"

DatabaseManager DatabaseManager::instance;

DatabaseManager::DatabaseManager() {

}

DatabaseManager::~DatabaseManager() {
	if (getRoot()) {
		getRoot()->removeEventListener(RootActor::DEACTIVATE, CLOSURE(this, &DatabaseManager::save));
	}
}

void DatabaseManager::init() {
	getRoot()->addEventListener(RootActor::DEACTIVATE, CLOSURE(this, &DatabaseManager::save));
}

void DatabaseManager::save(Event *ev) {
	AnimalsManager::instance.store();
	MoneyManager::instance.store();
	ExpManager::instance.store();
	HatManager::instance.store();
	AchievementManager::instance.store();
	MessageCenterManager::instance.store();

	ZooSettings::instance.save();
	Settings::instance.save();
}