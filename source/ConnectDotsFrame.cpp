#include "ConnectDotsFrame.h"

ConnectDotsFrame::ConnectDotsFrame() {
	init("LandingPageFrame.xml", true);
	selectTransitions();
}

void ConnectDotsFrame::selectTransitions() {
	spTransition transition = new TransitionFade;
	setTransitionIn(transition);
	setTransitionOut(transition);
}

void ConnectDotsFrame::_postHiding(Event *) {
	//FlurryAnalytics::instance.onLevelLeaveEvent(_whichLevel.c_str());
	_view->removeChildren();
	_resources.unload();
}

void ConnectDotsFrame::_preShowing(Event *) {
	//FlurryAnalytics::instance.onLevelEnterEvent(_whichLevel.c_str());
	selectTransitions();
	_resources.load();
	setData();
}

Action ConnectDotsFrame::loop() {
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

void ConnectDotsFrame::onFinished(Event *event) {
	_field->reset(1);
}

void ConnectDotsFrame::setData() {
	spColorRectSprite background = new ColorRectSprite();
	background->setColor(Color(88, 144, 217));
	background->setSize(_view->getSize());
	background->setAnchor(0.0f, 0.0f);
	background->setPosition(0.0f, 0.0f);
	background->attachTo(_view);

	_field = new ConnectDotsField(2);
	//float scale = getRoot()->getHeight() * 0.8f / _field->getHeight();

	// 320 x 480
	// todo: it's bad
	float m_height = 150;
	float m_width = 100;
	bool foundOne = false;

	while (!foundOne) {
		if (m_height * 1.1f <= _view->getHeight() * 0.9f && m_width * 1.1f <= _view->getWidth() * 0.9f) {
			m_height *= 1.1f;
			m_width *= 1.1f;
		}
		else {
			foundOne = true;
		}
	}
	_field->setScaleX(m_width / _field->getWidth());
	_field->setScaleY(m_height / _field->getHeight());

	_field->setPosition(getRoot()->getSize().x / 2 - _field->getDerivedWidth() / 2, getRoot()->getSize().y * 0.6f - _field->getDerivedHeight() / 2);
	_field->addEventListener(ConnectDotsField::ConnectDotsFieldEvent::FINISHED, CLOSURE(this, &ConnectDotsFrame::onFinished));
	_view->addChild(_field);

	//_counterBox = new CounterBoxElement(Vector2(getRoot()->getWidth() * 0.9f, getRoot()->getHeight() * 0.15f), 120);
	//_counterBox->addEventListener(CounterBoxElement::CounterBoxEvent::TIME_END, CLOSURE(this, &MemoryFrame::onTimesUp));
	//_counterBox->show(true);
	//_view->addChild(_counterBox);

	addButton("back", "BACK", Vector2(getRoot()->getWidth() * 0.9f, getRoot()->getHeight() * 0.9f));
}