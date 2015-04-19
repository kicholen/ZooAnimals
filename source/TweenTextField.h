#ifndef _TWEENTEXTFIELD_
#define _TWEENTEXTFIELD_

#include "oxygine-framework.h"

using namespace oxygine;

DECLARE_SMART(TweenTextField, spTweenTextField);

class TweenTextField : public TextField
{
public:
	TweenTextField();
	~TweenTextField();

	void setValue(int value);
	int getValue() const;

	typedef GetSet<int, int, TweenTextField, &TweenTextField::getValue, &TweenTextField::setValue> TweenTextInt;

private:
	int _value;
};

#endif