#include "FeederElement.h"
#include "SharedResources.h"
#include "FlashUtils.h"
#include "StartGameConfig.h"
#include "s3eTimer.h"
#include "AnimalsManager.h"

FeederElement::FeederElement(const Vector2& size, spAnimalModel model) {
	setSize(size);
	_model = model;
	_cooldownMax = FEED_INTERVAL_SECONDS;

	revalidate();
}

FeederElement::~FeederElement()
{
}

void FeederElement::revalidate() {
	_cooldownLeft = FEED_INTERVAL_SECONDS - ((int)(s3eTimerGetUTC() / 1000) - _model->lastFeedS());
	
	createButton();

	if (_cooldownLeft <= 0) {
		_button->setAlpha(255);
		_button->setTouchEnabled(true);
		_button->setTouchChildrenEnabled(true);
		if (_cooldown) {
			_cooldown->setVisible(false);
		}
	}
	else {
		_button->setTouchChildrenEnabled(false);
		_button->setTouchEnabled(false);
		_button->setAlpha(150);
		createProgressbar();
		_progressBar->addTween(ProgressBar::TweenProgress(0.0f), _cooldownLeft * 1000, 1, false, 1000);

		createCooldownTextfield();
		_cooldown->setVisible(true);
		spTween tween = addTween(FeederElement::TweenCooldown(0), _cooldownLeft * 1000);
		tween->setName("score_tween");
		tween->setDoneCallback(CLOSURE(this, &FeederElement::onTimePassed));
	}
}

void FeederElement::createButton() {
	if (!_button) {
		_button = new TweenButton();
		_button->setResAnim(gameResources.getResAnim("circle_border"));
		_button->setAnchor(Vector2(0.5f, 0.5f));
		_button->setPosition(getWidth() / 2.0f, getHeight() / 2.0f);
		_button->setBaseScale(getWidth() / _button->getWidth());
		_button->addEventListener(TouchEvent::CLICK, CLOSURE(this, &FeederElement::onFeedClicked));
		addChild(_button);
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
		_progressBar->setTouchEnabled(false);
		_progressBar->setTouchChildrenEnabled(false);
	}
}

void FeederElement::createCooldownTextfield() {
	if (!_cooldown) {
		_cooldown = new TextField();
		_cooldown->setStyle(createTextStyle(gameResources.getResFont("nobile_bold")->getFont(), Color(0, 0, 0, 255), false, TextStyle::HALIGN_CENTER, TextStyle::VALIGN_MIDDLE));
		_cooldown->setSize(getSize()); // todo probably set this to wtf
		_cooldown->setTouchEnabled(false);
		_cooldown->setTouchChildrenEnabled(false);
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
	AnimalsManager::instance.feedAnimalByModel(_model);
	revalidate();
}

void FeederElement::onTimePassed(Event *ev) {
	revalidate();
}
