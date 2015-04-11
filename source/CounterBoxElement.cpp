#include "CounterBoxElement.h"
#include "SharedResources.h"
#include "FlashUtils.h"

CounterBoxElement::CounterBoxElement(Vector2 size, int time) : _score(0), _level(1) {
	setTouchEnabled(false);
	setAnchor(0.5f, 0.5f);
	setSize(size.x, size.y);
	_timeLeft = time;

	setPosition(getRoot()->getWidth() / 2, -getHeight() / 2);

	createBackground();
	createScoreTextfield();
	createTimerTextfield();
	createTimerBar();
	createLevelTextfield();

	updateLevel(_level);
	updateTime(0);
	_progressBar->setProgress(0.0f);
	_progressBar->addTween(ProgressBarBox9::Tween9Progress(1.0f), 900);
	_progressBar->addTween(ProgressBarBox9::Tween9Progress(0.0f), _timeLeft * 1000, 1, false, 1000);
}

void CounterBoxElement::show(bool shouldAnimate) {
	setY(-getHeight() / 2);
	addTween(Sprite::TweenY(getDerivedHeight() / 2), 300, 1, false, 0, Tween::ease_outBack);
}

void CounterBoxElement::hide(bool shouldAnimate) {
	addTween(Sprite::TweenY(-getDerivedHeight() / 2), 150, 1, false, 0, Tween::ease_outBack);
}

void CounterBoxElement::updateLevel(int level) {
	_level = level;
	_levelTextfield->setText("Level: " + FlashUtils::CMath::intToString(level));
}

void CounterBoxElement::updateScore(int score) {
	removeTweensByName("score_tween");
	addTween(CounterBoxElement::TweenScore(score), 200)->setName("score_tween");
}

void CounterBoxElement::updateTime(int time) {
	removeTweensByName("timer_tween");
	spTween tween = addTween(CounterBoxElement::TweenTime(time), _timeLeft * 1000);
	tween->setName("timer_tween");
	tween->setDoneCallback(CLOSURE(this, &CounterBoxElement::onTimeEnded));
}

void CounterBoxElement::onTimeEnded(Event *ev) {
	CounterBoxEvent counterBoxEvent(CounterBoxEvent::TIME_END);
	dispatchEvent(&counterBoxEvent);
}

void CounterBoxElement::createBackground() {
	spBox9Sprite cardBackground = new Box9Sprite;
	cardBackground->setAnchor(0.5f, 0.5f);
	cardBackground->setResAnim(gameResources.getResAnim("progress_bar_box9"));
	cardBackground->setSize(getSize());
	cardBackground->setPosition(getWidth() / 2, getHeight() / 2);
	cardBackground->setGuides(7, 37, 7, 37);
	cardBackground->attachTo(this);
	cardBackground->setPriority(-1);
}

void CounterBoxElement::createTimerTextfield() {
	_timerTextfield = createTextfield(FlashUtils::CMath::intToString(_timeLeft), true, false);
	_timerTextfield->setStyle(createTextStyle(gameResources.getResFont("nobile_bold")->getFont(), Color(144, 217, 88), false, TextStyle::HALIGN_RIGHT, TextStyle::VALIGN_MIDDLE));
	_timerTextfield->setFontSize2Scale(20 * (int)getRoot()->getWidth() / 480);
	_timerTextfield->setSize(getWidth() * 0.2f, getHeight() * 0.2f);
	_timerTextfield->setPosition(getWidth() * 0.85f, getHeight() * 0.3f);
	_timerTextfield->setPriority(3);
	_timerTextfield->attachTo(this);
}

void CounterBoxElement::createScoreTextfield() {
	_scoreTextfield = createTextfield("Score: 0", true, false);
	_scoreTextfield->setStyle(createTextStyle(gameResources.getResFont("nobile_bold")->getFont(), Color(217,88,144), false, TextStyle::HALIGN_LEFT, TextStyle::VALIGN_MIDDLE));
	_scoreTextfield->setFontSize2Scale(20 * (int)getRoot()->getWidth() / 480);
	_scoreTextfield->setSize(getWidth() * 0.2f, getHeight() * 0.2f);
	_scoreTextfield->setPosition(getWidth() * 0.8f, getHeight() * 0.7f);
	_scoreTextfield->setPriority(2);
	_scoreTextfield->attachTo(this);
}

void CounterBoxElement::createLevelTextfield() {
	_levelTextfield = createTextfield("Level: 1", true, false);
	_levelTextfield->setStyle(createTextStyle(gameResources.getResFont("nobile_bold")->getFont(), Color(217,88,144), false, TextStyle::HALIGN_LEFT, TextStyle::VALIGN_MIDDLE));
	_levelTextfield->setFontSize2Scale(20 * (int)getRoot()->getWidth() / 480);
	_levelTextfield->setSize(getWidth() * 0.2f, getHeight() * 0.2f);
	_levelTextfield->setPosition(getWidth() * 0.15f, getHeight() * 0.7f);
	_levelTextfield->setPriority(2);
	_levelTextfield->attachTo(this);
}

void CounterBoxElement::createTimerBar() {
	_progressBar = new ProgressBarBox9(Vector2(getWidth() * 0.7f, getHeight() * 0.4f));
	_progressBar->setPosition(getWidth() * 0.1f, getHeight() * 0.1f);
	_progressBar->setPriority(2);
	addChild(_progressBar);
	/*
	UpdateCallback timerUpdateCallback = CLOSURE(this, &CounterBoxElement::timerUpdate);
	_timerBar->setCallbackDoUpdate(timerUpdateCallback);*/
}

void CounterBoxElement::timerUpdate(const UpdateState &us) {
	/*bool canCheckFileAgain = false;

	if (us.time > (_timerMs + 1000)) {
		_timerMs = us.time;
		_timerLimit -= 1;
		_timerBar->setProgress(_timerLimit / 120);
	}*/
}

void CounterBoxElement::setScore(int score) {
	if (_score == score) {
		return;
	}

	_score = score;
	_scoreTextfield->setText("Score: " + FlashUtils::CMath::intToString(_score));
}

int CounterBoxElement::getScore() const {
	return _score;
}

int CounterBoxElement::getTime() const {
	return _timeLeft;
}

void CounterBoxElement::setTime(int time) {
	if (_timeLeft == time) {
		return;
	}

	_timeLeft = time;
	_timerTextfield->setText(FlashUtils::CMath::intToString(_timeLeft));
}