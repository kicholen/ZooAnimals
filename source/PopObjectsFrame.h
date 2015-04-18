#ifndef _POPOBJECTSFRAME_
#define _POPOBJECTSFRAME_

#include "CustomFrame.h"
#include "PopObjectsField.h"
#include "SwipeTrailElement.h"

DECLARE_SMART(PopObjectsFrame, spPopObjectsFrame);

class PopObjectsFrame : public CustomFrame
{
public:
	PopObjectsFrame(const string& difficulty);
	~PopObjectsFrame();
	void selectTransitions();

	Action loop();
protected:
	void _postHiding(Event *);
	void _preShowing(Event *);

private:
	void onFinished(Event *event);

	void setData();
	void addSwipeTrail();
	void addPopField();

private:
	spSwipeTrailElement _swipeTrailElement;
	spPopObjectsField _popField;

	string _difficulty;
};

#endif