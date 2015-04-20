#include <stdio.h>

#include "core/oxygine.h"
#include "Stage.h"
#include "DebugActor.h"

#include "Main.h"
#include "FlurryAnalytics.h"

using namespace oxygine;

#ifndef IS_DEBUG
#   define IS_DEBUG 1
#endif

class MainRootActor : public RootActor {
public:
	MainRootActor() {
		addEventListener(RootActor::ACTIVATE, CLOSURE(this, &MainRootActor::onActivate));
		addEventListener(RootActor::DEACTIVATE, CLOSURE(this, &MainRootActor::onDeactivate));
	}

	void onActivate(Event *) {
		core::restore();
		FlurryAnalytics::instance.onSessionStarted();
	}

	void onDeactivate(Event *) {
		FlurryAnalytics::instance.onSessionEnded();
		core::reset();
	}
};

int mainloop() {
	main_update();

	getStage()->update();
	
	if (core::beginRendering())
	{		
		Color clearColor(32, 32, 32, 255);
		Rect viewport(Point(0, 0), core::getDisplaySize());
		getStage()->render(clearColor, viewport);

		core::swapDisplayBuffers();
	}

	bool done = core::update();

	return done ? 1 : 0;
}

void run() {
	ObjectBase::__startTracingLeaks();

	core::init_desc desc;
#if OXYGINE_SDL || OXYGINE_EMSCRIPTEN
	desc.w = 640;
	desc.h = 960;
#endif

	main_preinit();
	core::init(&desc);
	
	Stage::instance = new Stage(true);
	Point size = core::getDisplaySize();
	getStage()->setSize(size);

	if (IS_DEBUG) {
		DebugActor::show();
	}

	main_init();

	#ifdef EMSCRIPTEN
		return;
	#endif

    while (1) {
		int done = mainloop();
		if (done) {
			break;
		}
    }

	//lets dump all created objects into log
	//all created and not freed resources would be displayed
	ObjectBase::dumpCreatedObjects();

	//lets cleanup everything right now and call ObjectBase::dumpObjects() again
	//we need to free all allocated resources and delete all created actors
	//all actors/sprites are smart pointer objects and actually you don't need it remove them by hands
	//but now we want delete it by hands

	//check example.cpp
	main_destroy();

	/**releases all internal components and RootActor*/
	core::release();

	//dump list should be empty now
	//we deleted everything and could be sure that there aren't any memory leaks
	ObjectBase::dumpCreatedObjects();
	//end
	ObjectBase::__stopTracingLeaks();
}

#ifdef __S3E__
int main(int argc, char* argv[])
{
    run();
    return 0;
}
#endif


#ifdef OXYGINE_SDL
#ifdef __MINGW32__
int WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,int nCmdShow)
{
    run();
    return 0;
}
#else
#include "SDL_main.h"
extern "C"
{
	int main(int argc, char* argv[])
	{
		run();
		return 0;
	}
};
#endif
#endif

#ifdef EMSCRIPTEN
#include <emscripten.h>

void one(){ mainloop(); }

int main(int argc, char* argv[])
{
	run();
	emscripten_set_main_loop(one, 0, 0);
	return 0;
}
#endif