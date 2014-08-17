#ifndef _TWEENBUTON_ 
#define _TWEENBUTON_

#include "oxygine_include.h"
#include "Button.h"

namespace oxygine
{
	DECLARE_SMART(TweenButton, spTweenButton);
	class TweenButton : public Button
	{
	public:
		TweenButton();
		~TweenButton();
		void setBaseScale(float scale);

	protected:
		virtual void updateButtonState(state s);
	private:
		spTween _tween;
		Tween::EASE _ease;
		int _duration;
		float _baseScale;
		bool _wasBaseScaleSet;
	};

}

#endif

