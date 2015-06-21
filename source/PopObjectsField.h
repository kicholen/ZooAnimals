#ifndef _POPOBEJCTSFIELD_
#define _POPOBJECTSFIELD_

#include "oxygine-framework.h"
#include "FlashUtils.h"
#include "ParticleEmitter.h"

using namespace oxygine;
using namespace FlashUtils;

DECLARE_SMART(PopObjectsField, spPopObjectsField);

class PopObjectsField : public Actor
{
public:
	PopObjectsField();
	~PopObjectsField();

private:
	void onParticlePopped(Event *ev);

	uint _particlesPoppedCount;
	spParticleEmitter _emitter;
	spParticlesContainer _container;
};

#endif