#include "EndGameAction.h"
#include "DatabaseManager.h"
#include "FlurryAnalytics.h"

EndGameAction::EndGameAction() {
	doAction();
}


EndGameAction::~EndGameAction()
{
}


void EndGameAction::doAction() {
	DatabaseManager::instance.save(0);

	FlurryAnalytics::instance.onSessionEnded();

	complete();
}

void EndGameAction::complete() {
	detach();
}