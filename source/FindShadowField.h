#ifndef _FINDSHADOWFIELD_
#define _FINDSHADOWFIELD_

#include "oxygine-framework.h"
#include "RotatingContainer2d.h"

using namespace oxygine;

DECLARE_SMART(FindShadowField , spFindShadowField);

typedef enum {fsWaiting, fsCompleted, fsAnimating} FindShadowFieldState;

class FindShadowField : public Actor
{
public:
	class FindShadowFieldEvent : public Event {
	public:
		enum EV {
			SHADOW_FOUND = makefourcc('F', 'S', 'F', 'C')
		};

		FindShadowFieldEvent(EV ev):Event(ev) {}
	};
	
	FindShadowField(Vector2 size, int numberOfShadows, std::string mainObjectName);

	void restart(int numberOfShadows, std::string mainObjectName);
private:
	void fillField(int numberOfShadows, std::string mainObjectName);
	void animateField();
	
	void animateEndCallback(Event *event);
	void onShadowTap(Event *event);

	void animateProperShadowFound(spActor actor);
	void dispatchShadowFoundEvent(Event *event);

	void createRotatingContainer();
	spSprite createSprite(std::string name, std::string spriteName, bool isShadowSprite);
private:
	FindShadowFieldState _state;
	int _animatedCount;
	spSprite _mainSprite;

	spRotatingContainer2d _wheel;
};

#endif