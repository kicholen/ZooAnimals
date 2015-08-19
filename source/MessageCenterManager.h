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
			NEW_MESSAGE = makefourcc('M', 'C', 'N', 'M')
		};

		MessageEvent(EV ev) : Event(ev) {}
	};

	static MessageCenterManager instance;

	MessageCenterManager();
	~MessageCenterManager();

	void init();
	void store();

	const int getMessagesCount() const {
		return _messages.size();
	};
	const std::vector<spMessageModel>& getMessages();
	void removeMessageByNumber(int number);

	void onAchievementsGained(spAchievementModel model);
private:
	void parseSavedState();

	void dispatchNewMessageEvent();

	spMessageModel addMessage(int type, int lockitTitle, int lockitDesc, const std::string& resourceName, int dateMS);

private:
	std::vector<spMessageModel> _messages;
};

#endif