#include "TooltipElement.h"
#include "SharedResources.h"
#include "LanguageManager.h"

/*
*	If lockitId != 0 text will be added, otherwise sprite
*/
TooltipElement::TooltipElement(const Vector2& size, const string& background, const string& spriteName, int lockitId) {
	setSize(size);

	setData(background, spriteName, lockitId);
}

TooltipElement::~TooltipElement() {

}

void TooltipElement::show() {
	addTween(TweenScale(1.0f), 300, 1, false, 0, Tween::ease_outBounce);
}

void TooltipElement::hide(bool shouldDetach) {
	if (shouldDetach) {
		addTween(TweenScale(0.0f), 300, 1, false, 0, Tween::ease_outBounce)->addDoneCallback(CLOSURE(this, &TooltipElement::onTweenEnded));
	}
	else {
		addTween(TweenScale(0.0f), 300, 1, false, 0, Tween::ease_outBounce);
	}
}

void TooltipElement::setData(const string& background, const string& spriteName, int lockitId) {
	bool wasTextFieldAdded = createTextfieldIfNeeded(lockitId);
	
	if (!wasTextFieldAdded) {
		createSprite(spriteName);
	}
}

void TooltipElement::createBackground(const string& background) {
	_background = initActor(new Sprite,
		arg_anchor = Vector2(0.5f, 0.5f),
		arg_attachTo = this,
		arg_priority = -20,
		arg_animFrame = gameResources.getResAnim(background),
		arg_position = getSize() / 2.0f);
}

bool TooltipElement::createTextfieldIfNeeded(int lockitId) {
	if (lockitId != 0) {
		return false;
	}

	TextStyle style;
	style.font = gameResources.getResFont("nobile_bold")->getFont();
	style.vAlign = TextStyle::VALIGN_TOP;

	_textField = initActor(new TextField,
		arg_style = style,
		arg_hAlign = TextStyle::HALIGN_MIDDLE,
		arg_vAlign = TextStyle::VALIGN_MIDDLE,
		arg_attachTo = this,
		arg_width = getWidth(),
		arg_height = getHeight(),
		arg_input = false,
		arg_priority = 20,
		arg_text = LanguageManager::instance.getText(lockitId));
}

void TooltipElement::createSprite(const string& spriteName) {
	_sprite = initActor(new Sprite,
		arg_anchor = Vector2(0.5f, 0.5f),
		arg_attachTo = this,
		arg_priority = 20,
		arg_animFrame = gameResources.getResAnim(spriteName),
		arg_position = getSize() / 2.0f);
	_sprite->setScale(getActorScaleBySize(_sprite, getSize()));
}

void TooltipElement::onTweenEnded(Event *ev) {
	detach();
}
