#include "ItemPickerElement.h"
#include "SharedResources.h"

ItemPickerElement::ItemPickerElement(const Vector2& size) {
	setSize(size);
	setTouchEnabled(false);
	_state = ipeNotSelected;
}

ItemPickerElement::~ItemPickerElement() {

}

void ItemPickerElement::setData(VectorArray<spActor>& children) {
	createContainerIfDoesntExist();

	for (int i = children.length() - 1; i >= 0; i--) {
		children[i]->addEventListener(TouchEvent::CLICK, CLOSURE(this, &ItemPickerElement::onChildClicked));
	}

	_animatableContainer->addChildren(children);
	_animatableContainer->hideContainerElements();
	_animatableContainer->showContainerElements(true);
}

void ItemPickerElement::createContainerIfDoesntExist() {
	if (!_animatableContainer) {
		_animatableContainer = new AnimatableElementContainer(Vector2(getWidth(), getHeight() * CONTAINER_HEIGHT));
		_animatableContainer->setPriority(10);
		_animatableContainer->setAnimationType(aecScale);
	}
	_animatableContainer->attachTo(this);
}

void ItemPickerElement::onChildClicked(Event *ev) {
	_state = ipeAnimating;

	drawSelectionOnChild(safeSpCast<Sprite>(ev->target));

	ItemPickerElement::ItemPickerElementEvent itemPickedEvent(ItemPickerElementEvent::ITEM_CHOOSEN, ev->target->getName());
	dispatchEvent(&itemPickedEvent);
}

void ItemPickerElement::drawSelectionOnChild(spSprite sprite) {
	bool wasJustCreated = createSelectionIfDoesntExist();

	if (wasJustCreated) {
		_selectionSprite->setAlpha(char(0));
		_selectionSprite->addTween(Sprite::TweenAlpha(255), 200, 1, false, 0, Tween::ease_inCubic);
		_selectionSprite->setPosition(sprite->getPosition() + _animatableContainer->getPosition());
	}
	else {
		_selectionSprite->setAlpha(255);
		_selectionSprite->addTween(Sprite::TweenPosition(sprite->getPosition() + _animatableContainer->getPosition()), 400, 1, false, 0, Tween::ease_outBack);
	}
}

// it must be square, visual reasons
bool ItemPickerElement::createSelectionIfDoesntExist() {
	if (!_selectionSprite) {
		_selectionSprite = new Box9Sprite();
		_selectionSprite->setHorizontalMode(Box9Sprite::STRETCHING);
		_selectionSprite->setVerticalMode(Box9Sprite::STRETCHING);
		_selectionSprite->setResAnim(gameResources.getResAnim("greenProgressBarHorizontal"));
		_selectionSprite->setAnchor(Vector2(0.5f, 0.5f));
		_selectionSprite->setSize(Vector2(_animatableContainer->getHeight(), _animatableContainer->getHeight()));
		_selectionSprite->setGuides(9, 116, 9, 18);
		_selectionSprite->setPriority(0);
		_selectionSprite->attachTo(this);
		return true;
	}
	return false;
}