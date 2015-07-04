#ifndef _EQUATIONELEMENT_
#define _EQUATIONELEMENT_

#include "oxygine-framework.h"
#include "EquationFragment.h"

using namespace oxygine;

DECLARE_SMART(EquationElement, spEquationElement);

class EquationElement : public Actor
{
public:
	class EquationElementEvent: public Event {
	public:
		enum EV {
			CORRECT = makefourcc('E', 'E', 'E', 'Z')
		};

		EquationElementEvent(EV ev):Event(ev) {}
	};
	EquationElement(Vector2 size, int a, const std::string& sign, int b);
	~EquationElement();

	void reset(int a, const std::string& sign, int b);
	void show();
	void switchToAnotherView(const std::string& assetName = "");

private:
	void setData(int a, const std::string& sign, int b);
	int getCorrectResult(int a, const std::string& sign, int b);
	int getDiffrentIntegerInRange(VectorArray<int>& integers, int min, int max);

	spEquationFragment createEquationFragment(Vector2 size, int count, Vector2 position, const std::string& name, bool isSign = false, const std::string& sign = "");

	void onCorrectAnswerTapped(Event *event);
};

#endif