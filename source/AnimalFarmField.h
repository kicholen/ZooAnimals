#ifndef _ANIMALFARMFIELD_
#define _ANIMALFARMFIELD_

#include "oxygine-framework.h"
#include "FlashUtils.h"
#include "AnimalInFarmElement.h"
#include "SharedResources.h"
#include "TileField.h"
#include "AnimalsFarmAnimations.h"
#include "AnimalModel.h"
#include "AnimalFarmPanel.h"
#include "TooltipElement.h"
#include "FeederElement.h"

#define BASE_SIZE_IN_PERCENT_X 20
#define BASE_SIZE_IN_PERCENT_Y 8
#define EXPANDED_SIZE_IN_PERCENT_X 30
#define EXPANDED_SIZE_IN_PERCENT_Y 15
#define OFFSET_EDGES 1

#define TOOLTIP_START_SHOW 5000
#define TOOLTIP_RESHOW_MIN 10000
#define TOOLTIP_RESHOW_MAX 15000

using namespace FlashUtils;
using namespace oxygine;

DECLARE_SMART(AnimalFarmField, spAnimalFarmField);

typedef enum {afCreating, afWaiting, afAnimating} AnimalFarmState;

class AnimalFarmField : public Actor
{
public:
	AnimalFarmField(Vector2 fieldSize);
	~AnimalFarmField();

	void setData(spAnimalModel model);
	void addAnimal(Event *event);
	spTileField createTileField();
	void playNextAnimalsAnimation(Event *event);

	spAnimalModel getModel() {
		return _model;
	}

	Point getNumberOfTiles();

protected:
	virtual void doUpdate(const UpdateState &us);
private:
	void createSortElements(spTileField tileField);
	void createFenceAtBottom(spTileField tileField);
	void createFenceAtLeft(spTileField tileField);
	void createInformationTable(spTileField tileField);
	void createFeeder();

	spAnimalInFarmElement createAnimal(const std::string& animalNumber, spAnimalModel model);
	void removeLastAnimal();
	spButton createAnimalButton(const std::string& buttonName, Vector2 position);
	void animateAnimalsJump(Vector2 position);
	bool canAnimalsAnimate();
	void setAnimalsPriorityByY();

	spTooltipElement createTooltipElement();
	int getLockitId();

	void onTouchOver(Event *event);
	void onGameChosen(Event *event);
	void onAnimalCountChanged(Event *ev);

	VectorArray<spSprite> _zSortElements;
	AnimalFarmState _state;
	spAnimalFarmPanel _animalPanel;
	spFeederElement _feederElement;

	AnimalsFarmAnimationType _animationType;
	spAnimalsFarmAnimations _animalsFarmAnimation;
	spAnimalModel _model;
	spTooltipElement _tooltip;

	int _count;
	float _lastTooltipShowTime;
};


#endif