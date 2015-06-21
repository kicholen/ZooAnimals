#ifndef _FACEBOOKMANAGER_
#define _FACEBOOKMANAGER_

//#include "s3eFacebook.h"
#include "s3e.h"
#include "FlashUtils.h"

using namespace FlashUtils;

enum FBStatus
{
	FB_NOT_LOGGED_IN,
	FB_LOGGING_IN,
	FB_LOGGED_IN,
	FB_LOG_IN_FAILED,
	FB_GRAPH_REQUEST_FAILED,
	FB_GRAPH_REQUEST_SENDING,
	FB_GRAPH_REQUEST_SUCCESS,
	FB_DIALOG_REQUEST_FAILED,
	FB_DIALOG_REQUEST_SENDING,
	FB_DIALOG_REQUEST_SUCCESS
};

class FacebookManager
{
public:
	static FacebookManager instance;

	FacebookManager();
	~FacebookManager();

	void init();

	void logIn(bool withActionPermisstion);
	void logOut();

	void beginDialogRequest(const char* dialogPath, const char* httpMethod, bool useAccessToken = true);
	void addToDialogRequest(const char* param, const char* data);
	void finishDialogRequest();

	void beginGraphRequest(const char* graphPath, const char* httpMethod, bool useAccessToken = true);
	void addToGraphRequest(const char* param, const char* data);
	void finishGraphRequest();

	bool isAvailable() const {
		return _isFacebookAvailable;
	}

	bool isLoggedIn() const {
		return _loggedIn;
	}

	FBStatus getStatus() const {
		return _status;
	}

private:
	// Callback functions
	static void loginCallback(struct s3eFBSession* session, s3eResult* loginResult, void* userData);
	static void graphRequestCallback(struct s3eFBRequest* request, s3eResult* requestResult, void* userData);
	static void dialogRequestCallback(struct s3eFBDialog* dialog, s3eResult* requestResult, void* userData);
	
	// Private methods
	void deleteCurrentRequest();
	void deleteCurrentDialog();

	s3eFBSession* _currentSession;
	s3eFBDialog* _currentDialog;
	s3eFBRequest* _currentRequest;
	bool _loggedIn;
	bool _isFacebookAvailable;
	FBStatus _status;
};

#endif