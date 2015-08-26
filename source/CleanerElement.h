#ifndef _CLEANERELEMENT_
#define _CLEANERELEMENT_

#include "oxygine-framework.h"
#include "TweenButton.h"
#include "AnimalModel.h"

using namespace oxygine;

DECLARE_SMART(CleanerElement, spCleanerElement);

class CleanerElement : public Actor
{
public:
	CleanerElement(const Vector2& size, spAnimalModel model);
	~CleanerElement();

private:
	void revalidate();

	void createBackground();
	void createButton();
	void createProgressbar();
	void createCooldownTextfield();

	void setCooldown(int time);
	int getCooldown() const;

	typedef GetSet<int, int, CleanerElement, &CleanerElement::getCooldown, &CleanerElement::setCooldown> TweenCooldown;

	void onCleanClicked(Event *ev);
	void onTimePassed(Event *ev);
private:
	spProgressBar _progressBar;
	spButton _button;
	spTextField _cooldown;
	spSprite _background;

	spAnimalModel _model;

	int _cooldownLeft;
	int _cooldownMax;
};

#endif

