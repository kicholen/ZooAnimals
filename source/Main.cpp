#include "RootActor.h"
#include "res/Resources.h"
#include "Frame.h"
#include "Transition.h"
#include "Blocking.h"
#include "Main.h"
#include "SoundPlayer.h"
#include "SoundSystem.h"
#include "oxygine-framework.h"
#include "core/STDFileSystem.h"

#include "FlurryAnalytics.h"
#include "LoadingFrame.h"
#include "SharedResources.h"

using namespace oxygine;

int mainloop();

SoundPlayer sPlayer;

file::STDFileSystem extfs(true);

DECLARE_SMART(MainActor, spMainActor);

void main_preinit() {
}

void pause(Event *) {
}

void resume(Event *) {
}

void main_init() {
	blocking::setYieldCallback(mainloop);

	extfs.setPath(file::fs().getFullPath("ext").c_str());
	file::mount(&extfs);

	//initialize our sound system with 16 channels
	SoundSystem::instance = SoundSystem::create();
	SoundSystem::instance->init(4);

	SoundPlayer::initialize();

	Frame::initialize();

	spTransition tr = new TransitionSimple();
	spLoadingFrame loadingFrame = new LoadingFrame();

	try {
		transitionSwitchFrame(loadingFrame, tr);
	}
	catch (const Exit &) {

	}

	core::requestQuit();
}

void onRequestExit(Event *ev) {

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

	Frame::free();

	editResources.free();
	tilesResources.free();
	animalsResources.free();
	gameResources.free();
	SoundSystem::instance->release();
}