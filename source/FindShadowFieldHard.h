#ifndef _FINDSHADOWFIELDHARD_
#define _FINDSHADOWFIELDHARD_

#include "oxygine-framework.h"
#include "AnimatableElementContainer.h"

using namespace oxygine;

DECLARE_SMART(FindShadowFieldHard, spFindShadowFieldHard);

typedef enum { fshWaiting, fshCompleted, fshAnimating } FindShadowHardFieldState;

class FindShadowFieldHard : public Actor
{
public:
	class FindShadowFieldHardEvent : public Event {
	public:
		enum EV {
			SHADOW_FOUND = makefourcc('F', 'S', 'H', 'F'),
			SHADOW_WRONG = makefourcc('F', 'S', 'H', 'W'),
			LEVEL_ENDED = makefourcc('F', 'S', 'H', 'E')
		};

		FindShadowFieldHardEvent(EV ev) :Event(ev) {}
	};

	// difficulty 1 or 2
	FindShadowFieldHard(Vector2 size, int difficulty = 1);
	~FindShadowFieldHard();

	void restart();
private:
	void setParametersFromContent(int difficulty);

	void create();
	void createElementsInCanvasContainer();
	void createSpriteToBeFoundIfDoesntExist(const std::string name);
	spSprite createTouchableSprite(const std::string name);
	
	void animateCorrectSprite(spSprite sprite);
	void animateIncorrectSprite(spSprite sprite);

	bool isEnd();
	void dispatchLevelCompletedEvent();
	void dispatchWrongShadowClicked();
	void dispatchGoodShadowClicked();

	void onCorrectSpriteFinishedAnimating(Event *ev);
	void onSpriteClicked(Event *ev);
private:
	FindShadowHardFieldState _state;
	spAnimatableElementContainer _elementsInCanvasContainer;
	spSprite _spriteToBeFound;

	VectorArray<std::string> _rolledNamesVector;
	std::map<std::string, int> _rolledNamesMap;
	int _minElements;
	int _maxElements;
	int _elementsCount;
};

#endif