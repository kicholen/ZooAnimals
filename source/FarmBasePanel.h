#ifndef _FARMBASEPANEL_
#define _FARMBASEPANEL_

#include "oxygine-framework.h"
#include "SharedResources.h"
#include "StackContainer.h"
#include "FlashUtils.h"

using namespace oxygine;
using namespace FlashUtils;

DECLARE_SMART(FarmBasePanel, spFarmBasePanel);
//typedef enum {fcpAnimating, fcpHidden, fcpPartShown, fcpGameView, fcpShopView} FarmControlPanelState;

#define TWEEN_DURATION 300

class FarmBasePanel : public StackContainer
{
public:
	class FarmBasePanelEvent : public Event
	{
	public:
		enum EV {
			CLICK_EVENT = makefourcc('F', 'B', 'C', 'E'),
		};

		FarmBasePanelEvent(EV ev, const string &name): Event(ev), _name(name) {}

		const string _name;
	};

	FarmBasePanel(Vector2 size, StackContainerAlign alignType = scVertical);
	
	void updateScore(int score);
private:
	spTextActor createScoreTextField();
	spTweenButton createTweenButton(const string& actionName, const string& buttonResAnim);
	void handleClick(Event *event);

	void setScore(int score);
	int getScore() const;
	typedef GetSet<int, int, FarmBasePanel, &FarmBasePanel::getScore, &FarmBasePanel::setScore> TweenScore;

	int _score;
	spTextActor _scoreTextField;
};

#endif