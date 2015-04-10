#include "TweenButton.h"

namespace oxygine
{	
	TweenButton::TweenButton() {
		//Button::Button();
		_ease = Tween::ease_outBack;
		_duration = 200;
		_tween = 0;
		_wasBaseScaleSet = false;
	}

	TweenButton::~TweenButton() {
		//Button::~Button();
	}

	void TweenButton::updateButtonState(state s) {
		if (!_resAnim)
			return;

		if (_wasBaseScaleSet) {
			if (s == 2) {
				//this->removeTweens();
				_tween = createTween(Actor::TweenScale(_baseScale * 0.85f), _duration, 1, false);
				_tween->setEase(_ease);
				this->addTween(_tween);
			}
			else if (s == 0) {
				//this->removeTweens();
				_tween = createTween(Actor::TweenScale(_baseScale), _duration, 1, false);
				_tween->setEase(_ease);
				this->addTween(_tween);
			}
		}
		Sprite::setAnimFrame(_resAnim->getFrame(0, _row));
	}

	void TweenButton::doUpdate(const UpdateState &us) {
		if (!_wasBaseScaleSet) {
			setBasicScale(getScale().x);
		}
	}

	void TweenButton::setBaseScale(float scale) {
		setScale(scale);
		setBasicScale(scale);
	}

	void TweenButton::setScale(float scale) {
		Actor::setScale(Vector2(scale, scale));
		setBasicScale(scale);
	}

	void TweenButton::setBasicScale(float scale) {
		_baseScale = scale;
		_wasBaseScaleSet = true;
	}
}