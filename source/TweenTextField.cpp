#include "TweenTextField.h"
#include "FlashUtils.h"

TweenTextField::TweenTextField() {

}

TweenTextField::~TweenTextField() {

}

int TweenTextField::getValue() const {
	return _value;
}

void TweenTextField::setValue(int value) {
	if (_value == value) {
		return;
	}

	_value = value;
	setText(FlashUtils::CMath::intToString(_value));
}