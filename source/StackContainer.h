#ifndef _STACKCONTAINER_
#define _STACKCONTAINER_

#include "oxygine-framework.h"

using namespace oxygine;

DECLARE_SMART(StackContainer, spStackContainer);
typedef enum StackContainerAlign {scVertical = 0, scHorizontal = 1} ;

class StackContainer : public Actor
{
public:
	/*
	* Cause of stupidity of c++, and my lack of knowledge.
	* scVertical = 0, scHorizontal = 1
	*/
	StackContainer(Vector2 size, int alignType = 0);
	StackContainer();

	void setData(Vector2 size, int alignType = 0);

	void addChild(spActor actor);
	void addChild(Actor *actor);
	void removeChild(spActor actor);
	void removeChildren();

private:
	void updateChildren();
	void setChildPositionAndScale(spActor actor, int count);
	Vector2 getProperChildSize();
	Vector2 getProperChildPosition(int count);

	StackContainerAlign _alignType;
	int _childrenCount;
	Vector2 _properChildSize;
	bool _needsUpdateChildSize;
};

#endif