#ifndef _FLURRYADS_
#define _FLURRYADS_

//#include "s3eFlurryAppSpot.h"
#include "s3e.h"

class FlurryAds
{
public:
	static FlurryAds instance;

	FlurryAds();
	~FlurryAds();

	void init();

	void tryToShowFullScreen();
		
	void tryToShowBannerTop();
	void hideBannerTop();

	void tryToShowBannerBottom();
	void hideBannerBottom();
protected:
	void registerListeners() {
		/*s3eFlurryAppSpotRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADCLOSED, (s3eCallback)adClosedCallback, NULL);
		s3eFlurryAppSpotRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADSPACE_RECEIVED, (s3eCallback)adSpaceReceivedCallback, NULL);
		s3eFlurryAppSpotRegister(S3E_FLURRYAPPSPOT_CALLBACK_APPLICATION_EXIT, (s3eCallback)applicationExitCallback, NULL);
		s3eFlurryAppSpotRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADSPACE_RECEIVED_FAIL, (s3eCallback)adSpaceReceivedFailCallback, NULL);
		s3eFlurryAppSpotRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADCLICKED, (s3eCallback)adClickedCallback, NULL);
		s3eFlurryAppSpotRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADOPENED,(s3eCallback) adOpenedCallback, NULL);
		s3eFlurryAppSpotRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADVIDEOCOMPLETED, (s3eCallback)adVideoCompletedCallback, NULL);
		s3eFlurryAppSpotRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADRENDERFAIL, (s3eCallback)adRenderFailCallback, NULL);*/
	}
	void unregisterListeners() {
		/*s3eFlurryAppSpotUnRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADCLOSED, (s3eCallback)adClosedCallback);
		s3eFlurryAppSpotUnRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADSPACE_RECEIVED, (s3eCallback)adSpaceReceivedCallback);
		s3eFlurryAppSpotUnRegister(S3E_FLURRYAPPSPOT_CALLBACK_APPLICATION_EXIT, (s3eCallback)applicationExitCallback);
		s3eFlurryAppSpotUnRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADSPACE_RECEIVED_FAIL, (s3eCallback)adSpaceReceivedFailCallback);
		s3eFlurryAppSpotUnRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADCLICKED, (s3eCallback)adClickedCallback);
		s3eFlurryAppSpotUnRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADOPENED,(s3eCallback) adOpenedCallback);
		s3eFlurryAppSpotUnRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADVIDEOCOMPLETED, (s3eCallback)adVideoCompletedCallback);
		s3eFlurryAppSpotUnRegister(S3E_FLURRYAPPSPOT_CALLBACK_ADRENDERFAIL, (s3eCallback)adRenderFailCallback);*/
	}
private:
	static int32 adClosedCallback(void *systemData, void *userData) {
		s3eDebugOutputString("Marmalade-FlurryAdClosedCallback");
		return 0;
	}

	static int32 adSpaceReceivedCallback(void* systemData, void* userData) {
		s3eDebugOutputString("Marmalade-FlurryAdClosedCallback");
		return 0;
	}

	static int32 applicationExitCallback(void* system, void* user) {
		s3eDebugOutputString("Marmalade-FlurryApplicationExitCallback");
		return 0;
	}

	static int32 adSpaceReceivedFailCallback(void* system, void* user) {
		s3eDebugOutputString("Marmalade-FlurryAdSpaceReceivedFailCallback");
		return 0;
	}

	static int32 adClickedCallback(void* system, void* user) {
		s3eDebugOutputString("Marmalade-FlurryAdClickedCallback");
		return 0;
	}

	static int32 adOpenedCallback(void* system, void* user) {
		s3eDebugOutputString("Marmalade-FlurryAdOpenedCallback");
		return 0;
	}

	static int32 adVideoCompletedCallback(void* system, void* user) {
		s3eDebugOutputString("Marmalade-FlurryAdVideoCompleted_Callback");
		return 0;
	}

	static int32 adRenderFailCallback(void* system, void* user) {
		s3eDebugOutputString("Marmalade-FlurryAdRenderFail_Callback");
		return 0;
	}

private:
	s3eBool _isFlurryAdsAvailable;
};


#endif