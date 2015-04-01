#include "ProgressBarBox9.h"
#include "SharedResources.h"

ProgressBarBox9::ProgressBarBox9(Vector2 size) : _progress(1.0f) {
	setTouchEnabled(false);
	setAnchor(0.0f, 0.0f);
	setSize(size.x, size.y);

	createBackground();
	createMask();
	createMaskedSprite();
	createForeground();
}

void ProgressBarBox9::setProgress(float value) {
	if (_progress == value) {
		return;
	}

	_progress = scalar::clamp(value, 0.0f, 1.0f);
	_mask->setX(-(1 - _progress) * _mask->getDerivedWidth());
}

float ProgressBarBox9::getProgress() const {
	return _progress;
}

void ProgressBarBox9::createBackground() {
	spBox9Sprite timerBackground = new Box9Sprite;
	timerBackground->setResAnim(gameResources.getResAnim("progress_background_9"));
	timerBackground->setAnchor(Vector2(0.0f, 0.0f));
	timerBackground->setPosition(0.0f, 0.0f);
	timerBackground->setSize(getSize());
	//timerBackground->setGuides(33, 93, 33, 93);
	timerBackground->setGuides(7, 9, 6, 8);
	timerBackground->attachTo(this);
	timerBackground->setPriority(1);
}

void ProgressBarBox9::createForeground() {
	spBox9Sprite timerForeground = new Box9Sprite;
	timerForeground->setResAnim(gameResources.getResAnim("progress_background_9"));
	timerForeground->setAnchor(Vector2(0.0f, 0.0f));
	timerForeground->setPosition(0.0f, 0.0f);
	timerForeground->setSize(getSize());
	//timerForeground->setGuides(33, 93, 33, 93);
	timerForeground->setGuides(7, 9, 6, 8);
	timerForeground->attachTo(_masked);
	timerForeground->setColor(Color(144, 217, 88));
}

void ProgressBarBox9::createMask() {
	_mask = new Sprite(); 
	_mask->setAnchor(Vector2(0.0f, 0.5f));
	_mask->setTouchEnabled(false);
	_mask->setVisible(false);
	_mask->setResAnim(gameResources.getResAnim("quad_100"));//"quad_40"));
	_mask->setScale(getWidth() / _mask->getWidth() , getHeight() / _mask->getHeight());
	_mask->setPosition(0.0f, getHeight() / 2);
	addChild(_mask);
}

void ProgressBarBox9::createMaskedSprite() {
	_masked = new MaskedSprite();
	_masked->setPriority(2);
	addChild(_masked);

	_masked->setMask(_mask);
}