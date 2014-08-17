#ifndef _PROGRESSBARBOX9_
#define _PROGRESSBARBOX9_

#include "oxygine-framework.h"

using namespace oxygine;

DECLARE_SMART(ProgressBarBox9, spProgressBarBox9);

class ProgressBarBox9 : public Actor
{
public:
	ProgressBarBox9(Vector2 size);

	void show();
	void hide();

	void setProgress(float value);
	float getProgress() const;

	typedef GetSet<float, float, ProgressBarBox9, &ProgressBarBox9::getProgress, &ProgressBarBox9::setProgress> Tween9Progress;		
private:
	void createBackground();
	void createForeground();

	void createMask();
	void createMaskedSprite();

	spMaskedSprite _masked;
	spSprite _mask;

	//timeMS _timerMs;
	//int _timerLimit;

	float _progress;
};

#endif