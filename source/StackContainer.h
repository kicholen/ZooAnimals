#ifndef _STACKCONTAINER_
#define _STACKCONTAINER_

#include "oxygine-framework.h"

using namespace oxygine;

DECLARE_SMART(StackContainer, spStackContainer);
typedef enum {scVertical, scHorizontal} StackContainerAlign;

class StackContainer : public Actor
{
public:
	StackContainer(Vector2 size, StackContainerAlign alignType = scVertical);
	StackContainer();

	void setData(Vector2 size, StackContainerAlign alignType);

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