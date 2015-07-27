#include "TooltipElement.h"
#include "SharedResources.h"
#include "LanguageManager.h"

/*
*	If lockitId != 0 text will be added, otherwise sprite
*/
TooltipElement::TooltipElement(const Vector2& size, const std::string& background, const std::string& spriteName, int lockitId) {
	setSize(size);
	setAnchor(0.0f, 1.0f);
	setData(background, spriteName, lockitId);
}

TooltipElement::~TooltipElement() {

}

void TooltipElement::showForTime(int ms) {
	show();
	hide(true, ms);
}

void TooltipElement::show() {
	Vector2 currentScale = getScale();

	setScale(0.0f);
	addTween(TweenScale(currentScale), 500, 1, false, 0, Tween::ease_outBounce);
}

void TooltipElement::hide(bool shouldDetach, int delay) {
	if (shouldDetach) {
		addTween(TweenScale(0.0f), 300, 1, false, delay, Tween::ease_outBounce)->addDoneCallback(CLOSURE(this, &TooltipElement::onTweenEnded));
	}
	else {
		addTween(TweenScale(0.0f), 300, 1, false, delay, Tween::ease_outBounce);
	}
}

void TooltipElement::setText(int lockit) {
	createTextfieldIfNeeded(lockit);
}

void TooltipElement::setData(const std::string& background, const std::string& spriteName, int lockitId) {
	createBackground(background);
	bool wasTextFieldAdded = createTextfieldIfNeeded(lockitId);
	
	if (!wasTextFieldAdded) {
		createSprite(spriteName);
	}
}

// todo set it to box9sprite
void TooltipElement::createBackground(const std::string& background) {
	_background = initActor(new Sprite,
		arg_anchor = Vector2(0.5f, 0.5f),
		arg_attachTo = this,
		arg_priority = -20,
		arg_animFrame = gameResources.getResAnim(background),
		arg_position = getSize() / 2.0f);
	_background->setScaleX(getWidth() / _background->getWidth());
	_background->setScaleY(getHeight() / _background->getHeight());
}

bool TooltipElement::createTextfieldIfNeeded(int lockitId) {
	if (lockitId == 0) {
		return false;
	}

	if (!_textField) {
		TextStyle style;
		style.font = gameResources.getResFont("nobile_bold")->getFont();
		style.vAlign = TextStyle::VALIGN_MIDDLE;
		style.hAlign = TextStyle::HALIGN_CENTER;
		style.multiline = true;
		style.color = Color(35, 145, 245);
	

		_textField = initActor(new TextField,
			arg_style = style,
			arg_hAlign = TextStyle::HALIGN_MIDDLE,
			arg_vAlign = TextStyle::VALIGN_MIDDLE,
			arg_attachTo = this,
			arg_width = _background->getDerivedWidth(),
			arg_height = _background->getDerivedHeight(),
			arg_input = false,
			arg_priority = 20,
			arg_text = LanguageManager::instance.getText(lockitId));
	}
	else {
		_textField->setText(LanguageManager::instance.getText(lockitId));
	}

	setTextFieldRectToSize(_textField, _background->getDerivedSize());

	return true;
}

void TooltipElement::createSprite(const std::string& spriteName) {
	_sprite = initActor(new Sprite,
		arg_anchor = Vector2(0.5f, 0.5f),
		arg_attachTo = this,
		arg_priority = 20,
		arg_animFrame = gameResources.getResAnim(spriteName),
		arg_position = _background->getSize() / 2.0f);
	_sprite->setScale(getActorScaleBySize(_sprite, _background->getSize()));
}

void TooltipElement::onTweenEnded(Event *ev) {
	detach();
}
