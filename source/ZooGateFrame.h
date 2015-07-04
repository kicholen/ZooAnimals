#ifndef _ZOOGATEFRAME_
#define _ZOOGATEFRAME_

#include "CustomFrame.h"
#include "SpectatorSpawner.h"

DECLARE_SMART(ZooGateFrame, spZooGateFrame);

class ZooGateFrame : public CustomFrame
{
public:
	ZooGateFrame();

	void selectTransitions();

	Action loop();

protected:
	void _postHiding(Event *);
	void _preShowing(Event *);

private:
	void setData();
	float getXOnTilesToView(float x);
	void onButtonClicked(Event *ev);

	void onZooSectionChosen(Event *event);

	void addPossibleSpritesToSpawner();
	void addBaseTrackToSpawner(float tileSize);
	void addFarmTrackToSpawner(float tileSize, float tilesToViewOffsetX);
	void addSignPost(float tileSize, const std::string& region, const Vector2& position);
	void addWinterTrackToSpawner(float tileSize, float tilesToViewOffsetX);
	void addUnderwaterTrackToSpawner(float tileSize, float tilesToViewOffsetX);
	void addSteppeTrackToSpawner(float tileSize, float tilesToViewOffsetX);
	void addAsiaTrackToSpawner(float tileSize, float tilesToViewOffsetX);
	void addAustraliaTrackToSpawner(float tileSize, float tilesToViewOffsetX);

private:
	spSpectatorSpawner _spawner;
};

#endif