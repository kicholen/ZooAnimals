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

private:
	void setData();
	void revalidate(int removedPosition, float height);

	spMessageItem createMessageItem(const Vector2& size, spMessageModel model);
	void createBackground();
	void createTitle();

	void onRewardClaimed(Event *event);
	void onMessageDeleted(Event *event);
	void onRevalidateFinished(Event * event);
private:
	spColorRectSprite _rectangleContainer;
	spSlidingActor _slidingActor;

};

#endif