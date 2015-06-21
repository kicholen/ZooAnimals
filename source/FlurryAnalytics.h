#ifndef _GOOGLEANALYTICS_
#define _GOOGLEANALYTICS_

//#include "s3eFlurry.h"
#include "s3eDevice.h"

namespace oxygine
{
	class FlurryAnalytics
	{
	public:
		static FlurryAnalytics instance;

		FlurryAnalytics();
		~FlurryAnalytics();

		void init();
		void onSessionStarted();
		void onSessionEnded();

		void sendTapEvent();

		void sendLoadingTime(const char* whichFrame);
		void stopLoadingTime(const char* whichFrame);

		void onAnimatableObjectTapEvent(const char* whichFrame, const char* whichObject);
		void onLevelEnterEvent(const char* whichFrame);
		void onLevelLeaveEvent(const char* whichFrame);

		void fbAskForPermission(const char* whichPermission);
		void fbGotFreeLevel(const char* whichPermission);
	private:
		s3eBool _isFlurryAvaiable;
	};

}
#endif