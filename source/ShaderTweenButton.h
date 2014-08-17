#ifndef _SHADERTWEENBUTTON_ 
#define _SHADERTWEENBUTTON_

#include "oxygine_include.h"
#include "ShaderButton.h"

namespace oxygine
{
	DECLARE_SMART(ShaderTweenButton, spShaderTweenButton);


	class ShaderTweenButton : public ShaderButton
	{
	public:
		ShaderTweenButton();
		~ShaderTweenButton();
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

