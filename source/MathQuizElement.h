#ifndef _MATHQUIZELEMENT_
#define _MATHQUIZELEMENT_

#include "oxygine-framework.h"
#include "EquationElement.h"

using namespace oxygine;

DECLARE_SMART(MathQuizElement, spMathQuizElement);

class MathQuizElement : public Actor
{
public:
	MathQuizElement(Vector2 size);
	~MathQuizElement();

	void setData();
	void reset(int a, const std::string& sign, int b);
private:
	void createBackground();
	void createEquation(int a, const std::string& sign, int b);
	void createChangeViewSprite();

	void onChangeView(Event  *event);
	void onCorrectAnswer(Event *event);
private:
	spEquationElement _equation;
};

#endif