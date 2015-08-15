#ifndef _LEVELUPITEM_
#define _LEVELUPITEM_

#include "oxygine-framework.h"
#include "RewardModel.h"
#include "StackContainer.h"

#define OFFSET 20

using namespace oxygine;

DECLARE_SMART(LevelUpItem, spLevelUpItem);

class LevelUpItem : public Actor // todo change name of this
{
public:
	LevelUpItem(const Vector2& size, int requiredLevel, bool shouldAddStuff = true);
	~LevelUpItem();

	void addReward(spRewardModel model);

private:
	void createMainTextfield(int requiredLevel);
	void createBackground();
	void createStackContainer();
	
	spTextField createCountTextfield(int count);
	spSprite createRewardSprite(spRewardModel model);

private:
	spStackContainer _stackContainer;
	Vector2 _countTextfieldSize;
};

#endif