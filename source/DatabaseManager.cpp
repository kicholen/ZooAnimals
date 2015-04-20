#include "DatabaseManager.h"
#include "AnimalsManager.h"
#include "MoneyManager.h"
#include "ZooSettings.h"
#include "Settings.h"

DatabaseManager DatabaseManager::instance;

DatabaseManager::DatabaseManager() {

}

DatabaseManager::~DatabaseManager() {
	getRoot()->removeEventListener(RootActor::DEACTIVATE, CLOSURE(this, &DatabaseManager::save));
}

void DatabaseManager::init() {
	getRoot()->addEventListener(RootActor::DEACTIVATE, CLOSURE(this, &DatabaseManager::save));
}

void DatabaseManager::save(Event *ev) {
	AnimalsManager::instance.store();
	MoneyManager::instance.store();
	ZooSettings::instance.save();
	Settings::instance.save();
}