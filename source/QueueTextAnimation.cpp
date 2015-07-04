#include "QueueTextAnimation.h"
#include "SharedResources.h"

QueueTextAnimation::QueueTextAnimation() {
	
}

QueueTextAnimation::~QueueTextAnimation() {
	_textQueue.clear();
}

void QueueTextAnimation::start(EventCallback onAnimationFinishedCallback, bool shouldCallOnPenultimate) {
	_onAnimationFinishedCallback = onAnimationFinishedCallback;
	_shouldCallOnPenultimate = shouldCallOnPenultimate;

	animateNext(0);
}

void QueueTextAnimation::animateNext(Event *ev) {
	int textsLeft = _textQueue.size();

	if (ev) {
		safeSpCast <TextField>(ev->target)->detach();
	}
	
	if (_shouldCallOnPenultimate) {
		if (textsLeft == 1) {
			callAnimationFinishedCallback();
		}
	}
	else if (textsLeft == 0) {
		callAnimationFinishedCallback();
	}

	if (textsLeft > 0) {
		createTextField()->addTween(TweenScale(1.2f), 700)->addDoneCallback(CLOSURE(this, &QueueTextAnimation::animateNext));
	}
	else {
		detach();
	}
}

void QueueTextAnimation::addTextToQueue(const std::string& text) {
	_textQueue.push_front(text);
}

spTextField QueueTextAnimation::createTextField() {
	TextStyle style;
	style.font = gameResources.getResFont("nobile_bold")->getFont();
	style.vAlign = TextStyle::VALIGN_MIDDLE;
	style.hAlign = TextStyle::HALIGN_CENTER;
	style.multiline = true;
	style.color = Color(222, 145, 245);
	style.fontSize2Scale = 50;

	spTextField textField = initActor(new TextField,
		arg_style = style,
		arg_hAlign = TextStyle::HALIGN_MIDDLE,
		arg_vAlign = TextStyle::VALIGN_MIDDLE,
		arg_attachTo = this,
		arg_width = getDerivedWidth(),
		arg_height = getDerivedHeight(),
		arg_input = false,
		arg_priority = 20,
		arg_text = _textQueue.front());
	textField->setAnchor(0.5f, 0.5f);
	textField->setPosition(getDerivedSize() / 2.0f);

	_textQueue.pop_front();

	return textField;
}

void QueueTextAnimation::callAnimationFinishedCallback() {
	QueueTextEvent ev(1);

	if (_onAnimationFinishedCallback) {
		_onAnimationFinishedCallback(&ev);
	}
}