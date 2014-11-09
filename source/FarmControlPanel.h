#ifndef _FARMCONTROLPANEL_
#define _FARMCONTROLPANEL_

#include "oxygine-framework.h"
#include "FlashUtils.h"
#include "StackContainer.h"
#include "SharedResources.h"
#include "AnimatableElementContainer.h"

using namespace oxygine;
using namespace FlashUtils;

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

		FarmControlPanelEvent(EV ev, const string &name):Event(ev), _name(name) {}

		const string _name;
	};

	FarmControlPanel(Vector2 size, Vector2 protrudeSize);

	void show();//(bool shouldAnimate = true);
	void hide();//(bool shouldAnimate = true);
	
	void animateStar();
	void updateScore(int score);
protected:
	void handleClick(Event *event);

private:
	void setScore(int score);
	int getScore() const;

	typedef GetSet<int, int, FarmControlPanel, &FarmControlPanel::getScore, &FarmControlPanel::setScore> TweenScore;

	void onTweenEnded(Event *event);
	void showGamesView();
	void showShopView();
	void hideFullView();

	spBox9Sprite createBackground();
	spStackContainer createStackContainer();
	spTextActor createScoreTextField();
	spButton createButton(const string& actionName, const string& buttonResAnim);
	void createElementContainerIfNeeded();

	spAnimatableElementContainer _gameContainer;
	spTextActor _scoreTextField;
	spStackContainer _elementsContainer;
	int _score;
	FarmControlPanelState _state;
	Vector2 _protrudeSize;
};

#endif