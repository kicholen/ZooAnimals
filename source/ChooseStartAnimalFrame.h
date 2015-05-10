#ifndef _CHOOSESTARTANIMALFRAME_
#define _CHOOSESTARTANIMALFRAME_

#include "CustomFrame.h"
#include "FlashUtils.h"
#include "AnimalModel.h"

using namespace FlashUtils;

DECLARE_SMART(ChooseStartAnimalFrame, spChooseStartAnimalFrame);

class ChooseStartAnimalFrame : public CustomFrame
{
public:
	ChooseStartAnimalFrame();

	Action loop();
protected:
	void _postHiding(Event *);
	void _preShowing(Event *);

	void selectTransitions();

	void setData();

private:
	bool isInPool(int id);
	void unblockOkButton();

	void onAnimalPicked(Event *ev);

	spSprite createRandomAnimalSprite();

private:
	spShaderTweenButton _okButton;

	VectorArray<int> _inPool;

	spAnimalModel _choosenAnimal;
};

#endif