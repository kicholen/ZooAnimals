#include "FacebookManager.h"

FacebookManager FacebookManager::instance;

FacebookManager::FacebookManager() : _currentSession(NULL), _currentRequest(NULL) {
	_loggedIn = false;
	_status = FB_NOT_LOGGED_IN;
}

FacebookManager::~FacebookManager() {
	/*deleteCurrentRequest();

	if (_currentSession) {
		s3eFBTerminate(_currentSession);
		_currentSession = NULL;
	}*/
}

void FacebookManager::init() {
	/*_isFacebookAvailable = s3eFacebookAvailable() == S3E_TRUE;
	if (_isFacebookAvailable) {
		s3eDebugOutputString(CMath::stringFormat(" API FB - available : %s", "true").c_str());
	}
	else {
		s3eDebugOutputString(CMath::stringFormat(" API FB - available : %s", "false").c_str());
	}*/
}

void FacebookManager::logIn(bool withActionPermisstion) {
	/*if (_isFacebookAvailable) {
		char lAppId[S3E_CONFIG_STRING_MAX];
		s3eConfigGetString("APP", "FacebookAppId", lAppId);
		//s3eDebugOutputString(CMath::stringFormat(" API FB - Trying to log in by appId: %s", lAppId).c_str());

		if (!_currentSession) {
			_currentSession = s3eFBInit(lAppId);
		}

		if (_currentSession) {
			if (withActionPermisstion) {
				const char* permissions[] = { "publish_stream", "publish_actions" };
				s3eFBSession_Login(_currentSession, FacebookManager::loginCallback, NULL, permissions, sizeof(permissions)/sizeof(permissions[0]));
			}
			else {
				const char* permissions[] = { "publish_stream" };//, "publish_actions"
				s3eFBSession_Login(_currentSession, FacebookManager::loginCallback, NULL, permissions, sizeof(permissions)/sizeof(permissions[0]));
			}
			_status = FB_LOGGING_IN;
		}
		else {
			_status = FB_LOG_IN_FAILED;
		}
	}*/
}

void FacebookManager::logOut() {
	/*if (_isFacebookAvailable && _currentSession) {
		s3eFBSession_Logout(_currentSession);
		_loggedIn = false;
		_status = FB_NOT_LOGGED_IN;
	}*/
}

void FacebookManager::loginCallback(struct s3eFBSession* session, s3eResult* loginResult, void *userData) {
	/*if (*loginResult == S3E_RESULT_SUCCESS) {
		FacebookManager::instance._status = FB_LOGGED_IN;
		FacebookManager::instance._loggedIn = true;
	}
	else {
		FacebookManager::instance._status = FB_LOG_IN_FAILED;
		FacebookManager::instance._loggedIn = false;
	}*/
}

void FacebookManager::beginDialogRequest(const char* graphPath, const char* httpMethod, bool useAccessToken) {
	/*if (_currentDialog) {
		return;
	}

	if (_isFacebookAvailable && _currentSession) {
		_currentDialog = s3eFBDialog_WithAction(_currentSession, "stream.publish");
		
		if (_currentDialog && useAccessToken) {
			char lAppId[S3E_CONFIG_STRING_MAX];
			s3eConfigGetString("APP", "FacebookAppSecret", lAppId);
			addToDialogRequest("api_key", lAppId);
		}
	}*/
}

void FacebookManager::addToDialogRequest(const char* param, const char* data) {
	/*if (_isFacebookAvailable && _currentSession && _currentDialog) {
		s3eFBDialog_AddParamString(_currentDialog, param, data);
	}*/
}

void FacebookManager::finishDialogRequest() {
	/*if (_isFacebookAvailable && _currentSession && _currentDialog) {
		if (s3eFBDialog_Show(_currentDialog, FacebookManager::dialogRequestCallback, NULL) == S3E_RESULT_SUCCESS) {
			_status = FB_DIALOG_REQUEST_SENDING;
		}
		else {
			deleteCurrentDialog();
			_status = FB_DIALOG_REQUEST_FAILED;
		}
	}*/
}

void FacebookManager::beginGraphRequest(const char* graphPath, const char* httpMethod, bool useAccessToken) {
	/*if (_currentRequest) {
		return;
	}

	if (_isFacebookAvailable && _currentSession) {
		_currentRequest = s3eFBRequest_WithGraphPath(_currentSession, graphPath, httpMethod);
		
		if (_currentRequest && useAccessToken) {
			addToGraphRequest("access_token", s3eFBSession_AccessToken(_currentSession));
		}
	}*/
}

void FacebookManager::addToGraphRequest(const char* param, const char* data) {
	/*if (_isFacebookAvailable && _currentSession && _currentRequest) {
		s3eFBRequest_AddParamString(_currentRequest, param, data);
	}*/
}

void FacebookManager::finishGraphRequest() {
	/*if (_isFacebookAvailable && _currentSession && _currentRequest) {
		if (s3eFBRequest_Send(_currentRequest, FacebookManager::graphRequestCallback, NULL) == S3E_RESULT_SUCCESS) {
			_status = FB_GRAPH_REQUEST_SENDING;
		}
		else {
			deleteCurrentRequest();
			_status = FB_GRAPH_REQUEST_FAILED;
		}
	}*/
}

void FacebookManager::dialogRequestCallback(struct s3eFBDialog* request, s3eResult* requestResult, void* userData) {
	/*if (*requestResult == S3E_RESULT_SUCCESS) {
		FacebookManager::instance._status = FB_DIALOG_REQUEST_SUCCESS;
	}
	else {
		FacebookManager::instance._status = FB_DIALOG_REQUEST_FAILED;
	}
	FacebookManager::instance.deleteCurrentDialog();*/
}

void FacebookManager::deleteCurrentDialog() {
	/*if (_currentDialog) {
		s3eFBDialog_Delete(_currentDialog);
		_currentDialog = NULL;
	}*/
}

void FacebookManager::graphRequestCallback(struct s3eFBRequest* request, s3eResult* requestResult, void* userData) {
	if (*requestResult == S3E_RESULT_SUCCESS) {
		FacebookManager::instance._status = FB_GRAPH_REQUEST_SUCCESS;
	}
	else {
		FacebookManager::instance._status = FB_GRAPH_REQUEST_FAILED;
	}
	FacebookManager::instance.deleteCurrentRequest();
}

void FacebookManager::deleteCurrentRequest() {
	/*if (_currentRequest) {
		s3eFBRequest_Delete(_currentRequest);
		_currentRequest = NULL;
	}*/
}