#include "CleanerElement.h"
#include "StartGameConfig.h"
#include "s3eTimer.h"
#include "SharedResources.h"
#include "FlashUtils.h"
#include "ProcessMaster.h"
#include "CleanAnimalsProcess.h"

CleanerElement::CleanerElement(const Vector2& size, spAnimalModel model) {
	setSize(size);
	_model = model;
	_cooldownMax = CLEAN_INTERVAL_SECONDS;
	createBackground();
	revalidate();
}

CleanerElement::~CleanerElement() {

}

void CleanerElement::revalidate() {
	int cooldownPassed = (int)(s3eTimerGetUTC() / 1000) - _model->lastCleanS();
	_cooldownLeft = CLEAN_INTERVAL_SECONDS - cooldownPassed;

	createButton();

	if (_cooldownLeft <= 0) {
		_button->setVisible(255);
		_button->setTouchEnabled(true);
		_button->setTouchChildrenEnabled(true);
		if (_cooldown) {
			_cooldown->setVisible(false);
		}
		_background->setVisible(false);
	}
	else {
		_background->setVisible(true);
		_button->setTouchChildrenEnabled(false);
		_button->setTouchEnabled(false);
		_button->setVisible(0);
		createProgressbar();
		_progressBar->setProgress((float)cooldownPassed / (float)CLEAN_INTERVAL_SECONDS);
		_progressBar->addTween(ProgressBar::TweenProgress(1.0f), _cooldownLeft * 1000, 1, false, 1000);

		createCooldownTextfield();
		_cooldown->setVisible(true);
		spTween tween = addTween(CleanerElement::TweenCooldown(0), _cooldownLeft * 1000);
		tween->setDoneCallback(CLOSURE(this, &CleanerElement::onTimePassed));
	}
}

void CleanerElement::createBackground() {
	_background = new Sprite;
	_background->setAnchor(0.5f, 0.5f);
	_background->setResAnim(tilesResources.getResAnim("plate"));
	setSpriteScaleBySize(_background, getSize());
	_background->setPosition(getWidth() / 2.0f, getHeight() / 2.0f);
	_background->attachTo(this);
	_background->setPriority(-1);
}

void CleanerElement::createButton() {
	if (!_button) {
		_button = new Button();
		_button->setResAnim(tilesResources.getResAnim("cloud"));
		_button->setAnchor(Vector2(0.5f, 0.5f));
		_button->setPosition(getWidth() / 2.0f, getHeight() / 2.0f);
		_button->setScale(getWidth() / _button->getWidth());
		_button->addEventListener(TouchEvent::TOUCH_DOWN, CLOSURE(this, &CleanerElement::onCleanClicked));
		addChild(_button);
	}
}

void CleanerElement::createProgressbar() {
	if (!_progressBar) {
		_progressBar = initActor(new ProgressBar,
			arg_attachTo = this,
			arg_anchor = Vector2(0.5f, 0.5f),
			arg_position = Vector2(getWidth() / 2.0f, getHeight() / 2.0f),
			arg_resAnim = tilesResources.getResAnim("cloud"));
		_progressBar->setDirection(_progressBar->dir_radial_cw);
		_progressBar->setScale(getWidth() / _progressBar->getWidth());
		_progressBar->setProgress((float)_cooldownLeft / (float)_cooldownMax);
		_progressBar->setTouchEnabled(false);
		_progressBar->setTouchChildrenEnabled(false);
	}
}

void CleanerElement::createCooldownTextfield() {
	if (!_cooldown) {
		_cooldown = new TextField();
		_cooldown->setStyle(createTextStyle(gameResources.getResFont("nobile_bold")->getFont(), Color(0, 0, 0, 255), false, TextStyle::HALIGN_CENTER, TextStyle::VALIGN_MIDDLE));
		_cooldown->setSize(getSize()); // todo probably set this to wtf
		_cooldown->setTouchEnabled(false);
		_cooldown->setTouchChildrenEnabled(false);
		addChild(_cooldown);
	}
}


int CleanerElement::getCooldown() const {
	return _cooldownLeft;
}

void CleanerElement::setCooldown(int cooldown) {
	if (_cooldownLeft == cooldown) {
		return;
	}

	_cooldownLeft = cooldown;
	_cooldown->setText(FlashUtils::CMath::getFormattedCooldown(_cooldownLeft));
}

void CleanerElement::onCleanClicked(Event *ev) {
	spProcessMaster master = new ProcessMaster();
	master->addProcess(new CleanAnimalsProcess(safeCast<AnimalFarmField*>(this->getParent()), _button, ev));
	master->setCompleteCallback(CLOSURE(this, &CleanerElement::onTimePassed));
	master->start(this);
	_button->setTouchEnabled(false);
	_button->setVisible(false);
}

void CleanerElement::onTimePassed(Event *ev) {
	revalidate();
}