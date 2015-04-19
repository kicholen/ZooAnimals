#include "DatabaseManager.h"
#include "AnimalsManager.h"
#include "MoneyManager.h"
#include "ZooSettings.h"
#include "Settings.h"

DatabaseManager DatabaseManager::instance;

DatabaseManager::DatabaseManager() {

}

DatabaseManager::~DatabaseManager() {

}

void DatabaseManager::save() {
	AnimalsManager::instance.store();
	MoneyManager::instance.store();
	ZooSettings::instance.save();
	Settings::instance.save();
}