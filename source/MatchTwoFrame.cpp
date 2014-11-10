#include "MatchTwoFrame.h"
#include "Content.h"

MatchTwoFrame::MatchTwoFrame() {
	init("LandingPageFrame.xml", true);
	selectTransitions();
}

void MatchTwoFrame::selectTransitions() {
	spTransition transition = new TransitionFade;
	setTransitionIn(transition);
	setTransitionOut(transition);
}

void MatchTwoFrame::_postHiding(Event *) {
	//FlurryAnalytics::instance.onLevelLeaveEvent(_whichLevel.c_str());
	_view->removeChildren();
	_resources.unload();
}

void MatchTwoFrame::_preShowing(Event *) {
	//FlurryAnalytics::instance.onLevelEnterEvent(_whichLevel.c_str());
	selectTransitions();
	_resources.load();
	setData();
}

Action MatchTwoFrame::loop() {
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

void MatchTwoFrame::onFinished(Event *event) {
	int random = FlashUtils::CMath::random(0, Content::instance.getGameLevelsCount("match_two"));
	_field->restart(3, random);
}

void MatchTwoFrame::setData() {
	spColorRectSprite background = new ColorRectSprite();
	background->setColor(Color(88, 144, 217));
	background->setSize(_view->getSize());
	background->setAnchor(0.0f, 0.0f);
	background->setPosition(0.0f, 0.0f);
	background->attachTo(_view);

	int random = FlashUtils::CMath::random(0, Content::instance.getGameLevelsCount("match_two"));
	_field = new MatchTwoField(Vector2(_view->getWidth() * 0.8f, _view->getHeight()), 3, random);
	_field->setPosition(getRoot()->getSize().x / 2 - _field->getDerivedWidth() / 2, getRoot()->getSize().y / 2 - _field->getDerivedHeight() / 2);
	_field->addEventListener(MatchTwoField::MatchTwoFieldEvent::FINISHED, CLOSURE(this, &MatchTwoFrame::onFinished));
	_view->addChild(_field);

	//_counterBox = new CounterBoxElement(Vector2(getRoot()->getWidth() * 0.9f, getRoot()->getHeight() * 0.15f), 120);
	//_counterBox->addEventListener(CounterBoxElement::CounterBoxEvent::TIME_END, CLOSURE(this, &MemoryFrame::onTimesUp));
	//_counterBox->show(true);
	//_view->addChild(_counterBox);

	addButton("back", "BACK", Vector2(getRoot()->getWidth() * 0.9f, getRoot()->getHeight() * 0.9f));
}