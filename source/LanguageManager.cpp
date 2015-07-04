#include "LanguageManager.h"
#include "LockitFactory.h"
#include "FlashUtils.h"

LanguageManager LanguageManager::instance;

LanguageManager::LanguageManager() {

}

LanguageManager::~LanguageManager() {
	if (_lockit) {
		_lockit->reset();
		_lockit = NULL;
	}
}

void LanguageManager::init(int type) {
	setLanguage(type);
}

// todo check performance, maybe load all to some vector 
std::string LanguageManager::getText(int lockitNumber) {
	return _lockit->getText(lockitNumber);
}

void LanguageManager::setLanguage(int type) {
	if (_lockit) {
		_lockit->reset();
		_lockit = NULL;
	}

	_lockit = LockitFactory::getInstance(getLanguageCode(type).c_str());
}

std::string LanguageManager::getLanguageCode(int type) {
	std::string languageCode;
	if (type == lmEnglish) {
		languageCode = "EN_en";
	}
	else if (type == lmPolish) {
		languageCode = "PL_pl";
	}

	return languageCode;
}