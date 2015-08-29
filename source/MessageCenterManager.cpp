#include "MessageCenterManager.h"
#include "AchievementManager.h"
#include "ZooSettings.h"
#include "s3eTimer.h"

MessageCenterManager MessageCenterManager::instance;

MessageCenterManager::MessageCenterManager() {

}

MessageCenterManager::~MessageCenterManager() {

}

void MessageCenterManager::init() {
	parseSavedState();
	ZooSettings::instance.clearMessagesNode();
	addRef();
	_shouldAnimate = _messages.size() > 0;
}

void MessageCenterManager::store() {
	for (uint i = 0; i < _messages.size(); i++) {
		ZooSettings::instance.setMessage(_messages[i]->getType(), _messages[i]->getLockitTitle(), _messages[i]->getLockitDescription(), _messages[i]->getResourceName(), _messages[i]->getDateMS(), _messages[i]->getRewards());
	}
}

bool MessageCenterManager::shouldAnimate() {
	bool value = _shouldAnimate;
	_shouldAnimate = false;

	return value;
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
			attribute = attribute.next_attribute();

			while (!attribute.empty()) {
				model->addReward(attribute.as_string());
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

	_shouldAnimate = true;
	return model;
}

void MessageCenterManager::onAchievementsGained(spAchievementModel model) {
	spMessageModel message = addMessage(mmReward, model->getLockitTitle(), model->getLockitDescription(), model->getResourceName(), (int)(s3eTimerGetUTC() / 1000));
	
	const std::vector<std::string>& rewards = model->getRewards(model->getCurrentPart());
	for (uint i = 0; i < rewards.size(); i++) {
		message->addReward(rewards[i]);
	}

	dispatchMessageCountChangedEvent();
}

void MessageCenterManager::dispatchMessageCountChangedEvent() {
	MessageEvent ev(MessageEvent::MESSAGE_COUNT_CHANGE, _messages.size());
	dispatchEvent(&ev);
}