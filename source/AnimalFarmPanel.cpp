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

	_backgroundSprite = initActor(new ColorRectSprite,
		arg_attachTo = this,
		arg_color = Color::Chocolate,
		arg_anchor = Vector2(1.0f, 1.0f),
		arg_size = getSize(),
		arg_position = getSize());

	_expandButton = initActor(new TweenButton,
		arg_resAnim = gameResources.getResAnim("back_button"),
		arg_attachTo = this,
		arg_anchor = Vector2(0.5f, 0.5f),
		arg_position = getSize() / 2);
	setActorScaleBySize(_expandButton, getSize());
	_expandButton->addEventListener(TouchEvent::CLICK, CLOSURE(this, &AnimalFarmPanel::handleClick));

	_playButton = initActor(new TweenButton,
		arg_resAnim = gameResources.getResAnim("back_button"),
		arg_attachTo = this,
		arg_alpha = 0,
		arg_anchor = Vector2(0.5f, 0.5f),
		arg_position = -(_sizeExpanded / 2 - getSize()));
	setActorScaleBySize(_playButton, _sizeExpanded);
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
		_playButton->removeEventListener(TouchEvent::CLICK, CLOSURE(this, &AnimalFarmPanel::handleClick));

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

void AnimalFarmPanel::switchViewToExpanded() {
	_backgroundSprite->addTween(TweenScaleX(_sizeExpanded.x / _backgroundSprite->getWidth()), 500, 1, false, 0, Tween::EASE::ease_inOutBounce);
	_backgroundSprite->addTween(TweenScaleY(_sizeExpanded.y / _backgroundSprite->getHeight()), 500, 1, false, 0, Tween::EASE::ease_inOutBounce)->addDoneCallback(CLOSURE(this, &AnimalFarmPanel::onBackgroundAnimationFinished));
}

void AnimalFarmPanel::switchViewToNormal() {
	_backgroundSprite->addTween(TweenScaleX(1.0f), 500, 1, false, 0, Tween::EASE::ease_inOutBounce);
	_backgroundSprite->addTween(TweenScaleY(1.0f), 500, 1, false, 0, Tween::EASE::ease_inOutBounce)->addDoneCallback(CLOSURE(this, &AnimalFarmPanel::onBackgroundAnimationFinished));
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
	_playButton->addTween(TweenAlpha(255), 500)->addDoneCallback(CLOSURE(this, &AnimalFarmPanel::onViewSwitched));
	_playButton->addEventListener(TouchEvent::CLICK, CLOSURE(this, &AnimalFarmPanel::handleClick));
}

void AnimalFarmPanel::showNormalView() {
	_expandButton->addTween(TweenAlpha(255), 500)->addDoneCallback(CLOSURE(this, &AnimalFarmPanel::onViewSwitched));
	_expandButton->addEventListener(TouchEvent::CLICK, CLOSURE(this, &AnimalFarmPanel::handleClick));
}

void AnimalFarmPanel::hideNormalView() {
	_expandButton->addTween(TweenAlpha(0), 500);
}

void AnimalFarmPanel::hideExpandedView() {
	_playButton->addTween(TweenAlpha(0), 500);
}

void AnimalFarmPanel::onViewSwitched(Event *event) {
	_state = _state == afpExpanded ? afpNormal : afpExpanded;
	_isAnimating = false;
}