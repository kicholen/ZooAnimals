#ifndef _QUEUETEXTANIMATION_
#define _QUEUETEXTANIMATION_

#include "oxygine-framework.h"
#include "deque"

using namespace oxygine;

DECLARE_SMART(QueueTextAnimation, spQueueTextAnimation);

class QueueTextAnimation : public Actor
{
public:
	class QueueTextEvent : public Event
	{
	public:
		enum
		{
			ANIMATION_DONE = makefourcc('Q', 'T', 'E', 'D')
		};

		QueueTextEvent(int asd) :Event(ANIMATION_DONE, false) {}
	};

	QueueTextAnimation();
	~QueueTextAnimation();

	void start(EventCallback onAnimationFinishedCallback, bool shouldCallOnPenultimate = true);
	void addTextToQueue(const string& text);
	
private:
	void animateNext(Event *ev);
	
	spTextField createTextField();
	void callAnimationFinishedCallback();

private:
	EventCallback _onAnimationFinishedCallback;

	deque<string> _textQueue;

	bool _shouldCallOnPenultimate;
};

#endif