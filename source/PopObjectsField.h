#ifndef _POPOBEJCTSFIELD_
#define _POPOBJECTSFIELD_

#include "oxygine-framework.h"
#include "FlashUtils.h"

using namespace oxygine;
using namespace FlashUtils;

DECLARE_SMART(PopObjectsField, spPopObjectsField);

class PopObjectsField : public Actor
{
public:
	PopObjectsField();
	~PopObjectsField();

protected:
	virtual void doUpdate(const UpdateState &us);
	
private:
	void spawnPopObjects(int count);
	void spawnPopObject();
	void thinkObjects();

	void createParticles();

	float _frameTime;
};

#endif