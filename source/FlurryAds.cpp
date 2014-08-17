#include "FlurryAds.h"
#include "Settings.h"

FlurryAds FlurryAds::instance;

FlurryAds::FlurryAds() {
	_isFlurryAdsAvailable = S3E_FALSE;
}

FlurryAds::~FlurryAds() {
	if (_isFlurryAdsAvailable) {
		unregisterListeners();
	}
}

void FlurryAds::init() {
	_isFlurryAdsAvailable = s3eFlurryAppSpotAvailable();
	if (_isFlurryAdsAvailable) {
		s3eFlurryAppSpotInitialize("9G322SKQ539264W5X55N");
		registerListeners();
	}
}

void FlurryAds::tryToShowFullScreen() {
	if (_isFlurryAdsAvailable && Settings::instance.getValue("unblock_ads").as_int() == 0) {
		s3eDebugTracePrintf("showBannerFullScreen");
		s3eFlurryAppSpotFetchAndDisplayAdForSpace("Animals_Banner_Fullscreen", S3E_FLURRYAPPSPOT_FULLSCREEN);
	}
}

void FlurryAds::tryToShowBannerTop() {
	if (_isFlurryAdsAvailable) {
		//s3eDebugTracePrintf("showBannerTop");
		//s3eFlurryAppSpotSetDisplayAdView(0, 0, 320, 50);
		//s3eFlurryAppSpotFetchAndDisplayAdForSpace("Animals_Banner_Top", S3E_FLURRYAPPSPOT_BANNER_TOP);
	}
}

void FlurryAds::hideBannerTop() {
	if (_isFlurryAdsAvailable) {
		//s3eDebugTracePrintf("hideBannerTop");
		//s3eFlurryAppSpotRemoveAdFromSpace("Animals_Banner_Top");
	}
}

void FlurryAds::tryToShowBannerBottom() {
	if (_isFlurryAdsAvailable) {
		//s3eDebugTracePrintf("showBannerBottom");
		//s3eFlurryAppSpotSetDisplayAdView(0, 1, 320, 50);
		//s3eFlurryAppSpotFetchAndDisplayAdForSpace("Animals_Banner_Bottom", S3E_FLURRYAPPSPOT_BANNER_BOTTOM);
	}
}

void FlurryAds::hideBannerBottom() {
	if (_isFlurryAdsAvailable) {
		//s3eDebugTracePrintf("removeBannerBottom");
		//s3eFlurryAppSpotRemoveAdFromSpace("Animals_Banner_Bottom");
	}
}