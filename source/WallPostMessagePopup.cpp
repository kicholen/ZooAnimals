#include "WallPostMessagePopup.h"
#include "FacebookManager.h"
#include "Settings.h"
#include "FlurryAnalytics.h"

WallPostMessagePopup::WallPostMessagePopup() {
	init("popup.xml");
	selectTransitions();
	_updateCallback.p_this = this;
	_updateCallback.p_proxy = CLOSURE(this, &WallPostMessagePopup::_update).p_proxy;
	_isActionFB = false;
}

WallPostMessagePopup::~WallPostMessagePopup() {

}

void WallPostMessagePopup::selectTransitions() {
	spTransition tr = new TransitionScale;
	setTransitionIn(tr);
	setTransitionOut(tr);
}

Action WallPostMessagePopup::loop(){
	while (1) {
		Action action = waitAction();
		if (action.id == "close") {
			break;
		}
	}

	return _lastAction;
}

void WallPostMessagePopup::_postHiding(Event *) {
	_view->removeChildren();
	_resources.unload();
}

void WallPostMessagePopup::_preShowing(Event *) {
	selectTransitions();
	_resources.load();
	setData();
}

void WallPostMessagePopup::setData() {
	_wallPostStatus = WPS_IDLE;
	_isActionFB = Settings::instance.getValue("action_fb").as_bool();
	string name;
	if (_isActionFB) {
		name = "more_permission";
	}
	else {
		name = "less_permission";
	}
	FlurryAnalytics::instance.fbAskForPermission(name.c_str());

	spBox9Sprite popupBackground;
	popupBackground = new Box9Sprite;
	popupBackground->setAnchor(0.5f, 0.5f);
	popupBackground->setResAnim(gameResources.getResAnim("popup_box9_fb"));
	popupBackground->setSize(_view->getHeight() * 0.8f, _view->getHeight() * 0.8f);
	popupBackground->setPosition(_view->getWidth() / 2, _view->getHeight() / 2);
	popupBackground->setTouchEnabled(false);
	popupBackground->setVerticalMode(Box9Sprite::TILING_FULL);
	popupBackground->setHorizontalMode(Box9Sprite::TILING_FULL);
	popupBackground->setGuides(50, 200, 50, 200);
	popupBackground->attachTo(_view);

	if (FacebookManager::instance.isAvailable()) {
		if (!_infoMessage) {
			if (_isActionFB) {
				_infoMessage = createTextfield("Would you like to unlock additional level? \n Just post a message on your Facebook wall about Animals Zoo and like our fanpage.", true, true);
			}
			else {
				_infoMessage = createTextfield("Would you like to unlock additional level? \n Just post a message on your Facebook wall about Animals Zoo.", true, true);
			}
			_infoMessage->setFontSize2Scale(20 * (int)_view->getWidth() / 640);
			_infoMessage->setSize(popupBackground->getHeight() * 0.9f, popupBackground->getHeight() * 0.7f);
			_infoMessage->setPosition(_view->getWidth() / 2, _view->getHeight() * 0.4f);
		}
		if (!_infoMessage->getParent()) {
			_view->addChild(_infoMessage);
		}
		if (_isActionFB) {
			_infoMessage->setText("Would you like to unlock additional level? \n Just post a message on your Facebook wall about Animals Zoo and like our fanpage.");
		}
		else {
			_infoMessage->setText("Would you like to unlock additional level? \n Just post a message on your Facebook wall about Animals Zoo.");
		}
		float offset = 2.5;
		spTweenButton button = addButton("close", "CANCEL", Vector2(_view->getWidth() / 2, _view->getHeight() / 2));
		button->setBaseScale((popupBackground->getWidth() * 0.35f) / button->getWidth());
		button->setPosition(Vector2(_view->getWidth() / 2 - button->getDerivedWidth() / 2 - offset, _view->getHeight() * 0.7f));
		
		spTweenButton postMessageButton = createButton("POST ON WALL", "POST");
		postMessageButton->setBaseScale((popupBackground->getWidth() * 0.35f) / button->getWidth());
		postMessageButton->setResAnim(gameResources.getResAnim("button_green"));
		postMessageButton->setPosition(Vector2(_view->getWidth() / 2 + button->getDerivedWidth() / 2 + offset, _view->getHeight() * 0.7f));
		postMessageButton->addEventListener(TouchEvent::TOUCH_DOWN, CLOSURE(this, &WallPostMessagePopup::onWallPostMessage));
		_view->addChild(postMessageButton);

		spActor updatePostStateActor = new Actor();
		updatePostStateActor->setCallbackDoUpdate(_updateCallback);
		updatePostStateActor->attachTo(_view);
	}
	else {
		if (!_infoMessage) {
			_infoMessage = createTextfield("We are really sorry but posting message on your Facebook wall is unavailable.", true, true);
			_infoMessage->setFontSize2Scale(20 * (int)_view->getWidth() / 640);
			_infoMessage->setSize(popupBackground->getHeight() * 0.9f, popupBackground->getHeight() * 0.7f);
			_infoMessage->setPosition(_view->getWidth() / 2, _view->getHeight() * 0.4f);
		}
		if (!_infoMessage->getParent()) {
			_view->addChild(_infoMessage);
		}
		_infoMessage->setText("We are really sorry but posting message on your Facebook wall is unavailable now.");
		spTweenButton button = addButton("close", "CLOSE", Vector2(_view->getWidth() / 2,  _content->getHeight() * 0.8f));
		button->setBaseScale((popupBackground->getWidth() * 0.35f) / button->getWidth());
	}
}

void WallPostMessagePopup::onWallPostMessage(Event *event) {
	if (FacebookManager::instance.isLoggedIn()) {
		_wallPostStatus = WPS_POST;
	}
	else {
		_wallPostStatus = WPS_LOG_IN;
	}
}

void WallPostMessagePopup::_update(const UpdateState &us) {
	switch (_wallPostStatus) {
		case WPS_LOG_IN: {
			FacebookManager::instance.logIn(_isActionFB);

			// Show a message to keep the user informed
			showMessage("FB_PostingToFacebook");

			_wallPostStatus = WPS_WAITING_FOR_LOG_IN;
			break;
		}

		case WPS_WAITING_FOR_LOG_IN: {
			if (FacebookManager::instance.getStatus() == FB_LOGGED_IN) {
				_wallPostStatus = WPS_POST;
			}
			else if (FacebookManager::instance.getStatus() == FB_LOG_IN_FAILED) {
				showMessage("FB_LogInFailed");
			}
			break;
		}

		case WPS_POST: {
			if (_isActionFB) {
				FacebookManager::instance.beginGraphRequest("me/feed", "POST", true);

				// Add a link to the web page for the book
				FacebookManager::instance.addToGraphRequest("link", "https://play.google.com/store/apps/details?id=com.Zelek.ToddlersAnimals");

				// Reference an icon to accompany the post
				FacebookManager::instance.addToGraphRequest("picture", "http://s24.postimg.org/6c3wle535/ic_launcher.png");

				// The message to post
				string wallMessage = "For all my friends with a toddler - check this game out :)";
				FacebookManager::instance.addToGraphRequest("message", wallMessage.c_str());

				// Send it to Facebook
				FacebookManager::instance.finishGraphRequest();

				// Show a message to keep the user informed
				showMessage("FB_PostingToFacebook");
			}
			else {
				FacebookManager::instance.beginDialogRequest("", "");

				FacebookManager::instance.addToDialogRequest("link", "https://play.google.com/store/apps/details?id=com.Zelek.ToddlersAnimals");

				FacebookManager::instance.addToDialogRequest("picture", "http://s24.postimg.org/6c3wle535/ic_launcher.png");

				string wallMessage = "For all my friends with a toddler - check this game out :)";
				FacebookManager::instance.addToDialogRequest("message", wallMessage.c_str());

				FacebookManager::instance.finishDialogRequest();
				showMessage("FB_PostingToFacebook");
			}
			_wallPostStatus = WPS_WAITING_FOR_POST;
			break;
		}

		case WPS_WAITING_FOR_POST:
		{
			if (FacebookManager::instance.getStatus() == FB_GRAPH_REQUEST_SUCCESS) {
				showMessage("FB_PostSuccess");
			}
			else if (FacebookManager::instance.getStatus() == FB_GRAPH_REQUEST_FAILED) {
				showMessage("FB_PostFailed");
			}
			else if (FacebookManager::instance.getStatus() == FB_DIALOG_REQUEST_SUCCESS) {
				showMessage("FB_PostSuccess");
			}
			else if (FacebookManager::instance.getStatus() == FB_DIALOG_REQUEST_FAILED) {
				showMessage("FB_PostFailed");
			}
			break;
		}
	}
}

void WallPostMessagePopup::showMessage(string message) {
	if (message == "FB_PostingToFacebook") {
		_infoMessage->setText("Posting...");
	}
	else if (message == "FB_LogInFailed") {
		_infoMessage->setText("Log in failed :(. Please check your internet connection and try again.");
	}
	else if (message == "FB_PostSuccess") {
		if (_isActionFB) {
			FacebookManager::instance.beginGraphRequest("me/feed", "POST", true);

			// Add a link to the web page for the book
			FacebookManager::instance.addToGraphRequest("likes", "251606461694282");

			// Send it to Facebook
			FacebookManager::instance.finishGraphRequest();
		}
		string name;
		if (_isActionFB) {
			name = "more_permission";
		}
		else {
			name = "less_permission";
		}
		FlurryAnalytics::instance.fbGotFreeLevel(name.c_str());

		_infoMessage->setText("Success :), a new level is available now!");
		FacebookPostMessage wasFBPostSuccessEvent(FacebookPostMessage::FACEBOOK_POST_MESSAGE_SUCCESS);
		dispatchEvent(&wasFBPostSuccessEvent);
		generateAction("close");
	}
	else if (message == "FB_PostFailed") {
		_infoMessage->setText("Oops, post failed. Please check your internet connection and try again.");
	}
}