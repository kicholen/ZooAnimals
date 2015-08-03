#include "EquationFragment.h"
#include "SharedResources.h"

int EquationFragment::_textSize = 0;

EquationFragment::EquationFragment(Vector2& size, int count, bool isSign, const std::string& sign) {
	setSize(size);
	_count = count;
	_state = efNumericView;
	_isSign = isSign;
	if (isSign) {
		_sign = sign;
	}
	setTextfieldSize();
	
	reset(count, "");
}

EquationFragment::~EquationFragment() {
	_sign.clear();
}

void EquationFragment::switchToAnotherView(const std::string& assetName) {
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

void EquationFragment::switchToDisplayView(const std::string& assetName) {
	_state = efDisplayView;
	reset(_count, assetName);
}

void EquationFragment::switchToNumericView() {
	_state = efNumericView;
	reset(_count, "");
}

void EquationFragment::reset(int count, const std::string& assetName) {
	removeChildren();
	_count = count;
	if (_state == efNumericView) {
		addNumberTextField(count);
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

spSprite EquationFragment::createContainerElement(const std::string& assetName) {
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

void EquationFragment::setTextfieldSize() {
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
	}
}