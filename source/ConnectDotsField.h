#ifndef _CONNECTDOTFIELD_
#define _CONNECTDOTFIELD_

#include "oxygine-framework.h"
#include "ConnectDotElement.h"
#include "SpriteSpawner.h"

using namespace oxygine;

#define LINE_BASE_DIMENSION 5.0f

DECLARE_SMART(ConnectDotsField , spConnectDotsField);

typedef enum {cdWaiting, cdCompleted, cdAnimating, cdFirstDotFound} ConnectDotsFieldState;

class ConnectDotsField : public Actor
{
public:
	class ConnectDotsFieldEvent: public Event {
	public:
		enum EV {
			FINISHED = makefourcc('C', 'D', 'F', 'F')
		};

		ConnectDotsFieldEvent(EV ev):Event(ev) {}
	};

	ConnectDotsField(Vector2 size, int levelNumber = 2);

	void reset(int level);

	void fillField(int levelNumber);
	bool animateNextIfNecessary();
private:

	void onDotTap(Event *event);
	void onDotOver(Event *event);

	void onFieldMove(Event *event);

	void onDotAnimationCompleted(Event *event);
	void onLineDissapear(Event *event);

	bool isCorrectDotAndCanProceed(spConnectDotElement dot);
	void onCorrectDotFound(spConnectDotElement dot);
	
	void onComplete();
	void dispatchFinishedEvent(Event *event = NULL);
	void animateOnComplete();
	// line draw
	void setStrechingLineAndCreateIfNeeded(Vector2 _startPosition);
	void setLineDimensionsAndRotation(spColorRectSprite quad,Vector2 to);

	float calculateDistance(Vector2 from, Vector2 to);
	float calculateAngle(Vector2 from, Vector2 to);

	void drawLineBeetweenFirstAndLastDot();
	void createStaticLine(Vector2 startPosition, Vector2 endPosition);

	void createOrChangeMaskedSprite(Group g);
private:
	ConnectDotsFieldState _state;
	int _numberOfDots;
	int _numberOfDotsCreated;
	int _currentSelected;
	spColorRectSprite _lineSprite;

	uint _animatedCount;

	spConnectDotElement _currentDot;
	spConnectDotElement _previousDot;
};

#endif