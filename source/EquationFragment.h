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

	void createBackground();
	void createContainerIfDoesntExist();
	spSprite createContainerElement(const std::string& assetName);
	void addNumberTextField(int count);

	void setTextFieldAndBackgroundSize();
	void setBackgroundSize(int width, int height);

	void onBackgroundTweenEnded(Event *event);
private:
	int _count;
	std::string _sign;
	bool _isSign;
	EquationFragmentState _state;
	spAnimatableElementContainer _container;
	spColorRectSprite _cardBackground;

	static int _textSize;
	static Vector2 _backgroundSize;
};

#endif