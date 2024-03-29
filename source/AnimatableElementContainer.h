#ifndef _ANIMATABLEELEMENTCONTAINER_
#define _ANIMATABLEELEMENTCONTAINER_

#include "oxygine-framework.h"
#include "FlashUtils.h"
#include "StackContainer.h"

using namespace oxygine;
using namespace FlashUtils;

DECLARE_SMART(AnimatableElementContainer, spAnimatableElementContainer);
typedef enum {aecScale, aecMove, aecAlpha} AnimatableElementContainerAnimationType;

#define TWEEN_DURATION 300
#define TWEEN_TIME_MIN 0
#define TWEEN_TIME_MAX 200

class AnimatableElementContainer : public Actor
{
public:
	AnimatableElementContainer(Vector2 size, float offsetPercent = 5.0f);
	~AnimatableElementContainer();

	void setAnimationType(AnimatableElementContainerAnimationType animationType);
	void showContainerElements(bool shouldAnimate = false);
	void hideContainerElements(bool shouldAnimate = false);

	void removeChildren();
	void addChildren(VectorArray<spActor>& children);
	void alterChildrenPositionToPoint(const Vector2& point, float amplitude);
	void randomizeChildrenViewParams();

private:
	void preventActorOffContainerBounds(spActor actor);
	void clearStackContainers();
	void updateChildren();

	Point getProperArrayDimensions();
	void addAnimationTween(spActor actor, bool show);
	void setAlphaToActor(spActor actor, bool show);

	VectorArray<spStackContainer> _stackContainersArray;
	VectorArray<spActor> _childrenArray;

	Point _properArrayDimensions;
	bool _needsUpdateArrayDimensions;
	float _offsetPercent;

	AnimatableElementContainerAnimationType _animationType;
};

#endif
