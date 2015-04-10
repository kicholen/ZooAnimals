#include "AnimalFarmPanel.h"
#include "SharedResources.h"

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
		arg_resAnim = gameResources.getResAnim("progress_bar_box9"));
	_backgroundSprite->setSize(getSize() * 1.1f);
	_backgroundSprite->setGuides(7, 37, 7, 37);
	_backgroundSprite->setVerticalMode(Box9Sprite::StretchMode(Box9Sprite::STRETCHING));
	_backgroundSprite->setHorizontalMode(Box9Sprite::StretchMode(Box9Sprite::STRETCHING));

	_expandButton = initActor(new TweenButton,
		arg_resAnim = gameResources.getResAnim("back_button"),
		arg_attachTo = this,
		arg_anchor = Vector2(0.5f, 0.5f),
		arg_position = getSize() / 2);
	setActorScaleBySize(_expandButton, getSize());
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
	_backgroundSprite->addTween(TweenWidth(_sizeExpanded.x * 1.1f), 500, 1, false, 0);
	_backgroundSprite->addTween(TweenHeight(_sizeExpanded.y * 1.1f), 500, 1, false, 0)->addDoneCallback(CLOSURE(this, &AnimalFarmPanel::onBackgroundAnimationFinished));
}

void AnimalFarmPanel::switchViewToNormal() {
	_backgroundSprite->addTween(TweenWidth(getWidth() * 1.1f), 500, 1, false, 0);
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
	_expandedElement->addTween(TweenAlpha(255), 500)->addDoneCallback(CLOSURE(this, &AnimalFarmPanel::onViewSwitched));
	_expandedElement->addEventListener(FarmServiceElement::FarmServiceElementEvent::PLAY_GAMES, CLOSURE(this, &AnimalFarmPanel::onGameChosen));
}

void AnimalFarmPanel::showNormalView() {
	_expandButton->addTween(TweenAlpha(255), 500)->addDoneCallback(CLOSURE(this, &AnimalFarmPanel::onViewSwitched));
	_expandButton->addEventListener(TouchEvent::CLICK, CLOSURE(this, &AnimalFarmPanel::handleClick));
}

void AnimalFarmPanel::hideNormalView() {
	_expandButton->addTween(TweenAlpha(0), 500);
}

void AnimalFarmPanel::hideExpandedView() {
	_expandedElement->addTween(TweenAlpha(0), 500);
}

void AnimalFarmPanel::onViewSwitched(Event *event) {
	_state = _state == afpExpanded ? afpNormal : afpExpanded;
	_isAnimating = false;
}