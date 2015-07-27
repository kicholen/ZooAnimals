#include "Transition.h"
#include "RenderState.h"
#include "MaskedSprite.h"
#include "core/oxygine.h"

namespace oxygine
{
	Transition::Transition()
	{

	}

	Transition::~Transition()
	{

	}

	void Transition::start(spFrame prev, spFrame next, bool dialog, bool back)
	{
		trans t;
		t.prev = prev;
		t.next = next;
		t.dialog = dialog;
		t.back = back;
		_start(t);
	}

	void Transition::end()
	{
		_end();
	}

	void TransitionSimple::_start(trans &t)
	{
		attach(t);
		spFrame frame = t.back ? t.prev : t.next;
		if (_singleDirection)
			frame = t.next;

		setup(frame, t.back);
		middle(t);
		if (Frame::currentTransition && t.back)
			Frame::currentTransition->end();
		blockingPhase(frame, t.back);
		detach(t);
		reset(frame, t.back);
	}

	void TransitionSimple::middle(trans &t)
	{
		if (t.prev && (t.back || !t.dialog))
		{
			t.prev->preHiding();
		}
	}

	void TransitionSimple::attach(trans &t)
	{		
		if (t.next && (!t.back || !t.dialog))
		{
			t.next->preShowing();
		}

		
		
		if (t.back)
		{
			if (!t.dialog)
				getRoot()->insertChildBefore(t.next->getHolder(), t.prev->getHolder());
		}
		else
			t.next->getHolder()->attachTo(getRoot());	
	}

	void TransitionSimple::detach(trans &t)
	{
		if (t.prev && (t.back || !t.dialog))
		{
			t.prev->postHiding();			
			t.prev->getHolder()->detach();
		}

		if (t.next && (!t.back || !t.dialog))
		{
			t.next->postShowing();
		}		
	}

	TransitionMove::TransitionMove()
	{
		_src = Vector2(0.0f, -(float)getRoot()->getHeight());
		_dest = Vector2(0.0f, 0.0f);
	}

	void TransitionMove::setSrc(const Vector2 &pos)
	{
		_src = pos;
	}

	void TransitionMove::setDest(const Vector2 &pos)
	{
		_dest = pos;
	}

	void TransitionMove::setup(spFrame frame, bool back)
	{
		frame->getHolder()->setPosition(back ? _dest : _src);
	}

	void TransitionMove::blockingPhase(spFrame frame, bool back)
	{
		spActor holder = frame->getHolder();

		if (back)
		{

		}
		else
		{
			_fade = new ColorRectSprite;
			_fade->setSize(getRoot()->getSize());
			_fade->setColor(Color(0, 0, 0, 128));
			_fade->setAlpha(0);
			_fade->addTween(Actor::TweenAlpha(255), _duration);

			holder->getParent()->insertChildBefore(_fade, holder);
		}		
		spTween tween = frame->getHolder()->addTween(createTween(Actor::TweenPosition(back ? _src : _dest), _duration, 1, false, 0, Tween::ease_inOutBack));
		blocking::waitTween(tween);	
	}

	void TransitionMove::reset(spFrame frame, bool back)
	{
		frame->getHolder()->setPosition(Vector2(0,0));
	}

	void TransitionMove::_end()
	{
		//_fade->detach();
		spTween t = _fade->addTween(Actor::TweenAlpha(0), _duration);
		t->setDetachActor(true);
	}

	void TransitionScale::setup(spFrame frame, bool back)
	{
		spActor holder = frame->getHolder();
		holder->setAnchor(Vector2(0.5f, 0.5f));
		holder->setPosition(getRoot()->getDestRect().getCenter());
		holder->setScale(back ? 1.0f : 0.0f);
	}

	void TransitionScale::blockingPhase(spFrame frame, bool back)
	{
		spTween tween = frame->getHolder()->addTween(createTween(Actor::TweenScale(back ? 0.0f : 1.0f), _duration, 1, false, 0));
		blocking::waitTween(tween);	
	}

	void TransitionScale::reset(spFrame frame, bool back)
	{
		spActor holder = frame->getHolder();
		holder->setAnchor(Vector2(0.0f, 0.0f));
		holder->setPosition(Vector2(0, 0));
		holder->setScale(1);
	}

	void TransitionFade::setup(spFrame frame, bool back)
	{
		frame->getHolder()->setAlpha(back ? 255 : 0);
	}

	void TransitionFade::blockingPhase(spFrame frame, bool back)
	{
		spTween tween = frame->getHolder()->addTween(createTween(Actor::TweenAlpha(back ? 0 : 255), _duration, 1, false, 0));
		blocking::waitTween(tween);	
	}

	void TransitionFade::reset(spFrame frame, bool back)
	{
		frame->getHolder()->setAlpha(255);
	}

	TransitionScaleRotate::TransitionScaleRotate()
	{
		_duration = 1000;
	}

	void TransitionScaleRotate::setup(spFrame frame, bool back)
	{
		spActor holder = frame->getHolder();
		holder->setAnchor(Vector2(0.5f, 0.5f));
		holder->setPosition(getRoot()->getDestRect().getCenter());
		holder->setScale(back ? 1.0f : 0.0f);
		holder->setRotation(back ? 0 : MATH_PI * 2 * 4.0f);
	}

	void TransitionScaleRotate::blockingPhase(spFrame frame, bool back)
	{
		spTween tween;
		tween = frame->getHolder()->addTween(createTween(Actor::TweenRotation(back ? MATH_PI * 2 * 4.0f : 0), _duration));
		tween = frame->getHolder()->addTween(createTween(Actor::TweenScale(back ? 0.0f : 1.0f), _duration));
		blocking::waitTween(tween);	
	}

	void TransitionScaleRotate::reset(spFrame frame, bool back)
	{
		spActor holder = frame->getHolder();
		holder->setAnchor(Vector2(0.0f, 0.0f));
		holder->setPosition(Vector2(0, 0));
		holder->setScale(1);
		holder->setRotation(0);
	}

	TransitionQuads::TransitionQuads():_center(0,0)
	{
		_singleDirection = true;
		getRoot()->addEventListener(TouchEvent::CLICK, CLOSURE(this, &TransitionQuads::clicked));
	}
	TransitionQuads::~TransitionQuads()
	{
		getRoot()->removeEventListeners(this);
	}

	void TransitionQuads::clicked(Event *ev)
	{
		TouchEvent *te = (TouchEvent*)ev;
		_center = getRoot()->local2global(te->localPosition);
	}

	void TransitionQuads::setup(spFrame frame, bool back)
	{
	}

	void TransitionQuads::blockingPhase(spFrame frame, bool back_)
	{
		Point ds = core::getDisplaySize();
		spActor holder = new Actor;
		holder->setPosition(getRoot()->global2local(Vector2(0,0)));
		holder->setVisible(false);
		holder->setSize(core::getDisplaySize());
		holder->attachTo(getRoot());
		holder->setPriority(100);

		int numX = ds.x/40;
		int numY = ds.y/40;

		//log::messageln("tq1");
		Vector2 quad(holder->getWidth()/numX, holder->getHeight()/numY);
		spTween slowestTween;

		for (int y = 0; y < numY; ++y)
		{
			for (int x = 0; x < numX; ++x)
			{
				spSprite sp = new ColorRectSprite;
				Vector2 pos(quad.x * x, quad.y * y);
				pos += quad/2;
				sp->setPosition(pos);
				sp->setAnchor(Vector2(0.5f, 0.5f));
				sp->setSize(quad);
				sp->attachTo(holder);
				sp->setScale(0);
				sp->setColor(Color(0xffffffff));
				Vector2 d = pos - _center;
				float time = d.length() / holder->getSize().length();
				/*
				if (back)
					time = 1.0f - time;
					*/

				float mp = 1.0f;
				//mp = 10;
				int tm = int(1 + time * 800 * mp);
				spTween nt = sp->addTween(Actor::TweenScale(1.0f), int(100 * mp), 1, false, tm);
				if (!slowestTween || (int)slowestTween->getDelay() < tm)
					slowestTween = nt;
			}
		}

		//log::messageln("tq2");

		STDRenderer r;
		RenderState rs;
		rs.renderer = &r;


		
		spNativeTexture mask = IVideoDriver::instance->createTexture();
		mask->init(ds.x, ds.y, TF_R5G5B5A1, true);


		rs.renderer->initCoordinateSystem(ds.x, ds.y, true);

		Rect vp(Point(0, 0), ds);

		spSprite maskSprite = new Sprite;
		{
			AnimationFrame fr;
			Diffuse df;
			df.base = mask;
			RectF srcRect(0, 0, (float)ds.x/mask->getWidth(), (float)ds.y/mask->getHeight());
			RectF destRect(Vector2(0,0), ds);
			fr.init(0, df, srcRect, destRect, ds);
			maskSprite->setAnimFrame(fr);
		}

		//log::messageln("tq3");
#if ANDROID
//#define BUG
#endif

#ifdef BUG
		spSprite bg = new Sprite;
#else
		spMaskedSprite bg = new MaskedSprite;
		bg->setMask(maskSprite);
#endif

		bg->attachTo(getRoot());
		bg->setPriority(100);

		
		bg->addChild(frame->getHolder());
		bg->setInputEnabled(false);
		
		timeMS tm = getTimeMS() + 3000;

		while(!slowestTween->isDone())
		{
			//log::messageln("tq4");
			Color b(0,0,0,0);
			r.begin(0);
			{
				holder->setPosition(Vector2(0,0));
				holder->setVisible(true);
				holder->render(rs);
				holder->setPosition(getRoot()->global2local(Vector2(0,0)));
				holder->setVisible(false);
				r.end();
			}

			//log::messageln("tq41");
			blocking::yield();
		}

		//log::messageln("tq5");

		frame->getHolder()->attachTo(getRoot());

		holder->detach();
		bg->detach();
#ifdef BUG

#else
		bg->setMask(0);
#endif
	}

	void TransitionQuads::reset(spFrame frame, bool back)
	{

	}

	void TransitionInstant::setup(spFrame frame, bool back)
	{
		_duration = 50;
	}

	void TransitionInstant::blockingPhase(spFrame frame, bool back)
	{
		spTween tween = frame->getHolder()->addTween(createTween(TweenDummy(), _duration, 1, false, 0));
		blocking::waitTween(tween);
	}

	void TransitionInstant::reset(spFrame frame, bool back)
	{
	}
}