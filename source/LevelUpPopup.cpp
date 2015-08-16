#include "LevelUpPopup.h"
#include "ExpManager.h"
#include "LevelUpItem.h"
#include "SharedResources.h"
#include "LanguageManager.h"
#include "ProcessMaster.h"
#include "AddRewardsProcess.h"

LevelUpPopup::LevelUpPopup()
{
	init("LandingPageFrame.xml", false);
	selectTransitions();
}

LevelUpPopup::~LevelUpPopup()
{
}

void LevelUpPopup::selectTransitions() {
	spTransition transition = new TransitionScale;
	setTransitionIn(transition);
	setTransitionOut(transition);
}

void LevelUpPopup::_postHiding(Event *) {
	//FlurryAnalytics::instance.onLevelLeaveEvent(_whichLevel.c_str());
	_view->removeChildren();
	_rewardsArray.clear();
	_resources.unload();
}

void LevelUpPopup::_preShowing(Event *) {
	//FlurryAnalytics::instance.onLevelEnterEvent(_whichLevel.c_str());
	selectTransitions();
	_resources.load();
	setData();
}

Action LevelUpPopup::loop() {
	while (1) {
		Action action = waitAction();
		if (action.id == "back" || action.id == "_btn_back_" || action.id == "close") {
			spProcessMaster master = new ProcessMaster();
			master->addProcess(new AddRewardsProcess(_rewardsArray));
			master->start(getRoot());
			break;
		}
	}

	return _lastAction;
}

void LevelUpPopup::setData() {
	createPopupBackground();
	createTitleTextfield();
	createRewards();
	addButton("back", "OK", Vector2(_view->getWidth() / 2.0f, _view->getHeight() / 2.0f + _view->getHeight() / 4.0f));
}

void LevelUpPopup::createPopupBackground() {
	spBox9Sprite cardBackground = new Box9Sprite;
	cardBackground->setHorizontalMode(Box9Sprite::STRETCHING);
	cardBackground->setVerticalMode(Box9Sprite::STRETCHING);
	cardBackground->setAnchor(0.5f, 0.5f);
	cardBackground->setResAnim(gameResources.getResAnim("greyBox9"));
	cardBackground->setSize(_view->getSize() / 2.0f);
	cardBackground->setPosition(_view->getWidth() / 2.0f, _view->getHeight() / 2.0f);
	cardBackground->setGuides(9, 20, 9, 15);
	cardBackground->attachTo(_view);
	cardBackground->setPriority(-1);
}

void LevelUpPopup::createTitleTextfield() {
	TextStyle style;
	style.font = gameResources.getResFont("nobile_bold")->getFont();
	style.vAlign = TextStyle::VALIGN_MIDDLE;
	style.hAlign = TextStyle::HALIGN_CENTER;
	style.multiline = true;
	style.color = Color(35, 145, 245);

	spTextField textField = createTextFieldInBoundries(LanguageManager::instance.getText(100), Vector2(_view->getWidth() / 2.0f, _view->getHeight() * 0.2f), style);
	textField->setAnchorX(0.5f);
	textField->setPosition(_view->getWidth() / 2.0f, _view->getHeight() / 2.0f - _view->getHeight() / 4.0f);
	_view->addChild(textField);
}

void LevelUpPopup::createRewards() {
	_rewardsArray = ExpManager::instance.getRewardsForCurrentLevel();
	spLevelUpItem item = new LevelUpItem(Vector2(_view->getWidth() / 2.0f, _view->getHeight() * 0.3f), 10, false);
	item->setPosition(_view->getSize() / 2.0f);

	for (int i = 0; i < _rewardsArray._vector.size(); i++) {
		item->addReward(_rewardsArray._vector[i]);
	}
	_view->addChild(item);
}