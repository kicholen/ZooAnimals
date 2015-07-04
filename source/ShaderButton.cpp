#include "ShaderButton.h"
#include "RootActor.h"

namespace oxygine
{
	void ShaderButton::copyFrom(const ShaderButton &src, cloneOptions opt)
	{
		ShaderSprite::copyFrom(src, opt);

		_state = src._state;
		_resAnim = src._resAnim;
		_row = src._row;
	}

	ShaderButton::ShaderButton():_state(stateNormal), _resAnim(0), _row(0), _btnPressed(0), _btnOvered(0)
	{
		EventCallback ncb = CLOSURE(this, &ShaderButton::_mouseEvent);
		addEventListener(TouchEvent::TOUCH_DOWN, ncb);
		addEventListener(TouchEvent::OVER, ncb);
		addEventListener(TouchEvent::OUT, ncb);
		addEventListener(TouchEvent::CLICK, ncb);
	}

	ShaderButton::~ShaderButton()
	{
		/*
		removeEventHandlers();
		if (_ph)
		{
			_ph->reset();
		}
		*/
	}

	void ShaderButton::_mouseUpOutside(Event *event)
	{
		//Actor *act = safeCast<Actor*>(event->target.get());
		//if (isDescendant(act))
		_pressed = false;
		setState(stateNormal);
	}

	void ShaderButton::_mouseEvent(Event *event)
	{
		TouchEvent *me = safeCast<TouchEvent*>(event);
		Actor *act = safeCast<Actor*>(event->target.get());

		switch(event->type)
		{
		case TouchEvent::CLICK:
			{
				event->phase = Event::phase_target;
				event->target = this;
			}
			break;
		case TouchEvent::OVER:
			{				
				if (!_btnOvered)
				{
					_btnOvered = me->index;
					if (!_btnPressed)
						setState(stateOvered);
				}				
			}
			break;
		case TouchEvent::OUT:
			{
				if (_btnOvered == me->index)
				{
					if (!_btnPressed)
						setState(stateNormal);
					_btnOvered = 0;
				}				
			}
			break;
		case TouchEvent::TOUCH_DOWN:
			{
				{
					if (!_btnPressed)
					{
						_btnPressed = me->index;
						setState(statePressed);
						getRoot()->addEventListener(TouchEvent::TOUCH_UP, CLOSURE(this, &ShaderButton::_mouseEvent));
					}					
				}				
			}
			break;
		case TouchEvent::TOUCH_UP:
			{
				if (_btnPressed == me->index)
				{				
					setState(stateNormal);
					getRoot()->removeEventListener(TouchEvent::TOUCH_UP, CLOSURE(this, &ShaderButton::_mouseEvent));
					_btnPressed = 0;
				}
			}
			break;
		}		
	}

	void ShaderButton::setResAnim(const ResAnim *r)
	{
		_resAnim = r;
		updateButtonState(_state);
	}

	void ShaderButton::setRow(int row)
	{
		_row = row;
		updateButtonState(_state);
	}

	void ShaderButton::setState(state s)
	{
		if (s == _state)
			return;
		_state = s;
		updateButtonState(s);
	}

	void ShaderButton::updateButtonState(state s)
	{
		if (!_resAnim)
			return;
		
		if (_resAnim->getColumns() > s)
			ShaderSprite::setAnimFrame(_resAnim->getFrame(s, _row));
		else
			ShaderSprite::setAnimFrame(_resAnim->getFrame(0, _row));
		
	}
}