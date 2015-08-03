#ifndef _POPOBEJCTSFIELD_
#define _POPOBJECTSFIELD_

#include "oxygine-framework.h"
#include "FlashUtils.h"
#include "ParticleEmitter.h"
#include "PopObjectsContainer.h"
#include "EquationElement.h"

using namespace oxygine;
using namespace FlashUtils;

DECLARE_SMART(PopObjectsField, spPopObjectsField);

class PopObjectsField : public Actor
{
public:
	PopObjectsField(const Vector2& size, const std::string& difficulty);
	~PopObjectsField();

	bool isCorrect();
	void reset();
private:
	void setData();
	void createEmitter();
	void createContainer();
	void createEquation();

	void onParticlePopped(Event *ev);

private:
	std::string _difficulty;
	uint _particlesPoppedCount;
	spParticleEmitter _emitter;
	spPopObjectsContainer _container;
	spEquationElement _equation;
};

#endif