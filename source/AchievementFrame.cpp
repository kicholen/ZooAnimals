#include "AchievementFrame.h"
#include "AchievementManager.h"
#include "SharedResources.h"
#include "LanguageManager.h"
#include "AchievementItem.h"

AchievementFrame::AchievementFrame() {
	init("LandingPageFrame.xml", true);
	selectTransitions();
}

AchievementFrame::~AchievementFrame() {

}

void AchievementFrame::selectTransitions() {
	spTransition transition = new TransitionMove;
	setTransitionIn(transition);
	setTransitionOut(transition);
}

void AchievementFrame::_postHiding(Event *) {
	//FlurryAnalytics::instance.onLevelLeaveEvent(_whichLevel.c_str());
	_view->removeChildren();
	_resources.unload();
}

void AchievementFrame::_preShowing(Event *) {
	//FlurryAnalytics::instance.onLevelEnterEvent(_whichLevel.c_str());
	selectTransitions();
	_resources.load();
	setData();
}

Action AchievementFrame::loop() {
	while (1) {
		Action action = waitAction();
		if (action.id == "back" || action.id == "_btn_back_" || action.id == "close") {
			break;
		}
	}

	return _lastAction;
}

void AchievementFrame::setData() {
	//createPopupBackground();
	createTitleTextfield();
	createAchievementsList();
}

void AchievementFrame::createPopupBackground() {
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

void AchievementFrame::createTitleTextfield() {
	TextStyle style;
	style.font = gameResources.getResFont("nobile_bold")->getFont();
	style.vAlign = TextStyle::VALIGN_MIDDLE;
	style.hAlign = TextStyle::HALIGN_CENTER;
	style.multiline = true;
	style.color = Color(35, 145, 245);

	spTextField textField = createTextFieldInBoundries(LanguageManager::instance.getText(100), Vector2(_view->getWidth() / 2.0f, _view->getHeight() * 0.2f), style);
	textField->setAnchorX(0.5f);
	textField->setX(_view->getWidth() / 2.0f);
	_view->addChild(textField);
}

void AchievementFrame::createAchievementsList() {
	spSlidingActor slidingActor = new SlidingActor();
	slidingActor->setSize(_view->getWidth(), _view->getHeight());

	spColorRectSprite rectangleContainer = new ColorRectSprite();
	rectangleContainer->setSize(slidingActor->getWidth(), slidingActor->getHeight());
	rectangleContainer->setColor(Color(0, 0, 0, 0));

	float positionY = slidingActor->getHeight() * 0.3f / 2.0f + OFFSET_ACHIEVEMENT;
	float itemHeight = 0.0f;

	const std::vector<spAchievementModel>& achievements = AchievementManager::instance.getAchievements();

	const Vector2& size = Vector2(slidingActor->getWidth() * 0.9f, slidingActor->getHeight() * 0.2f);
	
	for (int i = 0; i < achievements.size(); i++) {
		spAchievementItem item = new AchievementItem(size);
		item->setData(achievements[i]);
		item->setPosition(slidingActor->getWidth() / 2.0f, positionY);
		positionY += item->getDerivedHeight() + OFFSET_ACHIEVEMENT;
		itemHeight = item->getDerivedHeight();
		rectangleContainer->addChild(item);
	}

	rectangleContainer->setSize(slidingActor->getWidth(), positionY - itemHeight / 2.0f);
	slidingActor->setContent(rectangleContainer);
	slidingActor->setPosition(_view->getSize() / 2.0f - slidingActor->getSize() / 2.0f);
	slidingActor->attachTo(_view);
}