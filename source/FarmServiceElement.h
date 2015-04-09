#ifndef _FARMSERVICEELEMENT_
#define _FARMSERVICEELEMENT_

#include "oxygine-framework.h"
#include "AnimalModel.h"
#include "TweenButton.h"

using namespace oxygine;

typedef enum {topGame, midGame, lowGame} FarmServiceGameType;

DECLARE_SMART(FarmServiceElement, spFarmServiceElement);

class FarmServiceElement : public Actor
{
public:
	class FarmServiceElementEvent : public Event
	{
	public:
		enum EV {
			PLAY_GAMES = makefourcc('F', 'C', 'P', 'G'),
		};

		FarmServiceElementEvent(EV ev, const string &name) :Event(ev), _name(name) {}

		const string _name;
	};

	FarmServiceElement(Vector2 size);
	~FarmServiceElement();

	void setData(spAnimalModel model);

private:
	void createAnimalSprite(spAnimalModel model);
	void createHappinessProgressBar(float happinessValue);
	void createGameButtonByType(FarmServiceGameType type, const string& gameName);
	spTweenButton createGameButton(const string& resourceName, const string& buttonName);

	void onGameChosen(Event *event);
private:
	spSprite _animalSprite;
	spProgressBar _happinessProgressBar;
	spTweenButton _topGameButton;
	spTweenButton _midGameButton;
	spTweenButton _lowGameButton;

};

#endif