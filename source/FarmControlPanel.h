#ifndef _FARMCONTROLPANEL_
#define _FARMCONTROLPANEL_

#include "oxygine-framework.h"
#include "FarmBasePanel.h"
#include "SharedResources.h"
#include "AnimatableElementContainer.h"

using namespace oxygine;

DECLARE_SMART(FarmControlPanel, spFarmControlPanel);
typedef enum {fcpAnimating, fcpHidden, fcpPartShown, fcpGameView, fcpShopView} FarmControlPanelState;

#define TWEEN_DURATION 300

class FarmControlPanel : public Actor
{
public:
	class FarmControlPanelEvent : public Event
	{
	public:
		enum EV {
			BUY_ANIMALS = makefourcc('F', 'C', 'B', 'A'),
			PLAY_GAMES = makefourcc('F', 'C', 'P', 'G'),
		};

		FarmControlPanelEvent(EV ev, const std::string &name):Event(ev), _name(name) {}

		const std::string _name;
	};

	FarmControlPanel(Vector2 size, Vector2 protrudeSize);

	void show();
	void hide();
	
	void animateStar();
	void updateScore(int score);
protected:
	void handleClick(Event *event);

private:
	void onTweenEnded(Event *event);
	void showGamesView();
	void showShopView();
	void hideFullView();
	void switchFarmBasePanel();

	spBox9Sprite createBackground();
	spFarmBasePanel createFarmBasePanel();
	spTweenButton createButton(const std::string& actionName, const std::string& buttonResAnim);
	void createElementContainerIfNeeded();
	spTweenButton createCloseButtonIfNeeded();

	spAnimatableElementContainer _gameContainer;
	spFarmBasePanel _farmBasePanel;
	FarmControlPanelState _state;
	Vector2 _protrudeSize;
};

#endif