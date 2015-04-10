#ifndef _ANIMALFARMPANEL_
#define _ANIMALFARMPANEL_

#include "oxygine-framework.h"
#include "AnimalModel.h"
#include "TweenButton.h"
#include "FarmServiceElement.h"

using namespace oxygine;

DECLARE_SMART(AnimalFarmPanel, spAnimalFarmPanel);

typedef enum {afpExpanded, afpNormal} AnimalFarmPanelState;

class AnimalFarmPanel : public Actor
{
public:
	AnimalFarmPanel(const Vector2& size, const Vector2& sizeExpanded);
	~AnimalFarmPanel();

	void setData(spAnimalModel model);
	void closeExpandedViewIfOpen();
private:
	void handleClick(Event *event);
	void onGameChosen(Event *event);
	void onBackgroundAnimationFinished(Event *event);
	void onViewSwitched(Event *event);

	void switchViewToExpanded();
	void switchViewToNormal();

	void showNormalView();
	void showExpandedView();

	void hideNormalView();
	void hideExpandedView();

private:
	AnimalFarmPanelState _state;
	spAnimalModel _model;

	spBox9Sprite _backgroundSprite;
	spTweenButton _expandButton;
	spTextField _animalNameTextfield;

	spFarmServiceElement _expandedElement;
	spProgressBar _happyBar;
	spSprite _happyFace;

	Vector2 _sizeExpanded;
	Vector2 _expanButtonBasePoition;
	Vector2 _expanButtonExpandedPoition;

	bool _isAnimating;
};

#endif