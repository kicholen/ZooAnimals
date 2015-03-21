#ifndef _WORLDMAPVIEW_
#define _WORLDMAPVIEW_

#include "oxygine-framework.h"

using namespace oxygine;

DECLARE_SMART(WroldMapView, spWroldMapView);

class WroldMapView : public Actor
{
public:
	class WroldMapViewEvent : public Event
	{
	public:
		enum EV {
			CONTINENT_SWITCHED = makefourcc('C', 'S', 'W', 'M')
		};

		WroldMapViewEvent(EV ev, const string &name):Event(ev), _name(name) {}

		const string _name;
	};


	WroldMapView(Vector2 size);
	~WroldMapView();

	void setData();

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

	float getMapScaleModifier(const string& name);
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