#include "MessageItem.h"
#include "SharedResources.h"
#include "LanguageManager.h"

MessageItem::MessageItem(const Vector2& size, spMessageModel model) {
	setSize(size);
	setTouchEnabled(false);

	setData(model);
}

MessageItem::~MessageItem() {

}

void MessageItem::setData(spMessageModel model) {
	createDateTextfield(model->getDateMS());
	createDescriptionTextfield(model->getLockitDescription());
	createTitleTexfield(model->getLockitTitle());
	createIconSprite(model->getResourceName());
	
	if (model->getType() == 0) {
		createButton("", LanguageManager::instance.getText(100))->addEventListener(TouchEvent::CLICK, CLOSURE(this, &MessageItem::onRewardsClaimed));
	}
	else {
		createButton("", LanguageManager::instance.getText(100))->addEventListener(TouchEvent::CLICK, CLOSURE(this, &MessageItem::onMessageDeleted));
	}
}

void MessageItem::createDateTextfield(int dateMS) {
	//todo make timMs to date converter?
	TextStyle style;
	style.font = gameResources.getResFont("nobile_bold")->getFont();
	style.vAlign = TextStyle::VALIGN_MIDDLE;
	style.hAlign = TextStyle::HALIGN_CENTER;
	style.multiline = true;
	style.color = Color(35, 145, 245);

	createTextFieldInBoundries(CMath::intToString(dateMS), Vector2(50, 50), style);
}

void MessageItem::createTitleTexfield(int lockitId) {
	TextStyle style;
	style.font = gameResources.getResFont("nobile_bold")->getFont();
	style.vAlign = TextStyle::VALIGN_MIDDLE;
	style.hAlign = TextStyle::HALIGN_CENTER;
	style.multiline = true;
	style.color = Color(35, 145, 245);

	createTextFieldInBoundries(LanguageManager::instance.getText(lockitId), Vector2(50, 50), style);
}

void MessageItem::createDescriptionTextfield(int lockitId) {
	TextStyle style;
	style.font = gameResources.getResFont("nobile_bold")->getFont();
	style.vAlign = TextStyle::VALIGN_MIDDLE;
	style.hAlign = TextStyle::HALIGN_CENTER;
	style.multiline = true;
	style.color = Color(35, 145, 245);

	createTextFieldInBoundries(LanguageManager::instance.getText(lockitId), Vector2(50, 50), style);
}

void MessageItem::createIconSprite(const std::string& resourceName) {

}

void MessageItem::onMessageDeleted(Event *event) {
	MessageItemEvent ev(MessageItemEvent::MESSAGE_DELETED, _model);
	dispatchEvent(&ev);
}

void MessageItem::onRewardsClaimed(Event *event) {
	MessageItemEvent ev(MessageItemEvent::REWARDS_CLAIMED, _model);
	dispatchEvent(&ev);
}