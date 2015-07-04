#ifndef _LOADINGFRAME_
#define _LOADINGFRAME_

#include "CustomFrame.h"
#include "ProgressBar.h"

DECLARE_SMART(LoadingFrame, spLoadingFrame);

class LoadingFrame : public CustomFrame
{
public:
	LoadingFrame();

	Action loop();
protected:
	void _postHiding(Event *);
	void _preShowing(Event *);
	void selectTransitions();
	void setData();

private:
	void initAnalytics();
	void initSettings(Event *ev);
	void initManagers(Event *ev);
	void initSoundPlayer(Event *ev);

	void loadGameResource();
	void loadAnimalRecource();
	void loadEditResource();
	void loadTileResource();

	void devPurposes();

	void loadedResource(Event *ev);
	void loadNext();
private:
	std::string _version;

	spProgressBar _progressBar;

	int _resourcesCounter;
};

#endif
