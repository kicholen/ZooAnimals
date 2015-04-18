#include "FindShadowFrame.h"

FindShadowFrame::FindShadowFrame(const string& difficulty) {
	init("LandingPageFrame.xml", true);
	_difficulty = difficulty;
	selectTransitions();
}

void FindShadowFrame::selectTransitions() {
	spTransition transition = new TransitionFade;
	setTransitionIn(transition);
	setTransitionOut(transition);
}

void FindShadowFrame::_postHiding(Event *) {
	//FlurryAnalytics::instance.onLevelLeaveEvent(_whichLevel.c_str());
	_view->removeChildren();
	_resources.unload();
}

void FindShadowFrame::_preShowing(Event *) {
	//FlurryAnalytics::instance.onLevelEnterEvent(_whichLevel.c_str());
	selectTransitions();
	_resources.load();
	setData();
}

Action FindShadowFrame::loop() {
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

void FindShadowFrame::onFinished(Event *event) {
	int random = int(FlashUtils::CMath::Rand(2, 4));
	_field->restart(random, "dog");
}

void FindShadowFrame::setData() {
	spColorRectSprite background = new ColorRectSprite();
	background->setColor(Color(88, 144, 217));
	background->setSize(_view->getSize());
	background->setAnchor(0.0f, 0.0f);
	background->setPosition(0.0f, 0.0f);
	background->attachTo(_view);

	_field = new FindShadowField(Vector2(_view->getWidth() * 0.8f, _view->getHeight()), 3, "cat");
	_field->setPosition(getRoot()->getSize().x / 2 - _field->getDerivedWidth() / 2, getRoot()->getSize().y / 2 - _field->getDerivedHeight() / 2);
	_field->addEventListener(FindShadowField::FindShadowFieldEvent::SHADOW_FOUND, CLOSURE(this, &FindShadowFrame::onFinished));
	_view->addChild(_field);

	//_counterBox = new CounterBoxElement(Vector2(getRoot()->getWidth() * 0.9f, getRoot()->getHeight() * 0.15f), 120);
	//_counterBox->addEventListener(CounterBoxElement::CounterBoxEvent::TIME_END, CLOSURE(this, &MemoryFrame::onTimesUp));
	//_counterBox->show(true);
	//_view->addChild(_counterBox);

	addButton("back", "BACK", Vector2(getRoot()->getWidth() * 0.9f, getRoot()->getHeight() * 0.9f));
}