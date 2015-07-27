#include "CloseFrameElement.h"
#include "SharedResources.h"

CloseFrameElement::CloseFrameElement() {
	_state = cfeHidden;
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

void CloseFrameElement::hide() {
	if (_state != cfeShown) {
		return;
	}

	addTween(Sprite::TweenPosition(_hidePosition), 300, 1, false, 0, Tween::ease_outBack);
}

void CloseFrameElement::setDisplayState() {
	if (_displayState == cfeLeft) {
		_showPosition = Vector2(0.0f, 0.0f);
		_hidePosition = _showPosition + Vector2(0.0f, -getHeight());
	}
	else if (_displayState == cfeRght) {
		_showPosition = Vector2(getRoot()->getWidth() - getDerivedWidth(), 0.0f);
		_hidePosition = _showPosition + Vector2(0.0f, -getHeight());
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