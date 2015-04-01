#include "Slider.h"

namespace oxygine
{
	Slider::Slider(float buttonHeight) {
		_clip->removeEventListeners(this);
		_clip->addEventListener(TouchEvent::TOUCH_UP, CLOSURE(this, &Slider::_newEvent));
		_clip->addEventListener(TouchEvent::OUT, CLOSURE(this, &Slider::_newEvent));
		_buttonHeight = buttonHeight;
	}

	void Slider::_newEvent(Event *event) {
		TouchEvent *te = safeCast<TouchEvent*>(event);
		timeMS tm = getTimeMS();
		switch(te->type) {
			case TouchEvent::TOUCH_UP: {
				float offset = 0.05f;
				//float currentDiff = scalar::abs(_content->getPosition().y) - scalar::abs(-_content->getHeight() / 2 + _buttonHeight / 2);
				//if (currentDiff <= offset && currentDiff >= -offset) {
				if (_content->getPosition().y + offset >= -_content->getHeight() / 2 + _buttonHeight / 2 
					&& _content->getPosition().y - offset <= -_content->getHeight() / 2 + _buttonHeight / 2) {
					CompletedEvent completedEvent(CompletedEvent::COMPLETED);
					dispatchEvent(&completedEvent);
				}
				else {
					_content->setPosition(0.0f, 0.0f);
				}
			}
			break;
			case TouchEvent::OUT: {
				_content->setPosition(0.0f, 0.0f);
			}
			break;
		}
	}

	void Slider::reset() {
		_content->setPosition(0.0f, 0.0f);
	}

	void Slider::startDrag() {
		_drag.startDrag(Vector2(this->getDerivedWidth() / 2, this->getDerivedHeight() * 0.95f));
	}

	void Slider::addChildToClip(spActor actor) {
		_clip->addChild(actor.get());
	}
}