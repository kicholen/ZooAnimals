#include "AnimalDropField.h"
#include "SharedResources.h"

AnimalDropField::AnimalDropField(int objectsNeeded) {
	_animatedCount = 0;
	_droppedCount = 0;
	_objectsNeeded = objectsNeeded;

	_state = afWaiting;
}
