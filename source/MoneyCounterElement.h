#ifndef _MONEYCOUNTERELEMENT_
#define _MONEYCOUNTERELEMENT_

#include "oxygine-framework.h"
#include "TweenTextField.h"

#define ANIMATION_TIME 500

using namespace oxygine;

typedef enum {mceBottom, mceTop} MoneyCounterElementState;

DECLARE_SMART(MoneyCounterElement, spMoneyCounterElement);

class MoneyCounterElement : public Actor
{
public:
	MoneyCounterElement(MoneyCounterElementState state = mceTop);
	~MoneyCounterElement();

	void setData();
	void show(bool shouldAnimate = true);
	void hide(bool shouldAnimate = false);

	void setDisplayState(MoneyCounterElementState state);

protected:
	virtual void doUpdate(const UpdateState &us);
private:
	void createBackground();
	spTweenTextField createMoneyTextfield(int money);
	spSprite createMoneyIcon(const string& spriteName);

	int getBronzeMoney();
	int getSilverMoney();
	int getGoldMoney();
	void updateTextFields();
	void updatePosition();

	void onMoneyChanged(Event *ev);

private:
	Vector2 _hidePosition;
	Vector2 _showPosition;

	spBox9Sprite _background;
	spTweenTextField _bronzeTextField;
	spTweenTextField _silverTextField;
	spTweenTextField _goldTextField;
	spSprite _bronzeSprite;
	spSprite _silverSprite;
	spSprite _goldSprite;

	int _money;
	int _bronzeMoney;
	int _silverMoney;
	int _goldMoney;

	float _updateSizeTime;
};

#endif