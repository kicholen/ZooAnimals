#include "TouchBlock.h"

TouchBlock::TouchBlock() {
	_state = qsHidden;
	_size = getRoot()->getSize();
	setTouchEnabled(false);
}

TouchBlock::TouchBlock(const Vector2& size) {
	_state = qsHidden;
	_size = size;
	setTouchEnabled(false);
}

TouchBlock::~TouchBlock() {

}

void TouchBlock::changeState() {
	if (_touchQuad) {
		_touchQuad->removeTweens(true);
	}

	if (_state == qsHidden) {
		show();
	}
	else if (_state == qsShowed) {
		hide();
	}
}

void TouchBlock::show() {
	_state = qsAnimating;
	createTouchQuadIfDoesntExist();
	unsigned char alpha = 0;
	_touchQuad->setAlpha(alpha);
	alpha = 180;
	_touchQuad->addTween(TweenAlpha(alpha), 400)->addDoneCallback(CLOSURE(this, &TouchBlock::onShowEnded));
}

void TouchBlock::hide() {
	_state = qsAnimating;
	unsigned char alpha = 180;
	_touchQuad->setAlpha(alpha);
	alpha = 0;
	_touchQuad->addTween(TweenAlpha(alpha), 400)->addDoneCallback(CLOSURE(this, &TouchBlock::onHideEnded));
}

void TouchBlock::createTouchQuadIfDoesntExist() {
	if (!_touchQuad) {
		_touchQuad = new ColorRectSprite();
		_touchQuad->setColor(Color::Black);
		unsigned char alpha = 0;
		_touchQuad->setAnchor(0.5f, 0.5f);
		_touchQuad->setAlpha(alpha);
		_touchQuad->setPriority(-2);
		_touchQuad->setSize(_size);
		_touchQuad->attachTo(this);
	}
}

void TouchBlock::onShowEnded(Event *ev) {
	_state = qsShowed;
}

void TouchBlock::onHideEnded(Event *ev) {
	_state = qsHidden;
}