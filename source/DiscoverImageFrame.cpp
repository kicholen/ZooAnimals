#include "DiscoverImageFrame.h"
#include "SharedResources.h"

DiscoverImageFrame::DiscoverImageFrame(const std::string& difficulty) {
	init("LandingPageFrame.xml", true);
	_difficulty = difficulty;
	selectTransitions();
}


DiscoverImageFrame::~DiscoverImageFrame() {

}

void DiscoverImageFrame::selectTransitions() {
	spTransition transition = new TransitionFade;
	setTransitionIn(transition);
	setTransitionOut(transition);
}

void DiscoverImageFrame::_postHiding(Event *) {
	//FlurryAnalytics::instance.onLevelLeaveEvent(_whichLevel.c_str());
	_view->removeChildren();
	_resources.unload();
}

void DiscoverImageFrame::_preShowing(Event *) {
	//FlurryAnalytics::instance.onLevelEnterEvent(_whichLevel.c_str());
	selectTransitions();
	_resources.load();
	setData();
}

Action DiscoverImageFrame::loop() {
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

void DiscoverImageFrame::onFinished(Event *event) {
	_previousAnimal = _previousAnimal < 33 ? _previousAnimal + 1 : 0;
	_totalScore += 1;
	//_counterBox->updateScore(_totalScore);
	_field->reset("dog", 8);
}

void DiscoverImageFrame::onCorrectAnswer(Event *event) {
	_quizElement->reset(10, "-", 4);
	_field->discoverNextElement();
}

void DiscoverImageFrame::setData() {
	_totalScore = 0;
	spColorRectSprite background = new ColorRectSprite();
	background->setColor(Color(88, 144, 217));
	background->setSize(_view->getSize());
	background->setAnchor(0.0f, 0.0f);
	background->setPosition(0.0f, 0.0f);
	background->attachTo(_view);
	_previousAnimal = 0;
	// 320 x 480
	//float scaleFactor = (_view->getHeight() * 1.0f) / 320.0f;
	//_field = new DiscoverImageField(Vector2(scaleFactor * 480.0f, scaleFactor * 320.0f), "bee", 12);
	//getActorScaleBySize(safeSpCast<Actor>(_view), Vector2(_view->getWidth(), _view->getHeight() * 0.66f));
	_field = new DiscoverImageField(Vector2(_view->getWidth(), _view->getHeight() * 0.66f), "bee", 12);
	_field->setPosition(_view->getSize().x / 2 - _field->getDerivedWidth() / 2, 0.0f);//_view->getSize().y * 0.5f - _field->getDerivedHeight() / 2);

	_field->addEventListener(DiscoverImageField::DiscoverImageFieldEvent::FINISHED, CLOSURE(this, &DiscoverImageFrame::onFinished));
	_view->addChild(_field);

	_quizElement = new MathQuizElement(Vector2(_view->getWidth(), _view->getHeight() - _field->getHeight()));
	_quizElement->attachTo(_view);
	_quizElement->setPosition(0.0f, _field->getHeight());
	_quizElement->addEventListener(EquationElement::EquationElementEvent::CORRECT, CLOSURE(this, &DiscoverImageFrame::onCorrectAnswer));
	/*
	_counterBox = new CounterBoxElement(Vector2(getRoot()->getWidth() * 0.9f, getRoot()->getHeight() * 0.15f), 120);
	_counterBox->addEventListener(CounterBoxElement::CounterBoxEvent::TIME_END, CLOSURE(this, &ConnectDotsFrame::onTimesUp));
	_counterBox->show(true);
	_view->addChild(_counterBox);
	*/
	addButton("back", "BACK", Vector2(_view->getWidth() * 0.9f, _view->getHeight() * 0.9f));
}