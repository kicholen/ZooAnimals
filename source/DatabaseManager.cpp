#include "DatabaseManager.h"
#include "AnimalsManager.h"
#include "MoneyManager.h"
#include "ExpManager.h"
#include "ZooSettings.h"
#include "Settings.h"

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
	ZooSettings::instance.save();
	Settings::instance.save();
}