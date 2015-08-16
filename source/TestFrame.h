#pragma once
#include "CustomFrame.h"


DECLARE_SMART(TestFrame, spTestFrame);

class TestFrame : public CustomFrame
{
public:
	TestFrame();
	~TestFrame();

	Action loop();
protected:
	void _postHiding(Event *);
	void _preShowing(Event *);

	void selectTransitions();

private:
	spTweenButton createButtonWithListener(const std::string& actionName, const std::string& text);
	void setData();

};

