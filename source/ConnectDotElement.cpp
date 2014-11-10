#include "ConnectDotElement.h"
#include "SharedResources.h"

ConnectDotElement::ConnectDotElement(string number) {
	setAnchor(0.5f, 0.5f);
	setSize(DOT_SIZE_X, DOT_SIZE_Y);

	createDotSprite();
	createText(number);
}

void ConnectDotElement::setBaseScale(Vector2 baseScale) {
	_baseScale = baseScale;
}

void ConnectDotElement::playAnimation() {
	_dotSprite->addTween(Sprite::TweenScale(_dotSprite->getScale() * 1.2f), 500, -1, true)->setName("animate_dat_dot");
}

void ConnectDotElement::stopAnimation() {
	_dotSprite->removeTweensByName("animate_dat_dot");
	_dotSprite->setScale(0.8f * DOT_SIZE_X / _dotSprite->getWidth());
}

void ConnectDotElement::createDotSprite() {
	_dotSprite = new Sprite();
	_dotSprite->setPriority(0);
	setDotSprite();
	addChild(_dotSprite);
}

void ConnectDotElement::setDotSprite() {
	_dotSprite->setResAnim(animalsResources.getResAnim("goat"));
	setAlpha(255);
	_dotSprite->setAnchor(0.5f, 0.5f);
	_dotSprite->setPosition(getWidth() / 2, getHeight() / 2);
	
	float scaleInParent = 0.8f;
	float dotScale = 1.0f;
	dotScale = scaleInParent * DOT_SIZE_X / _dotSprite->getWidth();
	_dotSprite->setScale(dotScale);
}

void ConnectDotElement::createText(string number) {
	spTextActor numberTextField = createTextfield(number, true, 0, false);
	numberTextField->setStyle(createTextStyle(gameResources.getResFont("halycon")->getFont(), Color(144, 217, 88), false, TextStyle::HALIGN_CENTER, TextStyle::VALIGN_MIDDLE));
	numberTextField->setFontSize2Scale(2 * (int)getRoot()->getWidth() / 320);
	numberTextField->setSize(getWidth() * 0.8f, getHeight() * 0.8f);
	numberTextField->setPosition(getWidth() / 2, getHeight() / 2);
	numberTextField->setPriority(3);
	numberTextField->attachTo(this);
}