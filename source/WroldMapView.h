#ifndef _WORLDMAPVIEW_
#define _WORLDMAPVIEW_

#include "oxygine-framework.h"

using namespace oxygine;

DECLARE_SMART(WroldMapView, spWroldMapView);

class WroldMapView : public Actor
{
public:
	WroldMapView(Vector2 size);
	~WroldMapView();

	void setData();

protected:
	virtual void doUpdate(const UpdateState& us);
private:
	void showNorthAmerica();
	void showSouthAmerica();
	void showGreenland();
	void showAfrica();
	void showEurope();
	void showAustralia();
	void showAsia();

	void addMapSprite();

	void onContinentClicked(Event *event);

	void animateSprite(spSprite sprite);
private:
	spSprite _northAmerica;
	spSprite _southAmerica;
	spSprite _greenland;
	spSprite _africa;
	spSprite _europe;
	spSprite _australia;
	spSprite _asia;
	spSprite _animatedSprite;

	spSprite _map;
};

#endif