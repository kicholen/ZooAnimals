#ifndef _WALLPOSTMESSAGEPOPUP_
#define _WALLPOSTMESSAGEPOPUP_

#include "LandingPageFrame.h"
#include "Main.h"
#include "TextActor.h"

DECLARE_SMART(WallPostMessagePopup, spWallPostMessagePopup);

class WallPostMessagePopup : public CustomFrame
{
public:
	class FacebookPostMessage: public Event
	{
	public:
		enum EV
		{
			FACEBOOK_POST_MESSAGE_SUCCESS = makefourcc('F', 'P', 'M', 'S'),
		};

		FacebookPostMessage(EV ev):Event(ev) {}
	};

	WallPostMessagePopup();
	~WallPostMessagePopup();

	Action loop();

	void startWallPost();
	void updateWallPost();
	void showMessage(std::string message);

protected:
	void selectTransitions();
	void _postHiding(Event *);
	void _preShowing(Event *);
	void setData();

	void _update(const UpdateState &us);

private:
	enum WallPostStatus {
		WPS_IDLE,
		WPS_LOG_IN, WPS_WAITING_FOR_LOG_IN,
		WPS_POST, WPS_WAITING_FOR_POST,
		WPS_SHOWING_MESSAGE
	};

	spTextActor _infoMessage;
	void onWallPostMessage(Event *event);
	WallPostStatus _wallPostStatus;

	UpdateCallback _updateCallback;
	bool _isActionFB;
};

#endif