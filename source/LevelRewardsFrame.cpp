#include "LevelRewardsFrame.h"
#include "ExpManager.h"
#include "FlashUtils.h"
#include "RewardModel.h"
#include "LevelUpItem.h"

using namespace FlashUtils;

LevelRewardsFrame::LevelRewardsFrame() {
	init("LandingPageFrame.xml", true);
	selectTransitions();
}

LevelRewardsFrame::~LevelRewardsFrame() {

}

void LevelRewardsFrame::selectTransitions() {
	spTransition transition = new TransitionFade;
	setTransitionIn(transition);
	setTransitionOut(transition);
}

void LevelRewardsFrame::_postHiding(Event *) {
	//FlurryAnalytics::instance.onLevelLeaveEvent(_whichLevel.c_str());
	_view->removeChildren();
	_resources.unload();
}

void LevelRewardsFrame::_preShowing(Event *) {
	//FlurryAnalytics::instance.onLevelEnterEvent(_whichLevel.c_str());
	selectTransitions();
	_resources.load();
	setData();
}


Action LevelRewardsFrame::loop() {
	while (1) {
		Action action = waitAction();
		if (action.id == "back" || action.id == "_btn_back_") {
			break;
		}
		else if (action.id == "close") {
			break;
		}
		else if (action.id == "result") {
			break; // todo
		}
	}

	return _lastAction;
}

void LevelRewardsFrame::setData() {
	_rotatingContainer = new SlidingActor();
	_rotatingContainer->setSize(_view->getWidth(), _view->getHeight());
	_rotatingContainer->setPriority(-5);

	spColorRectSprite rectangleContainer = new ColorRectSprite();
	rectangleContainer->setSize(_rotatingContainer->getWidth(), _rotatingContainer->getHeight());
	rectangleContainer->setColor(Color(0, 0, 0, 0));

	float positionY = _rotatingContainer->getHeight() * 0.3f / 2.0f + OFFSET;
	float itemHeight = 0.0f;
	const VectorArray< VectorArray<spRewardModel> >& rewardsArray = ExpManager::instance.getAllRewards();
	const Vector2& size = Vector2(_rotatingContainer->getWidth() * 0.9f, _rotatingContainer->getHeight() * 0.3f);
	
	for (int i = 0; i < rewardsArray._vector.size(); i++) {
		spLevelUpItem item = new LevelUpItem(size, i + 1);
		item->setPosition(_rotatingContainer->getWidth() / 2.0f, positionY);
		positionY += item->getDerivedHeight() + OFFSET;
		itemHeight = item->getDerivedHeight();
		rectangleContainer->addChild(item);
		
		for (int j = 0; j < rewardsArray._vector[i]._vector.size(); j++) {
			item->addReward(rewardsArray._vector[i]._vector[j]);
		}
	}
	
	rectangleContainer->setSize(_rotatingContainer->getWidth(), positionY - itemHeight / 2.0f);
	_rotatingContainer->setContent(rectangleContainer);
	_rotatingContainer->setPosition(_view->getSize() / 2 - _rotatingContainer->getSize() / 2);
	_rotatingContainer->attachTo(_view);

	/*
	_counterBox = new CounterBoxElement(Vector2(getRoot()->getWidth() * 0.9f, getRoot()->getHeight() * 0.15f), 120);
	_counterBox->addEventListener(CounterBoxElement::CounterBoxEvent::TIME_END, CLOSURE(this, &ConnectDotsFrame::onTimesUp));
	_counterBox->show(true);
	_view->addChild(_counterBox);
	*/
	addButton("back", "BACK", Vector2(_view->getWidth() * 0.9f, _view->getHeight() * 0.9f));
}
