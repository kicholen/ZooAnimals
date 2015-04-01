#ifndef _WORLDMAPFRAME_
#define _WORLDMAPFRAME_

#include "CustomFrame.h"
#include "RegionAnimalsContainer.h"

DECLARE_SMART(WorldMapFrame, spWorldMapFrame);

class WorldMapFrame : public CustomFrame
{
public:
	WorldMapFrame();
	void selectTransitions();

	Action loop();

protected:
	void _postHiding(Event *);
	void _preShowing(Event *);

	void onContinentSwitched(Event *event);

	void setData();

private:
	spRegionAnimalsContainer _animalsContainer;
};

#endif