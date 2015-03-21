#ifndef _REGIONANIMALSCONTAINER_
#define _REGIONANIMALSCONTAINER_

#include "oxygine-framework.h"
#include "StackContainer.h"
#include "AnimalSlot.h"

using namespace oxygine;

DECLARE_SMART(RegionAnimalsContainer, spRegionAnimalsContainer);

class RegionAnimalsContainer : public Actor
{
public:
	RegionAnimalsContainer(const Vector2& size);
	~RegionAnimalsContainer();

	void setRegion(const string& regionName);

private:
	void reset();
	void createContainerIfDoesntExist();

private:
	spStackContainer _stackContainer;
	string _regionName;
	int _previousCount;
};

#endif