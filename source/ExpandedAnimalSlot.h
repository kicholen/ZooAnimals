#ifndef _EXPANDEDANIMALSLOT_
#define _EXPANDEDANIMALSLOT_

#include "AnimalSlot.h"
#include "AnimalModel.h"
#include "ConnectDotElement.h"

DECLARE_SMART(ExpandedAnimalSlot, spExpandedAnimalSlot);

class ExpandedAnimalSlot : public AnimalSlot
{
public:
	ExpandedAnimalSlot(spAnimalModel model);
	ExpandedAnimalSlot(const string& id);
	~ExpandedAnimalSlot();

	void switchAnimalModel(spAnimalModel model);

private:
	void setAnimalCount();

private:
	spAnimalModel _model;
	spConnectDotElement _animalCountElement;
};

#endif