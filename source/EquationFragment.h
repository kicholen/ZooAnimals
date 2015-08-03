#ifndef _EQUATIONFRAGMENT_
#define _EQUATIONFRAGMENT_

#include "oxygine-framework.h"
#include "AnimatableElementContainer.h"

using namespace oxygine;

DECLARE_SMART(EquationFragment, spEquationFragment);
typedef enum {efDisplayView, efNumericView} EquationFragmentState;

class EquationFragment : public Actor
{
public:
	EquationFragment(Vector2& size, int count, bool isSign = false, const std::string& sign = "");
	~EquationFragment();

	void reset(int count, const std::string& assetName = "");
	void show(bool shouldAnimate);
	void hide(bool shouldAnimate);
	void switchToAnotherView(const std::string& assetName);

private:
	void switchToNumericView();
	void switchToDisplayView(const std::string& assetName = "");

	void createContainerIfDoesntExist();
	spSprite createContainerElement(const std::string& assetName);
	void addNumberTextField(int count);

	void setTextfieldSize();

	void onBackgroundTweenEnded(Event *event);
private:
	int _count;
	std::string _sign;
	bool _isSign;
	EquationFragmentState _state;
	spAnimatableElementContainer _container;

	static int _textSize;
};

#endif