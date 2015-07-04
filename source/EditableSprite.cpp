#include "EditableSprite.h"
#include "SharedResources.h"

EditableSprite::EditableSprite() {
	drag.init(this);
	drag.getDragClient()->addEventListener(TouchEvent::TOUCH_DOWN, CLOSURE(this, &EditableSprite::onTouchEvent));
	_areButtonsCreated = false;
	_areButtonsShown = false;
	_state = "SCALE";
}

void EditableSprite::onTouchEvent(Event *event) {
	bool isSecondClick = false;
	if (_doubleClickTimer) {
		if (_doubleClickTimer->isStarted() && !_doubleClickTimer->isDone()) {
			isSecondClick = true;
		}
		else {
			_doubleClickTimer->reset();
			drag.getDragClient()->removeTween(_doubleClickTimer);
			drag.getDragClient()->addTween(_doubleClickTimer);
		}
	}
	else {
		_doubleClickTimer = createTween(TweenDummy(), 150, 1);
		drag.getDragClient()->addTween(_doubleClickTimer);
	}
	if (isSecondClick) {
		if (_areButtonsShown) {
			hideButtons();
		}
		else {
			showAndCreateButtonsIfNecessary();
		}
	}
}

void EditableSprite::showAndCreateButtonsIfNecessary() {
	if (_areButtonsCreated) {
		showButtons();
	}
	else {
		createButtons(true);
	}
}

void EditableSprite::createButtons(bool shouldShow) {
	addButton("PLUS", "PLUS", Vector2(drag.getDragClient()->getWidth(), 0), Vector2(0.0f, 0.0f));
	addButton("MINUS", "MINUS", Vector2(0.0f, 0.0f), Vector2(1.0f, 0.0f));
	addButton("REMOVE", "REMOVE", Vector2(drag.getDragClient()->getWidth(), drag.getDragClient()->getHeight() / 2), Vector2(0.0f, 0.5f));
	addButton("Z_PRIORITY", "Z_PRIORITY", Vector2(drag.getDragClient()->getWidth(), drag.getDragClient()->getHeight()), Vector2(0.0f, 0.0f));
	addButton("SCALE", "SCALE", Vector2(0.0f, drag.getDragClient()->getHeight()), Vector2(1.0f, 0.0f));
	_areButtonsCreated = true;
	if (shouldShow) {
		showButtons();
	}
	else {
		hideButtons();
	}
}

void EditableSprite::addButton(const std::string &name, const std::string &text, Vector2 position, Vector2 anchor) {
	spTweenButton button = new TweenButton();
	button = createButton(name, text);
	button->setAnchor(anchor);
	button->setPosition(position);
	button->attachTo(drag.getDragClient());
	button->addEventListener(TouchEvent::TOUCH_DOWN, CLOSURE(this, &EditableSprite::onButtonTouch));
}

void EditableSprite::onButtonTouch(Event *event) {
	std::string buttonName = event->currentTarget->getName();

	if (buttonName == "PLUS") {
		if (_state == "SCALE") {
			drag.getDragClient()->setScaleX(drag.getDragClient()->getScaleX() * 1.05f);
			drag.getDragClient()->setScaleY(drag.getDragClient()->getScaleY() * 1.05f);
		}
		else if (_state == "Z_PRIORITY") {
			drag.getDragClient()->setPriority(drag.getDragClient()->getPriority() + 1);
		}
		else {
			printf("can't happen");
		}
	}
	else if (buttonName == "MINUS") {
		if (_state == "SCALE") {
			drag.getDragClient()->setScaleX(drag.getDragClient()->getScaleX() * 0.95f);
			drag.getDragClient()->setScaleY(drag.getDragClient()->getScaleY() * 0.95f);
		}
		else if (_state == "Z_PRIORITY") {
			drag.getDragClient()->setPriority(drag.getDragClient()->getPriority() - 1);
		}
		else {
			printf("can't happen");
		}
	}
	else if (buttonName == "REMOVE") {
		this->detach();
	}
	else if (buttonName == "SCALE") {
		_state = "SCALE";
	}
	else if (buttonName == "Z_PRIORITY") {
		_state = "Z_PRIORITY";
	}
}

void EditableSprite::showButtons() {
	spActor child = drag.getDragClient()->getFirstChild();
	while(child) {
		child->setVisible(true);
		child = child->getNextSibling();
	}
	_areButtonsShown = true;
}

void EditableSprite::hideButtons() {
	spActor child = drag.getDragClient()->getFirstChild();
	while(child) {
		child->setVisible(false);
		child = child->getNextSibling();
	}
	_areButtonsShown = false;
}