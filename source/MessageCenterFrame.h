#ifndef _MESSAGECENTERFRAME_
#define _MESSAGECENTERFRAME_

#include "CustomFrame.h"
#include "MessageItem.h"

#define OFFSET_MESSAGES 20

DECLARE_SMART(MessageCenterFrame, spMessageCenterFrame);

class MessageCenterFrame : public CustomFrame
{
public:
	MessageCenterFrame();
	~MessageCenterFrame();

	Action loop();

	void selectTransitions();
protected:
	void _postHiding(Event *);
	void _preShowing(Event *);

	void setData();

	spMessageItem createMessageItem(const Vector2& size, spMessageModel model);

private:
	void onRewardClaimed(Event *event);
	void onMessageDeleted(Event *event);

};

#endif