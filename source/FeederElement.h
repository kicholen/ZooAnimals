#ifndef _FEEDERELEMENT_
#define _FEEDERELEMENT_

#include "oxygine-framework.h"
#include "TweenButton.h"
#include "AnimalModel.h"

using namespace oxygine;

DECLARE_SMART(FeederElement, spFeederElement);

class FeederElement : public Actor 
{
public:
	FeederElement(const Vector2& size, spAnimalModel model);
	~FeederElement();

private:
	void revalidate();

	void createBackground();
	void createButton();
	void createProgressbar();
	void createCooldownTextfield();

	void setCooldown(int time);
	int getCooldown() const;

	typedef GetSet<int, int, FeederElement, &FeederElement::getCooldown, &FeederElement::setCooldown> TweenCooldown;

	void onFeedClicked(Event *ev);
	void onTimePassed(Event *ev);
private:
	spProgressBar _progressBar;
	spTweenButton _button;
	spTextField _cooldown;
	spSprite _background;

	spAnimalModel _model;

	int _cooldownLeft;
	int _cooldownMax;
};

#endif