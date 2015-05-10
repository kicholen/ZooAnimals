#include "EquationFragment.h"
#include "SharedResources.h"

int EquationFragment::_textSize = 0;
Vector2 EquationFragment::_backgroundSize = Vector2(0.0f, 0.0f);

EquationFragment::EquationFragment(Vector2& size, int count, bool isSign, const string& sign) {
	setSize(size);
	_count = count;
	_state = efNumericView;
	_isSign = isSign;
	if (isSign) {
		_sign = sign;
	}
	setTextFieldAndBackgroundSize();
	
	reset(count, "");
}

EquationFragment::~EquationFragment() {
	_cardBackground->releaseRef();
	_sign.clear();
}

void EquationFragment::switchToAnotherView(const string& assetName) {
	if (_isSign) {
		return;
	}

	if (_state == efNumericView) {
		switchToDisplayView(assetName);
	}
	else {
		switchToNumericView();
	}
}

void EquationFragment::switchToDisplayView(const string& assetName) {
	_state = efDisplayView;
	reset(_count, assetName);
}

void EquationFragment::switchToNumericView() {
	_state = efNumericView;
	reset(_count, "");
}

void EquationFragment::reset(int count, const string& assetName) {
	removeChildren();
	_count = count;
	createBackground();
	if (_state == efNumericView) {
		addNumberTextField(count);
		if (_cardBackground->getScaleX() != _backgroundSize.x / _cardBackground->getWidth()) {
			_cardBackground->addTween(TweenScaleX(_backgroundSize.x / _cardBackground->getWidth()), 500, 1, false, 0, Tween::ease_inBack);
			_cardBackground->addTween(TweenScaleY(_backgroundSize.y / _cardBackground->getHeight()), 500, 1, false, 0, Tween::ease_inBack);
		}
	}
	else {
		createContainerIfDoesntExist();
		_container->removeChildren();
		VectorArray<spActor> dupaArray;
		dupaArray._vector.resize(0);
		for (int i = 0; i < count; i++) {
			dupaArray.push(createContainerElement(assetName));
		}
		_container->addChildren(dupaArray);
		_container->hideContainerElements();
		_cardBackground->addTween(TweenScaleX(getWidth() / _cardBackground->getWidth()), 500, 1, false, 0, Tween::ease_inBack);
		_cardBackground->addTween(TweenScaleY(getHeight() / _cardBackground->getHeight()), 500, 1, false, 0, Tween::ease_inBack)->setDoneCallback(CLOSURE(this, &EquationFragment::onBackgroundTweenEnded));
	}
}

void EquationFragment::onBackgroundTweenEnded(Event *event) {
	_container->showContainerElements(true);
}

void EquationFragment::show(bool shouldAnimate) {
	setVisible(true);
}

void EquationFragment::hide(bool shouldAnimate) {
	setVisible(false);
}

spSprite EquationFragment::createContainerElement(const string& assetName) {
	spSprite element = new Sprite();
	element->setResAnim(gameResources.getResAnim(assetName));
	return element;
}

void EquationFragment::createContainerIfDoesntExist() {
	if (!_container) {
		_container = new AnimatableElementContainer(getSize());
		_container->setAnimationType(aecScale);
	}
	_container->attachTo(this);
}

void EquationFragment::addNumberTextField(int count) {
	spTextActor textfield = createTextfield(_isSign ? _sign : FlashUtils::CMath::intToString(count), true, false);
	TextStyle style = createTextStyle(gameResources.getResFont("nobile_bold")->getFont(), Color(144, 217, 88), true, TextStyle::HALIGN_CENTER, TextStyle::VALIGN_MIDDLE);
	textfield->setStyle(style);
	textfield->setSize(getSize().x * 0.5f, getSize().y * 0.5f);
	textfield->setFontSize2Scale(EquationFragment::_textSize);
	textfield->setPriority(3);
	textfield->setAnchor(0.5f, 0.5f);
	textfield->setPosition(getWidth() / 2, getHeight() / 2);
	textfield->attachTo(this);
}

void EquationFragment::createBackground() {
	if (!_cardBackground) {
		_cardBackground = new ColorRectSprite;
		_cardBackground->setAnchor(0.5f, 0.5f);
		_cardBackground->setColor(Color(200, 120, 200));
		_cardBackground->setSize(EquationFragment::_backgroundSize);
		_cardBackground->setPosition(getWidth() / 2, getHeight() / 2);
		_cardBackground->setPriority(-11);
		_cardBackground->addRef();
	}
	_cardBackground->attachTo(this);
}

void EquationFragment::setTextFieldAndBackgroundSize() {
	if (EquationFragment::_textSize == 0) {
		spTextActor textfield = createTextfield("99", true, false);
		TextStyle style = createTextStyle(gameResources.getResFont("nobile_bold")->getFont(), Color(144, 217, 88), true, TextStyle::HALIGN_CENTER, TextStyle::VALIGN_MIDDLE);
		textfield->setStyle(style);
		textfield->setSize(getSize().x * 0.5f, getSize().y * 0.5f);
		textfield->setFontSize2Scale(15);
		if (textfield->getWidth() < textfield->getHeight()) {
			EquationFragment::_textSize = int(getSize().x * 0.3f / textfield->getTextRect().getWidth() * textfield->getFontSize2Scale());
		}
		else {
			EquationFragment::_textSize = int(getSize().y * 0.3f / textfield->getTextRect().getHeight() * textfield->getFontSize2Scale());
		}
		textfield->setFontSize2Scale(EquationFragment::_textSize);
		setBackgroundSize(textfield->getTextRect().getWidth(), textfield->getTextRect().getHeight());
	}
}

void EquationFragment::setBackgroundSize(int width, int height) {
	EquationFragment::_backgroundSize.x = width * 1.2f;
	EquationFragment::_backgroundSize.y = height * 1.2f;
}