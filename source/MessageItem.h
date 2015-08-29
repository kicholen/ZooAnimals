#ifndef _MESSAGEITEM_
#define _MESSAGEITEM_

#include "oxygine-framework.h"
#include "MessageModel.h"

using namespace oxygine;

DECLARE_SMART(MessageItem, spMessageItem);

class MessageItem : public Actor
{
public:
	class MessageItemEvent : public Event
	{
	public:
		enum EV
		{
			MESSAGE_DELETED = makefourcc('M', 'I', 'M', 'D'),
			REWARDS_CLAIMED = makefourcc('M', 'I', 'M', 'C')
		};

		MessageItemEvent(EV ev, spMessageModel model_) : Event(ev), model(model_) {}

		spMessageModel model;
	};

	MessageItem(const Vector2& size, spMessageModel model);
	~MessageItem();

	void addReward(const std::string& rewardName);

	int position;
private:
	void setData(spMessageModel model);

	void createDateTextfield(int dateMS);
	void createTitleTexfield(int lockitId);
	void createDescriptionTextfield(int lockitId);
	void createIconSprite(const std::string& resourceName);
	void createBackground();

	void onMessageDeleted(Event *event);
	void onRewardsClaimed(Event *event);

private:
	spMessageModel _model;
};

#endif