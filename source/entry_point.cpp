#include <stdio.h>

#include "core/Renderer.h"
#include "RootActor.h"
#include "DebugActor.h"

#include "Main.h"
#include "FlurryAnalytics.h"
#include "Settings.h"

using namespace oxygine;

Renderer renderer;
Rect viewport;

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
		core::reset();
		FlurryAnalytics::instance.onSessionEnded();
		Settings::instance.save();
	}
};

int mainloop() {
	main_update();

	getRoot()->update();

	Color clear(10, 10, 10, 255);

	if (renderer.begin(0, viewport, &clear)) {
		getRoot()->render(renderer);
		renderer.end();
		core::swapDisplayBuffers();
	}

	bool done = core::update();

	return done ? 1 : 0;
}

void run() {
	core::init_desc desc;
	main_preinit();
#if OXYGINE_SDL
	desc.w = 640;
	desc.h = 960;
#endif

	core::init(&desc);
	
	RootActor::instance = new MainRootActor();
	Point size = core::getDisplaySize();
	getRoot()->init(size, size);

	if (IS_DEBUG) {
		DebugActor::initialize();

		getRoot()->addChild(new DebugActor());
	}

	Matrix view = makeViewMatrix(size.x, size.y);

	viewport = Rect(0, 0, size.x, size.y);

	Matrix proj;
	Matrix::orthoLH(proj, (float)size.x, (float)size.y, 0, 1);

	//Renderer is class helper for rendering primitives and batching them
	//Renderer is lightweight class you could create it many of times
	renderer.setDriver(IVideoDriver::instance);

	//initialization view and projection matrix
	//where Left Top corner is (0, 0), and right bottom is (width, height)
	renderer.initCoordinateSystem(size.x, size.y);

	main_init();

	bool done = false;

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

	renderer.cleanup();

	/**releases all internal components and RootActor*/
	core::release();

	//dump list should be empty now
	//we deleted everything and could be sure that there aren't any memory leaks
	ObjectBase::dumpCreatedObjects();
	//end
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

#ifdef __FLASHPLAYER__
int main(int argc, char* argv[])
{
	printf("test\n");
	run();
	return 0;
}
#endif
