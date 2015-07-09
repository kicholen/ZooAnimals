#ifndef _COUNTERBOXELEMENT_
#define _COUNTERBOXELEMENT_

#include "oxygine-framework.h"
#include "ProgressBarBox9.h"

using namespace oxygine;

DECLARE_SMART(CounterBoxElement, spCounterBoxElement);

class CounterBoxElement : public Actor
{
public:
	class CounterBoxEvent: public Event
	{
	public:
		enum EV
		{
			TIME_END = makefourcc('C', 'B', 'T', 'E'),
		};

		CounterBoxEvent(EV ev):Event(ev) {}
	};

	CounterBoxElement(Vector2 size, int time);

	void show(bool shouldAnimate);
	void hide(bool shouldAnimate);

	void updateScore(int score);
	void updateLevel(int level);
	int getScore() const { return _score; };
	int getLevel() const { return _level; };

	void animateTimeLoss(int loss);
	void animateTimeGain(int gain);
private:
	void animateProgressBar(float ratio);
	void updateTime(int time);

	void setScore(int score);

	void setTime(int time);
	int getTime() const;

	typedef GetSet<int, int, CounterBoxElement, &CounterBoxElement::getScore, &CounterBoxElement::setScore> TweenScore;
	typedef GetSet<int, int, CounterBoxElement, &CounterBoxElement::getTime, &CounterBoxElement::setTime> TweenTime;

	void createBackground();
	void createTimerTextfield();
	void createScoreTextfield();
	void createTimerBar();
	void createLevelTextfield();

	void timerUpdate(const UpdateState &us);

	void onTimeEnded(Event *ev);

	spTextField _timerTextfield;
	spTextField _scoreTextfield;
	spTextField _levelTextfield;
	spProgressBarBox9 _progressBar;

	timeMS _timerMs;
	int _timeLeft;
	int _timeMax;
	int _score;
	int _level;
};

#endif
