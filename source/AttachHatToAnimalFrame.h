#ifndef _ATTACHHATTOANIMALFRAME_
#define _ATTACHHATTOANIMALFRAME_

#include "CustomFrame.h"
#include "StackContainer.h"

#define OFFSET_ATTACH_HAT 30
#define HATS_IN_ROW 4

DECLARE_SMART(AttachHatToAnimalFrame, spAttachHatToAnimalFrame);

class AttachHatToAnimalFrame : public CustomFrame
{
public:
	AttachHatToAnimalFrame(const std::string& animal);
	~AttachHatToAnimalFrame();

	void selectTransitions();

	Action loop();

protected:
	void _postHiding(Event *);
	void _preShowing(Event *);

private:
	void setData();

	void createPopupBackground();
	void createTitleTextfield();
	void createHats();
	spTweenButton createHatButton(const std::string& name);

	void onHatClicked(Event *event);

private:
	std::string _animal;
	spStackContainer _container;
};

#endif