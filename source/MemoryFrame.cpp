#include "MemoryFrame.h"
#include "FlurryAnalytics.h"

MemoryFrame::MemoryFrame(const std::string& whichLevel) {
	init("LandingPageFrame.xml", true);
	_whichLevel = whichLevel;
	_size = getSize();
	selectTransitions();
}

void MemoryFrame::selectTransitions() {
	spTransition transition = new TransitionFade;
	setTransitionIn(transition);
	setTransitionOut(transition);
}

void MemoryFrame::_postHiding(Event *) {
	//FlurryAnalytics::instance.onLevelLeaveEvent(_whichLevel.c_str());
	_view->removeChildren();
	_resources.unload();
}

void MemoryFrame::_preShowing(Event *) {
	//FlurryAnalytics::instance.onLevelEnterEvent(_whichLevel.c_str());
	selectTransitions();
	_resources.load();
	setData();
}

Action MemoryFrame::loop() {
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

void MemoryFrame::onGoBack(Event *event) {
	const std::string &name = "close";
	generateAction(name);
}

void MemoryFrame::onShowSliderFrame(Event *event) {
	const std::string &name = "slideFrame";
	generateAction(name);
}

void MemoryFrame::onTimesUp(Event *event) {
	const std::string &name = "result";
	generateAction(name);
}

void MemoryFrame::onCardsScored(Event *event) {
	_currentLevelCardsScored += 2;
	_totalScore += 2 * _levelScoreMultiplier;
	_counterBox->updateScore(_totalScore);

	if (_maxCards == _currentLevelCardsScored) {
		_currentLevelCardsScored = 0;
		_level += 1;
		_counterBox->updateLevel(_level);
		_field->fillField();
		_field->animateField();
	}
}

void MemoryFrame::setData() {
	spColorRectSprite background = new ColorRectSprite();
	background->setColor(Color(88, 144, 217));
	background->setSize(_view->getSize());
	background->setAnchor(0.0f, 0.0f);
	background->setPosition(0.0f, 0.0f);
	background->attachTo(_view);

	_currentLevelCardsScored = 0;
	_maxCards = _size.x * _size.y;
	_totalScore = 0;
	_levelScoreMultiplier = 1;
	_level = 1;
	_field = new MemoryField(_size);
	float scale = getRoot()->getHeight() * 0.8f / _field->getHeight();
	_field->setScale(scale);

	_field->setPosition(getRoot()->getSize().x / 2 - _field->getSize().x * scale / 2, getRoot()->getSize().y * 0.6f - _field->getSize().y * scale / 2);
	_field->addEventListener(MemoryField::MemoryFieldEvent::CARDS_SCORED, CLOSURE(this, &MemoryFrame::onCardsScored));
	_view->addChild(_field);

	_counterBox = new CounterBoxElement(Vector2(getRoot()->getWidth() * 0.9f, getRoot()->getHeight() * 0.15f), 120);
	_counterBox->addEventListener(CounterBoxElement::CounterBoxEvent::TIME_END, CLOSURE(this, &MemoryFrame::onTimesUp));
	_counterBox->show(true);
	_view->addChild(_counterBox);

	addButton("back", "BACK", Vector2(getRoot()->getWidth() * 0.9f, getRoot()->getHeight() * 0.9f));
}

Point MemoryFrame::getSize() {
	if (_whichLevel == "easy") {
		return Point(3, 2);
	}
	else if (_whichLevel == "normal") {
		return Point(4, 3);
	}
	else if (_whichLevel == "hard") {
		return Point(6, 4);
	}
	else {
		std::string error = "Not excepted parametr: " + _whichLevel;
		oxygine::log::error(error.c_str());

		return Point(0, 0);
	}
}