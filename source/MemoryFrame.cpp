#include "MemoryFrame.h"
#include "FlurryAnalytics.h"

MemoryFrame::MemoryFrame(const char* whichLevel) {
	init("LandingPageFrame.xml", true);
	//_whichLevel = whichLevel;
	slideFrame->addEventListener(SlideFrame::GoBackToPreviousFrameEvent::GO_BACK, CLOSURE(this, &MemoryFrame::onGoBack));

	selectTransitions();
}

void MemoryFrame::selectTransitions() {
	spTransition transition = new TransitionQuads;
	setTransitionIn(transition);
	setTransitionOut(transition);
}

void MemoryFrame::_postHiding(Event *) {
	//FlurryAnalytics::instance.onLevelLeaveEvent(_whichLevel.c_str());
	slideFrame->removeEventListener(SlideFrame::GoBackToPreviousFrameEvent::GO_BACK, CLOSURE(this, &MemoryFrame::onGoBack));
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
		if (action.id == "slideFrame" || action.id == "_btn_back_") {
			transitionShowFrameAsDialog(slideFrame);
		}
		else if (action.id == "back") {
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
	const string &name = "close";
	generateAction(name);
}

void MemoryFrame::onShowSliderFrame(Event *event) {
	const string &name = "slideFrame";
	generateAction(name);
}

void MemoryFrame::onTimesUp(Event *event) {
	const string &name = "result";
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

	spButton sliderButton = new Button();
	sliderButton->setResAnim(gameResources.getResAnim("back_button"));
	sliderButton->setScale(_content->getWidth() * 8 / 100 / sliderButton->getWidth());
	sliderButton->setPosition(Vector2(_content->getWidth() * 9 / 10 - 12, _content->getHeight() * 95 / 100 - sliderButton->getDerivedHeight()));

	sliderButton->setName("slideFrame");
	sliderButton->addEventListener(TouchEvent::TOUCH_DOWN, CLOSURE(this, &MemoryFrame::onShowSliderFrame));
	
	_currentLevelCardsScored = 0;
	_maxCards = 6;
	_totalScore = 0;
	_levelScoreMultiplier = 1;
	_level = 1;
	_field = new MemoryField(Point(2, 3));
	float scale = getRoot()->getHeight() * 0.8f / _field->getHeight();
	_field->setScale(scale);

	_field->setPosition(getRoot()->getSize().x / 2 - _field->getSize().x * scale / 2, getRoot()->getSize().y * 0.6f - _field->getSize().y * scale / 2);
	_field->addEventListener(MemoryField::MemoryFieldEvent::CARDS_SCORED, CLOSURE(this, &MemoryFrame::onCardsScored));
	_view->addChild(_field);

	_counterBox = new CounterBoxElement(Vector2(getRoot()->getWidth() * 0.9f, getRoot()->getHeight() * 0.15f), 120);
	_counterBox->addEventListener(CounterBoxElement::CounterBoxEvent::TIME_END, CLOSURE(this, &MemoryFrame::onTimesUp));

	//counter->setScaleX(getRoot()->getWidth() * 0.8f / counter->getWidth());
	//counter->setScaleY(getRoot()->getHeight() * 0.15f / counter->getHeight());

	//counter->setPosition(getRoot()->getWidth() / 2, -getRoot()->getHeight() * 0.75f);//getSize() / 2 - field->getSize() * scale / 2);
	_counterBox->show(true);
	_view->addChild(_counterBox);

	sliderButton->attachTo(_view);
}