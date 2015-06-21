#include "FlurryAnalytics.h"
#include "string"

using namespace std;

namespace oxygine
{
	FlurryAnalytics FlurryAnalytics::instance;

	FlurryAnalytics::FlurryAnalytics() {
		_isFlurryAvaiable = false;
	}

	FlurryAnalytics::~FlurryAnalytics() {

	}

	void FlurryAnalytics::init() {
		/*_isFlurryAvaiable = s3eFlurryAvailable();
		if (!_isFlurryAvaiable) {
			return;
		}*/
	}

	void FlurryAnalytics::onSessionStarted() {
		/*if (_isFlurryAvaiable) {
			if (s3eDeviceGetInt(S3E_DEVICE_OS) == S3E_OS_ID_ANDROID) {
				s3eFlurryStartSession("9G322SKQ539264W5X55N");
			}
		}*/
	}

	void FlurryAnalytics::onSessionEnded() {
		/*if (_isFlurryAvaiable) {
			s3eFlurryEndSession();
		}*/
	}

	void FlurryAnalytics::onAnimatableObjectTapEvent(const char* whichFrame, const char* whichObject) {
		/*if (_isFlurryAvaiable) {
			string param = "|1";
			string eventParams = whichObject + param;
			s3eFlurryLogEventParams(whichFrame, eventParams.c_str());
		}*/
	}

	void FlurryAnalytics::fbAskForPermission(const char* whichPermission) {
		/*if (_isFlurryAvaiable) {
			string param = "|1";
			string eventParams = whichPermission + param;
			s3eFlurryLogEventParams("fbPopup", eventParams.c_str());
		}*/
	}

	void FlurryAnalytics::fbGotFreeLevel(const char* whichPermission) {
		/*if (_isFlurryAvaiable) {
			string param = "|1";
			string eventParams = whichPermission + param;
			s3eFlurryLogEventParams("fbPopup", eventParams.c_str());
		}*/
	}

	void FlurryAnalytics::onLevelEnterEvent(const char* whichFrame) {
		/*if (_isFlurryAvaiable) {
			string param = "|1";
			string eventParams = whichFrame + param;
			s3eFlurryLogEventParamsTimed("inLevelTime", eventParams.c_str());
		}*/
	}

	void FlurryAnalytics::onLevelLeaveEvent(const char* whichFrame) {
		/*if (_isFlurryAvaiable) {
			string param = "|1";
			string eventParams = whichFrame + param;
			s3eFlurryEndTimedEvent("inLevelTime", eventParams.c_str());
		}*/
	}
}