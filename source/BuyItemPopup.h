#ifndef _POPUP_
#define _POPUP_

#include "LandingPageFrame.h"
#include "Main.h"

DECLARE_SMART(BuyItemPopup, spBuyItemPopup);

class BuyItemPopup : public CustomFrame
{
public:
	class WasPurchaseCompletedEvent: public Event
	{
	public:
		enum EV
		{
			PURCHASE_COMPLETED = makefourcc('P', 'W', 'C', 'D'),
		};

		WasPurchaseCompletedEvent(EV ev):Event(ev) {}
	};
	BuyItemPopup();

	Action loop();
protected:
	void selectTransitions();
	void _update(const UpdateState &us);
	void _postHiding(Event *);
	void _preShowing(Event *);
	void setData();
private:
	UpdateCallback _updateCallback;
	timeMS _lastCheckTimeMS;
};

#endif