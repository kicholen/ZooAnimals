#include "EquationElement.h"
#include "FlashUtils.h"

EquationElement::EquationElement(Vector2 size, int a, const string& sign, int b) {
	setSize(size);
	setData(a, sign, b);
}

EquationElement::~EquationElement() {

}

void EquationElement::reset(int a, const string& sign, int b) {
	removeChildren();
	setData(a, sign, b);
}

void EquationElement::show() {
	spActor actor = _children._first;
	while (actor) {
		spEquationFragment fragment = safeSpCast<EquationFragment>(actor);
		fragment->show(true);
		actor = actor->getNextSibling();
	}
}

void EquationElement::switchToAnotherView(const string& assetName) {
	spActor actor = _children._first;
	while (actor) {
		spEquationFragment fragment = safeSpCast<EquationFragment>(actor);
		fragment->switchToAnotherView(assetName);
		actor = actor->getNextSibling();
	}
}

void EquationElement::setData(int a, const string& sign, int b) {
	createEquationFragment(Vector2(getWidth() / 4.0f, getHeight() * 0.8f), a, Vector2(getWidth() / 8.0f, getHeight() / 2.0f), "first")->attachTo(this);
	createEquationFragment(Vector2(getWidth() / 10.0f, getHeight() * 0.8f), 0, Vector2(getWidth() / 4.0f + getWidth() / 20.0f, getHeight() / 2.0f), "sign", true, sign)->attachTo(this);
	createEquationFragment(Vector2(getWidth() / 4.0f, getHeight() * 0.8f), b, Vector2(getWidth() / 4.0f + getWidth() / 10.0f + getWidth() / 8.0f, getHeight() / 2), "second")->attachTo(this);
	createEquationFragment(Vector2(getWidth() / 10.0f, getHeight() * 0.8f), 0, Vector2(getWidth() / 4.0f + getWidth() / 10.0f + getWidth() / 4.0f + getWidth() / 20.0f, getHeight() / 2), "sign", true, "=")->attachTo(this);
	
	int result = getCorrectResult(a, sign, b);
	VectorArray<int> checkingVector;
	checkingVector.push(result);
	int firstFalseResult = getDiffrentIntegerInRange(checkingVector, 0, 9);
	checkingVector.push(firstFalseResult);
	int secondFalseResult = getDiffrentIntegerInRange(checkingVector, 0, 9); 
	checkingVector.clear();

	VectorArray<Vector2> positionVector;
	positionVector.ensureCapacity(3);
	float yPosition = getWidth() / 4.0f + getWidth() / 10.0f + getWidth() / 4.0f + getWidth() / 10.0f + getWidth() * 0.15f;
	positionVector.push(Vector2(yPosition, getHeight() / 6.0f));
	positionVector.push(Vector2(yPosition, getHeight() / 6.0f + getHeight() / 3.0f));
	positionVector.push(Vector2(yPosition, getHeight() / 6.0f + getHeight() / 3.0f * 2.0f));
	int randomPosition = CMath::random(0, positionVector.length());
	Vector2 answerSize = Vector2(getWidth() * 0.3f, getHeight() * 0.8f / 3.0f);
	spEquationFragment correctAnswerFragment = createEquationFragment(answerSize, result, positionVector[randomPosition], "first_answer");
	correctAnswerFragment->attachTo(this);
	correctAnswerFragment->addEventListener(TouchEvent::CLICK, CLOSURE(this, &EquationElement::onCorrectAnswerTapped));
	positionVector.splice(randomPosition);

	createEquationFragment(answerSize, firstFalseResult, positionVector[0], "second_answer")->attachTo(this);
	createEquationFragment(answerSize, secondFalseResult, positionVector[1], "third_answer")->attachTo(this);
	positionVector.clear();
}

int EquationElement::getCorrectResult(int a, const string& sign, int b) {
	if (sign == "+") {
		return a + b;
	}
	else if (sign == "-") {
		return a - b;
	}
	else if (sign == "*") {
		return a * b;
	}
	else if (sign == "/") {
		return a / b;
	}
	else {
		log::error("EquationElement:getCorrectResult: Incorrect sign value");
		return 0;
	}
}

int EquationElement::getDiffrentIntegerInRange(VectorArray<int>& integers, int min, int max) {
	if (integers.length() >= max - min) {
		log::error("EquationElement:getDiffrentIntegerInRange: Incorrect parameters.");
	}
	bool flag = true;
	int falseResult = 0;

	while (flag) {
		bool repeated = false;
		falseResult = CMath::random(min, max);
		for (int i = 0; i < integers.length(); i++) {
			if (integers[i] == falseResult) {
				repeated = true;
			}
		}
		if (!repeated) {
			flag = false;
		}
	}

	return falseResult;
}
spEquationFragment EquationElement::createEquationFragment(Vector2 size, int count, Vector2 position, const string& name, bool isSign, const string& sign) {
	spEquationFragment fragment = new EquationFragment(size, count, isSign, sign);
	fragment->setName(name);
	fragment->setPosition(position);
	fragment->setAnchor(0.5f, 0.5f);

	return fragment;
}

void EquationElement::onCorrectAnswerTapped(Event *event) {
	EquationElementEvent correctAnswerEvent(EquationElementEvent::CORRECT);
	dispatchEvent(&correctAnswerEvent);
}