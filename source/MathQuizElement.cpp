#include "MathQuizElement.h"
#include "SharedResources.h"

MathQuizElement::MathQuizElement(Vector2 size) {
	setSize(size);
	//setAnchor(0.5f, 0.5f);
	setData();
}

MathQuizElement::~MathQuizElement() {

}

void MathQuizElement::setData() {
	createChangeViewSprite();
	createBackground();
	reset(10, "+", 2);
}

void MathQuizElement::reset(int a, const std::string& sign, int b) {
	if (!_equation) {
		createEquation(a, sign, b);
	}
	else {
		_equation->reset(a, sign, b);
	}
}

void MathQuizElement::createBackground() {
	spBox9Sprite cardBackground = new Box9Sprite;
	cardBackground->setAnchor(0.5f, 0.5f);
	cardBackground->setResAnim(gameResources.getResAnim("memory_card_box9_a"));
	cardBackground->setSize(getSize());
	cardBackground->setPosition(getWidth() / 2, getHeight() / 2);
	cardBackground->setGuides(36, 144, 36, 144);
	cardBackground->attachTo(this);
	cardBackground->setPriority(-11);
}

void MathQuizElement::createEquation(int a, const std::string& sign, int b) {
	_equation = new EquationElement(getSize() * 0.8f, a, sign, b);
	_equation->setPosition(getWidth() / 2 - _equation->getWidth() / 2, getHeight() / 2 - _equation->getHeight() / 2);
	_equation->addEventListener(EquationElement::EquationElementEvent::CORRECT, CLOSURE(this, &MathQuizElement::onCorrectAnswer));
	_equation->attachTo(this);
	//_equation->animate();
}

void MathQuizElement::createChangeViewSprite() {
	spSprite help = new Sprite();
	help->setResAnim(gameResources.getResAnim("question_mark"));
	help->attachTo(this);
	help->addEventListener(TouchEvent::CLICK, CLOSURE(this, &MathQuizElement::onChangeView));
}

void MathQuizElement::onChangeView(Event  *event) {
	_equation->switchToAnotherView("test_particle");
}

void MathQuizElement::onCorrectAnswer(Event *event) {
	//_totalScore += 1;
	//_counterBox->updateScore(_totalScore);
	//_field->reset("dog", 8);
	dispatchEvent(event);
}