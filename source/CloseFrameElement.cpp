#include "CloseFrameElement.h"
#include "SharedResources.h"

CloseFrameElement::CloseFrameElement() {
	_state = cfeShown;
	_displayState = cfeLeft;
	setAnchor(0.0f, 0.0f);
	create();
	setDisplayState();
}

CloseFrameElement::~CloseFrameElement()
{
}

void CloseFrameElement::show() {
	if (_state != cfeHidden) {
		return;
	}

	setY(-getHeight() / 2);
	addTween(Sprite::TweenPosition(_showPosition), 300, 1, false, 0, Tween::ease_outBack);
}

void CloseFrameElement::hide(bool shouldAnimate) {
	if (_state != cfeShown) {
		return;
	}

	if (shouldAnimate) {
		addTween(Sprite::TweenPosition(_hidePosition), 300, 1, false, 0, Tween::ease_outBack);
	}
	else {
		setPosition(_hidePosition);
	}
}

void CloseFrameElement::setDisplayState() {
	if (_displayState == cfeLeft) {
		_showPosition = Vector2(0.0f, 0.0f);
		_hidePosition = _showPosition + Vector2(0.0f, - _button->getDerivedHeight());
	}
	else if (_displayState == cfeRght) {
		_showPosition = Vector2(getRoot()->getWidth() - _button->getDerivedWidth(), 0.0f);
		_hidePosition = _showPosition + Vector2(0.0f, - _button->getHeight());
	}
	else {
		log::error("CloseFrameElement::setDisplayState state not available.");
	}
}

void CloseFrameElement::create() {
	_button = new TweenButton();
	_button->setResAnim(gameResources.getResAnim("greenButton"));
	_button->setTouchChildrenEnabled(false);
	_button->setBaseScale(getRoot()->getHeight() * 0.075f / _button->getHeight());
	_button->addEventListener(TouchEvent::CLICK, CLOSURE(this, &CloseFrameElement::dispatchCloseEvent));

	addChild(_button);
}

void CloseFrameElement::dispatchCloseEvent(Event *ev) {
	CloseFrameEvent closeEvent(CloseFrameEvent::CLOSE);
	dispatchEvent(&closeEvent);
}