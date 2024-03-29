#ifndef _PRESENTANIMALSFRAME_
#define _PRESENTANIMALSFRAME_

#include "CustomFrame.h"
#include "FlashUtils.h"
#include "AnimalModel.h"
#include "CardNavigator.h"

using namespace FlashUtils;

DECLARE_SMART(PresentAnimalsFrame, spPresentAnimalsFrame);

class PresentAnimalsFrame : public CustomFrame
{
public:
	PresentAnimalsFrame(const std::string& region);
	void selectTransitions();

	Action loop();
protected:
	void _postHiding(Event *);
	void _preShowing(Event *);

	void setData();

	void onSwipeLeft(Event *ev);
	void onSwipeRight(Event *ev);
private:
	spCardNavigator _cardNavigator;
	std::string _region;
};

#endif