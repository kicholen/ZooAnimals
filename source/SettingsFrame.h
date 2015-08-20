#ifndef _SETTINGSFRAME_
#define _SETTINGSFRAME_

#include "CustomFrame.h"

DECLARE_SMART(SettingsFrame, spSettingsFrame);

class SettingsFrame : public CustomFrame
{
public:
	SettingsFrame();
	~SettingsFrame();
	void selectTransitions();

	Action loop();

protected:
	void _postHiding(Event *);
	void _preShowing(Event *);

private:
	void setData();
	spTextField createTitleTextfield();
	spTextField createNormalTextfield(int lockitId);
	void createPopupBackground();
	spButton createSwitchButton(const std::string& name, bool isOn);

	void onButtonClicked(Event *event);
private:
	bool _sound;
	bool _music;
};

#endif