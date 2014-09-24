#include "Actor.h"
#include "Sprite.h"
#include "Button.h"
#include "RootActor.h"
#include "res/Resources.h"
#include "Blocking.h"
#include "Main.h"
#include "LandingPageFrame.h"
#include "MaskFrame.h"
#include "SlideFrame.h"
#include "SoundPlayer.h"
#include "SoundSystem.h"
#include "core/STDFileSystem.h"
#include "oxygine-framework.h"
#include "SharedResources.h"

#include "Content.h"
#include "Settings.h"
#include "FlurryAnalytics.h"
#include "FlurryAds.h"
//#include "LeadBoltManager.h"
#include "GooglePlayInAppPurchaseManager.h"
#include "FacebookManager.h"

using namespace oxygine;

int mainloop();

SoundPlayer sPlayer;

DECLARE_SMART(MainActor, spMainActor);

spLandingPageFrame landingPageFrame;
spSlideFrame slideFrame;
spBuyItemPopup buyItemPopup;
spWallPostMessagePopup wallPostPopup;

void main_preinit() {
}

void pause(Event *) {
}

void resume(Event *) {
}

file::STDFileSystem extfs(true);

void main_init() {
	extfs.setPath(file::fs().getFullPath("ext").c_str());
	file::mount(&extfs);

	blocking::setYieldCallback(mainloop);

	Content::instance.init("1");
	// opcje, dŸwiêku, odblokowane funkcjonalnoœci
	Settings::instance.init("1");
	Settings::instance.getValue("sounds").set_value(60);
	Settings::instance.getValue("music").set_value(100);
	Settings::instance.getValue("level_1").set_value(1);
	Settings::instance.getValue("level_2").set_value(1);
	Settings::instance.getValue("level_3").set_value(1);
	bool whichFb;
	if (FlashUtils::CMath::Rand(10, 100) < 50) {
		whichFb = true;
	}
	else {
		whichFb = false;
	}
	Settings::instance.getValue("action_fb").set_value(whichFb);

	Settings::instance.save();

	// analityka
	FlurryAnalytics::instance.init();
	FlurryAnalytics::instance.onSessionStarted();

	//ads
	FlurryAds::instance.init();
	FlurryAds::instance.tryToShowFullScreen(); // load add
	//LeadBoltManager::instance.init();

	// managers
	GooglePlayInAppPurchaseManager::instance.init();
	FacebookManager::instance.init();

	Frame::initialize();

	//initialize our sound system with 16 channels
	SoundSystem::instance = SoundSystem::create();
	SoundSystem::instance->init(4);

	//initialize SoundPlayer
	SoundPlayer::initialize();
	sPlayer.setResources(&gameResources);

	//load xml file with resources definition
	gameResources.loadXML("res.xml");
	animalsResources.loadXML("xmls/AnimalsResources.xml");

	editResources.loadXML("edit_res.xml");
	// for edit app
	Point *ad = new Point(getRoot()->getWidth(), getRoot()->getHeight());
	SpriteSpawner::Initialize(*ad);

	spTransition tr = new TransitionSimple();

	landingPageFrame = new LandingPageFrame();
	slideFrame = new SlideFrame();
	buyItemPopup = new BuyItemPopup();
	wallPostPopup = new WallPostMessagePopup();

	try {
		transitionSwitchFrame(landingPageFrame, tr);
	}
	catch (const Exit &) {	
	}	

	core::requestQuit();
}

void main_update() {
	SoundSystem::instance->update();
	sPlayer.update();
}

void main_destroy() {
	sPlayer.stop();
	SoundSystem::instance->stop();
	FlurryAnalytics::instance.onSessionEnded();
	//delete &FlurryAnalytics::instance;
	//delete &FlurryAds::instance;
	//delete &GooglePlayInAppPurchaseManager::instance;
	//delete &FacebookManager::instance;

	landingPageFrame = 0;
	slideFrame = 0;
	buyItemPopup = 0;
	wallPostPopup = 0;

	Frame::free();

	gameResources.free();
	SoundSystem::instance->release();
}