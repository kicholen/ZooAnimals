#ifndef _ANIMALFARMPANEL_
#define _ANIMALFARMPANEL_

#include "oxygine-framework.h"
#include "AnimalModel.h"
#include "TweenButton.h"

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

	spColorRectSprite _backgroundSprite;
	spTweenButton _expandButton;

	spTweenButton _playButton;
	spProgressBar _happyBar;
	spSprite _happyFace;

	Vector2 _sizeExpanded;
};

#endif