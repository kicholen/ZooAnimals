#ifndef _POPOBJECTSCONTAINER_
#define _POPOBJECTSCONTAINER_

#include "oxygine-framework.h"
#include "ParticlesContainer.h"

using namespace oxygine;

DECLARE_SMART(PopObjectsContainer, spPopObjectsContainer);

class PopObjectsContainer : public ParticlesContainer
{
public:
	PopObjectsContainer(const Vector2& size);
	~PopObjectsContainer();

	virtual void clear();
	int count();
	bool isFreeSpace();
	spParticle addParticle(const Vector2& position, float angle, unsigned int ncol, const std::string& resAnim, float radius);

protected:
	virtual void onParticleDie(Event *event);
private:
	void createBackground(const Vector2& size);
	void animateToPosition(spActor object, const Vector2& position);
	
	int getRows();
	int getColumns();
	void enableTouchOnAnimationFinish(Event *ev);

	Vector2 getNextObjectPosition();
	Vector2 getObjectPosition(int i, int j);

private:
	VectorArray<Point> _freeSlots;
	spBox9Sprite _background;
	float _radius;
	float _offset;
	int _count;
};

#endif