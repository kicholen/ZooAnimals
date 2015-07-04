#include "ConnectDotElement.h"
#include "SharedResources.h"

ConnectDotElement::ConnectDotElement() {
	setAnchor(0.5f, 0.5f);
	setSize(DOT_SIZE_X, DOT_SIZE_Y);
	createDotSprite();
}

void ConnectDotElement::addText(std::string number) {
	float factor = number.length() == 1 ? 2.0f : 1.0f;
	spTextActor numberTextField = createTextfield(number, true, false);
	numberTextField->setStyle(createTextStyle(gameResources.getResFont("nobile_bold")->getFont(), Color(144, 217, 88), false, TextStyle::HALIGN_CENTER, TextStyle::VALIGN_MIDDLE));
	numberTextField->setFontSize2Scale(15);
	numberTextField->setSize(_dotSprite->getDerivedWidth() * 0.8f, _dotSprite->getDerivedHeight() * 0.8f);
	float textWidth = numberTextField->getTextRect().getWidth() * factor;
	numberTextField->setFontSize2Scale(int(numberTextField->getWidth() / textWidth * numberTextField->getFontSize2Scale()));
	numberTextField->setAnchor(0.5f, 0.5f);
	numberTextField->setPosition(getWidth() / 2, getHeight() / 2);
	numberTextField->setPriority(3);
	numberTextField->attachTo(this);
}

void ConnectDotElement::playAnimation() {
	_baseScaleX = getScaleX();
	addTween(Sprite::TweenScale(getScale() * 1.2f), 500, -1, true)->setName("animate_dat_dot");
}

void ConnectDotElement::stopAnimation() {
	removeTweensByName("animate_dat_dot");
	setScale(_baseScaleX);
}

void ConnectDotElement::createDotSprite() {
	_dotSprite = new Sprite();
	_dotSprite->setPriority(0);
	setDotSprite();
	addChild(_dotSprite);
}

void ConnectDotElement::setDotSprite() {
	_dotSprite->setResAnim(gameResources.getResAnim("circle_border"));
	setAlpha(255);
	_dotSprite->setAnchor(0.5f, 0.5f);
	_dotSprite->setPosition(getWidth() / 2, getHeight() / 2);
	
	float scaleInParent = 0.8f;
	float dotScale = 1.0f;
	dotScale = scaleInParent * DOT_SIZE_X / _dotSprite->getWidth();
	_dotSprite->setScale(dotScale);
}

