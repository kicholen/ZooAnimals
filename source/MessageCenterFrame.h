#ifndef _MESSAGECENTERFRAME_
#define _MESSAGECENTERFRAME_

#include "CustomFrame.h"
#include "MessageItem.h"

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

	spMessageItem createMessageItem(spMessageModel model);
};

#endif