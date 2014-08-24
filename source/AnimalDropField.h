#ifndef _ANIMALDROPFIELD_
#define _ANIMALDROPFIELD_

#include "oxygine-framework.h"

using namespace oxygine;

DECLARE_SMART(AnimalDropField, spAnimalDropField);

typedef enum {afWaiting, afFalling, afChecking, afBackSwaping} AnimalDropFieldState;

class AnimalDropField : public Actor
{
public:
	AnimalDropField(int objectsNeeded);

private:
	AnimalDropFieldState _state;

	int _animatedCount;
	int _droppedCount;
	int _objectsNeeded;
};


#endif
