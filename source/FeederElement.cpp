#include "FeederElement.h"
#include "SharedResources.h"
#include "FlashUtils.h"
// todo: it's in 1/10 s just convert it to sec and invert = all done
FeederElement::FeederElement(const Vector2& size, int cooldownLeft, int cooldownMax) {
	setTouchEnabled(false);
	setSize(size);
	_cooldownLeft = cooldownLeft;
	_cooldownMax = cooldownMax;

	revalidate();
}


FeederElement::~FeederElement()
{
}

void FeederElement::revalidate() {
	createButton();

	if (_cooldownLeft <= 0) {
		_button->setAlpha(255);
		_button->setTouchEnabled(true);
	}
	else {
		_button->setAlpha(150);
		createProgressbar();
		_progressBar->addTween(ProgressBar::TweenProgress(1.0f), 900);
		_progressBar->addTween(ProgressBar::TweenProgress(0.0f), _cooldownLeft * 1000, 1, false, 1000);

		createCooldownTextfield();
		spTween tween = addTween(FeederElement::TweenCooldown(_cooldownMax), _cooldownLeft * 1000);
		tween->setName("score_tween");
		tween->setDoneCallback(CLOSURE(this, &FeederElement::onTimePassed));
	}
}

void FeederElement::createButton() {
	if (!_button) {
		_button = new TweenButton();
		_button->setResAnim(gameResources.getResAnim("redButton"));
		_button->setAnchor(Vector2(0.5f, 0.5f));
		_button->setTouchChildrenEnabled(false);
		_button->setTouchEnabled(false);
		_button->setTouchChildrenEnabled(false);
		_button->addEventListener(TouchEvent::CLICK, CLOSURE(this, &FeederElement::onFeedClicked));
		_button->setBaseScale((getHeight() * 0.8f) / _button->getHeight());
	}
}

void FeederElement::createProgressbar() {
	if (!_progressBar) {
		_progressBar = initActor(new ProgressBar,
			arg_attachTo = this,
			arg_anchor = Vector2(0.5f, 0.5f),
			arg_position = Vector2(getWidth() / 2.0f, getHeight() / 2.0f),
			arg_resAnim = gameResources.getResAnim("circle_border"));
		_progressBar->setDirection(_progressBar->dir_radial_cw);
		_progressBar->setScale(getWidth() / _progressBar->getWidth());
		_progressBar->setProgress((float)_cooldownLeft / (float)_cooldownMax);
	}
}

void FeederElement::createCooldownTextfield() {
	if (!_cooldown) {
		_cooldown = new TextField();
		_cooldown->setStyle(createTextStyle(gameResources.getResFont("nobile_bold")->getFont(), Color(0, 0, 0, 255), false, TextStyle::HALIGN_CENTER, TextStyle::VALIGN_MIDDLE));
		_cooldown->setSize(getSize()); // todo probably set this to wtf
		addChild(_cooldown);
	}
}


int FeederElement::getCooldown() const {
	return _cooldownLeft;
}

void FeederElement::setCooldown(int cooldown) { // update this only sometimes?
	if (_cooldownLeft == cooldown) {
		return;
	}

	_cooldownLeft = cooldown;
	int hours = (int)std::floor((double)_cooldownLeft / 3600.0f);
	int minutes = (int)std::floor((double)(_cooldownLeft % 3600) / 60.0f);

	if (hours > 0) {
		std::string prefix = hours < 10 ? "0" : "";
		std::string between = minutes < 10 ? ":0" : ":";
		_cooldown->setText(prefix + FlashUtils::CMath::intToString(hours) + between + FlashUtils::CMath::intToString(minutes));
	}
	else {
		int seconds = _cooldownLeft % 60;
		std::string between = seconds < 10 ? ":0" : ":";
		std::string prefix = minutes < 10 ? "0" : "";
		_cooldown->setText(prefix + FlashUtils::CMath::intToString(minutes) + between + FlashUtils::CMath::intToString(seconds));
	}
}

void FeederElement::onFeedClicked(Event *ev) {
	// send fed event to update feed time
	//if () {

	//}
}

void FeederElement::onTimePassed(Event *ev) {
	revalidate();
}
