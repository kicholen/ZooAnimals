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
		void setScale(float scale);

	protected:
		void doUpdate(const UpdateState &us);
		virtual void updateButtonState(state s);
	private:
		void setBasicScale(float scale);

		spTween _tween;
		Tween::EASE _ease;
		int _duration;
		float _baseScale;
		bool _wasBaseScaleSet;
	};

}

#endif

