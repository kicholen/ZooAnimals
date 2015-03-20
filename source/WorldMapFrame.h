#ifndef _WORLDMAPFRAME_
#define _WORLDMAPFRAME_

#include "CustomFrame.h"

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

	void onFinished(Event *event);

	void setData();
};

#endif