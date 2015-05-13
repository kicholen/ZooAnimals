#ifndef _FRAMEH_
#define _FRAMEH_

#include "Frame.h"
#include "Sprite.h"
#include "res/Resources.h"
#include "Main.h"
#include "TweenButton.h"
#include "ShaderTweenButton.h"
#include "Transition.h"
#include "MoneyCounterElement.h"

using namespace oxygine;

class CustomFrame : public Frame
{
public:
	CustomFrame();
	
	spTweenButton addButton(const string &name, const string &text, Vector2 position);
	spShaderTweenButton addShaderButton(const string &name, const string &text, Vector2 position);

protected:
	static spMoneyCounterElement _moneyCounter;

	virtual void selectTransitions();

	virtual void _preShowing(Event *);

	virtual void _postHiding(Event *);

	virtual string getBackground();

	void init(const string &xml, bool shouldAddBackground);
	void init(const string &xml);

	void animateBackground();
	void onBackgroundTweenCompleted(Event *event);
	
	void playClickSound(Event *event);

	int _whichBackground;
	spActor _view;
	spActor _content;

	Resources _resources;
};

#endif