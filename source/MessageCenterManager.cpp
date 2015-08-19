#include "MessageCenterManager.h"
#include "AchievementManager.h"
#include "ZooSettings.h"

MessageCenterManager MessageCenterManager::instance;

MessageCenterManager::MessageCenterManager() {

}

MessageCenterManager::~MessageCenterManager() {

}

void MessageCenterManager::init() {
	parseSavedState();
	ZooSettings::instance.clearMessagesNode();
	addRef();
}

void MessageCenterManager::store() {
	for (uint i = 0; i < _messages.size(); i++) {
		ZooSettings::instance.setMessage(_messages[i]->getType(), _messages[i]->getLockitTitle(), _messages[i]->getLockitDescription(), _messages[i]->getResourceName(), _messages[i]->getDateMS(), _messages[i]->getRewards());
	}
}

const std::vector<spMessageModel>& MessageCenterManager::getMessages() {
	return _messages;
}

void MessageCenterManager::removeMessage(spMessageModel model) {
	for (uint i = 0; i < _messages.size(); i++) {
		if (model == _messages[i]) {
			_messages.erase(_messages.begin() + i);
		}
	}
	dispatchMessageCountChangedEvent();
}

void MessageCenterManager::parseSavedState() {
	pugi::xml_node messages = ZooSettings::instance.getMessagesNode();

	int type = 0;
	int lockitTitle = 0;
	int lockitDesc = 0;
	std::string resourceName = "";
	int dateMS = 0;

	if (!messages.empty()) {
		pugi::xml_node message = messages.first_child();

		while (!message.empty()) {
			spMessageModel model = new MessageModel();

			pugi::xml_attribute attribute = message.first_attribute();
			type = attribute.as_int();
			attribute = attribute.next_attribute();
			lockitTitle = attribute.as_int();
			attribute = attribute.next_attribute();
			lockitDesc = attribute.as_int();
			attribute = attribute.next_attribute();
			resourceName = attribute.as_string();
			attribute = attribute.next_attribute();
			dateMS = attribute.as_int();

			while (!attribute.empty()) {
				model->addReward(attribute.as_int());
				attribute = attribute.next_attribute();
			}

			model->setData(type, lockitTitle, lockitDesc, resourceName, dateMS);
			_messages.push_back(model);

			message = message.next_sibling();
		}
	}
}

spMessageModel MessageCenterManager::addMessage(int type, int lockitTitle, int lockitDesc, const std::string& resourceName, int dateMS) {
	spMessageModel model = new MessageModel(type, lockitTitle, lockitDesc, resourceName, dateMS);
	_messages.push_back(model);

	return model;
}

void MessageCenterManager::onAchievementsGained(spAchievementModel model) {
	addMessage(mmReward, model->getLockitTitle(), model->getLockitDescription(), model->getResourceName(), (int)getTimeUTCMS());
	dispatchMessageCountChangedEvent();
}

void MessageCenterManager::dispatchMessageCountChangedEvent() {
	MessageEvent ev(MessageEvent::MESSAGE_COUNT_CHANGE, _messages.size());
	dispatchEvent(&ev);
}