#ifndef _MESSAGECENTERMANAGER_
#define _MESSAGECENTERMANAGER_

#include "oxygine-framework.h"
#include "AchievementModel.h"
#include "MessageModel.h"

typedef enum { mmReward, mmInformation } MessageModelType;

class MessageCenterManager : public EventDispatcher
{
public:
	class MessageEvent : public Event
	{
	public:
		enum EV
		{
			MESSAGE_COUNT_CHANGE = makefourcc('M', 'C', 'M', 'C')
		};

		MessageEvent(EV ev, int count_) : Event(ev), _count(count_) {}

		int _count;
	};

	static MessageCenterManager instance;

	MessageCenterManager();
	~MessageCenterManager();

	void init();
	void store();

	const int getMessagesCount() const {
		return _messages.size();
	};

	bool shouldAnimate();
	const std::vector<spMessageModel>& getMessages();
	void removeMessage(spMessageModel model);

	void onAchievementsGained(spAchievementModel model);
private:
	void parseSavedState();

	void dispatchMessageCountChangedEvent();

	spMessageModel addMessage(int type, int lockitTitle, int lockitDesc, const std::string& resourceName, int dateMS);

private:
	std::vector<spMessageModel> _messages;
	bool _shouldAnimate;
};

#endif