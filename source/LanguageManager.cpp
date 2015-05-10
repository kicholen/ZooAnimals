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

void LanguageManager::init(Language type) {
	setLanguage(type);
}

// todo check performance, maybe load all to some vector 
string LanguageManager::getText(int lockitNumber) {
	return _lockit->getText(lockitNumber);
}

void LanguageManager::setLanguage(Language type) {
	if (_lockit) {
		_lockit->reset();
		_lockit = NULL;
	}

	_lockit = LockitFactory::getInstance(getLanguageCode(type));
}

const char* LanguageManager::getLanguageCode(Language type) {
	const char* languageCode;
	if (type == lmEnglish) {
		languageCode = "EN_en";
	}
	else if (type == lmPolish) {
		languageCode = "PL_pl";
	}

	return languageCode;
}