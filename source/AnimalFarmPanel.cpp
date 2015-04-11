#include "AnimalFarmPanel.h"
#include "SharedResources.h"
#include "FlashUtils.h"
#include <algorithm>
#include <string>

AnimalFarmPanel::AnimalFarmPanel(const Vector2& size, const Vector2& sizeExpanded) {
	setSize(size);
	_sizeExpanded = sizeExpanded;
}

AnimalFarmPanel::~AnimalFarmPanel() {

}

void AnimalFarmPanel::setData(spAnimalModel model) {
	_model = model;
	_state = afpNormal;
	_isAnimating = false;

	_backgroundSprite = initActor(new Box9Sprite,
		arg_attachTo = this,
		arg_anchor = Vector2(1.0f, 1.0f),
		arg_position = getSize(),
		arg_resAnim = tilesResources.getResAnim("informationTableBox9"));
	_backgroundSprite->setSize(getSize() * 1.1f);
	_backgroundSprite->setGuides(7, 58, 7, 22);
	_backgroundSprite->setVerticalMode(Box9Sprite::StretchMode(Box9Sprite::STRETCHING));
	_backgroundSprite->setHorizontalMode(Box9Sprite::StretchMode(Box9Sprite::STRETCHING));

	string animalNameUpperCase = model->animalName().c_str();
	transform(animalNameUpperCase.begin(), animalNameUpperCase.end(), animalNameUpperCase.begin(), ::toupper);
	_animalNameTextfield = createTextfield(animalNameUpperCase, false, false);
	_animalNameTextfield->setStyle(createTextStyle(gameResources.getResFont("nobile_bold")->getFont(), Color(100, 140, 50), false, TextStyle::HALIGN_CENTER, TextStyle::VALIGN_MIDDLE));
	_animalNameTextfield->setFontSize2Scale(7 * (int)getRoot()->getWidth() / 320);
	_animalNameTextfield->setSize(getWidth() * 0.8f, getHeight() * 0.8f);
	_animalNameTextfield->setAnchor(0.5f, 0.5f);
	_animalNameTextfield->setPosition(getWidth() / 2, getHeight() / 2);
	_animalNameTextfield->setPriority(3);
	_animalNameTextfield->setTouchEnabled(false);
	_animalNameTextfield->attachTo(this);

	_expandButton = initActor(new TweenButton,
		arg_resAnim = gameResources.getResAnim("greenArrow"),
		arg_attachTo = this,
		arg_anchor = Vector2(0.5f, 0.5f),
		arg_rotation = FlashUtils::CMath::DegToRad(225));
	setActorScaleBySize(_expandButton, getSize() * 0.5f);
	_expandButton->setExtendedClickArea(30);
	_expanButtonBasePoition = Vector2(_backgroundSprite->getX() - _backgroundSprite->getDerivedWidth() + _expandButton->getDerivedWidth() / 2, _backgroundSprite->getY() - _backgroundSprite->getDerivedHeight() + _expandButton->getDerivedHeight() / 2);
	_expanButtonExpandedPoition = Vector2(_backgroundSprite->getX() - _sizeExpanded.x * 1.1f + _expandButton->getDerivedWidth() / 2, _backgroundSprite->getY() - _sizeExpanded.y * 1.1f + _expandButton->getDerivedHeight() / 2);
	_expandButton->setPosition(_expanButtonBasePoition);
	_expandButton->addEventListener(TouchEvent::CLICK, CLOSURE(this, &AnimalFarmPanel::handleClick));

	_expandedElement = new FarmServiceElement(_sizeExpanded);
	_expandedElement->attachTo(this);
	_expandedElement->setAlpha(0);
	_expandedElement->setPosition(-(_sizeExpanded / 2 - getSize()));
	_expandedElement->setAnchor(0.5f, 0.5f);
	_expandedElement->setData(model);
}

void AnimalFarmPanel::closeExpandedViewIfOpen() {
	if (_state == afpExpanded && !_isAnimating) {
		_isAnimating = true;
		_expandButton->removeEventListener(TouchEvent::CLICK, CLOSURE(this, &AnimalFarmPanel::handleClick));
		hideExpandedView();
		switchViewToNormal();
	}
}

void AnimalFarmPanel::handleClick(Event *event) {
	if (!_isAnimating) {
		_isAnimating = true;
		_expandButton->removeEventListener(TouchEvent::CLICK, CLOSURE(this, &AnimalFarmPanel::handleClick));
		_expandedElement->removeEventListener(FarmServiceElement::FarmServiceElementEvent::PLAY_GAMES, CLOSURE(this, &AnimalFarmPanel::onGameChosen));

		if (_state == afpNormal) {
			hideNormalView();
			switchViewToExpanded();
		}
		else if (_state == afpExpanded) {
			hideExpandedView();
			switchViewToNormal();
		}
	}

	event->stopImmediatePropagation();
}

void AnimalFarmPanel::onGameChosen(Event *event) {
	dispatchEvent(event);
	_expandedElement->removeEventListener(FarmServiceElement::FarmServiceElementEvent::PLAY_GAMES, CLOSURE(this, &AnimalFarmPanel::onGameChosen));
}

void AnimalFarmPanel::switchViewToExpanded() {
	_animalNameTextfield->addTween(TweenAlpha(0), 500);
	_backgroundSprite->addTween(TweenWidth(_sizeExpanded.x * 1.1f), 500, 1, false, 0);
	_backgroundSprite->addTween(TweenHeight(_sizeExpanded.y * 1.1f), 500, 1, false, 0)->addDoneCallback(CLOSURE(this, &AnimalFarmPanel::onBackgroundAnimationFinished));
}

void AnimalFarmPanel::switchViewToNormal() {
	_animalNameTextfield->addTween(TweenAlpha(255), 500);
	_backgroundSprite->addTween(TweenWidth(getWidth() * 1.1f), 500, 1, false, 0);
	_expandButton->addTween(TweenRotation(FlashUtils::CMath::DegToRad(225)), 500);
	_expandButton->addTween(TweenPosition(_expanButtonBasePoition), 500)->addDoneCallback(CLOSURE(this, &AnimalFarmPanel::onViewSwitched));
	_backgroundSprite->addTween(TweenHeight(getHeight() * 1.1f), 500, 1, false, 0)->addDoneCallback(CLOSURE(this, &AnimalFarmPanel::onBackgroundAnimationFinished));
}

void AnimalFarmPanel::onBackgroundAnimationFinished(Event *event) {
	if (_state == afpNormal) {
		showExpandedView();
	}
	else {
		showNormalView();
	}
}

void AnimalFarmPanel::showExpandedView() {
	_expandButton->setResAnim(gameResources.getResAnim("redArrow"));
	_expandedElement->addTween(TweenAlpha(255), 500)->addDoneCallback(CLOSURE(this, &AnimalFarmPanel::onViewSwitched));
	_expandedElement->addEventListener(FarmServiceElement::FarmServiceElementEvent::PLAY_GAMES, CLOSURE(this, &AnimalFarmPanel::onGameChosen));
}

void AnimalFarmPanel::showNormalView() {
	_expandButton->setResAnim(gameResources.getResAnim("greenArrow"));
	_expandButton->addEventListener(TouchEvent::CLICK, CLOSURE(this, &AnimalFarmPanel::handleClick));
}

void AnimalFarmPanel::hideNormalView() {
	_expandButton->addTween(TweenRotation(FlashUtils::CMath::DegToRad(45)), 500);
	_expandButton->addTween(TweenPosition(_expanButtonExpandedPoition), 500);
}

void AnimalFarmPanel::hideExpandedView() {
	_expandedElement->addTween(TweenAlpha(0), 500);
}

void AnimalFarmPanel::onViewSwitched(Event *event) {
	_state = _state == afpExpanded ? afpNormal : afpExpanded;
	_isAnimating = false;
}