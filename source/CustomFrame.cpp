#include "CustomFrame.h"
#include "RenderState.h"
#include "MaskedSprite.h"
#include "FlurryAnalytics.h"
#include "SoundInstance.h"
#include "SharedResources.h"

spMoneyCounterElement CustomFrame::_moneyCounter = 0;

CustomFrame::CustomFrame() {
	_content = new Actor;
	_content->attachTo(_holder);
	_view = new Actor;
	_view->attachTo(_holder);
	_whichBackground = 0;

	addEventListener(Frame::EVENT_PRESHOWING, CLOSURE(this, &CustomFrame::_preShowing));
	addEventListener(Frame::EVENT_POSTHIDING, CLOSURE(this, &CustomFrame::_postHiding));
	this->selectTransitions();
}

spTweenButton CustomFrame::addButton(const string &name, const string &text, Vector2 position) {
	spTweenButton button = createButton(name, text);
	button->setPosition(position);
	button->attachTo(_view);
	handleClick(button);
	button->addEventListener(TouchEvent::CLICK, CLOSURE(this, &CustomFrame::playClickSound));
	return button;
}

spShaderTweenButton CustomFrame::addShaderButton(const string &name, const string &text, Vector2 position) {
	spShaderTweenButton button = createShaderButton(name, text);
	button->setPosition(position);
	button->attachTo(_view);
	handleClick(button);
	button->addEventListener(TouchEvent::CLICK, CLOSURE(this, &CustomFrame::playClickSound));
	return button;
}

void CustomFrame::playClickSound(Event *event) {
	spSoundInstance instance = sPlayer.play("button_click");
}

void CustomFrame::selectTransitions() {
	int c = 55;
	// Abstract function
}

void CustomFrame::_preShowing(Event *) {
	selectTransitions();

	_resources.load();
}

void CustomFrame::_postHiding(Event *) {
	_resources.unload();
}

void CustomFrame::init(const string &xml) {
	init(xml, false);
}

void CustomFrame::init(const string &xml, bool shouldAddBackground) {
	_resources.loadXML("xmls/" + xml, 0, false);
	
	_view->setSize(getRoot()->getSize());
	_content->setSize(getRoot()->getSize());
	if (shouldAddBackground) {
		spSprite bg = new Sprite;
		bg->setResAnim(_resources.getResAnim("back"));
		bg->setName("_background");
		bg->setScale(_content->getWidth() / bg->getWidth(), _content->getHeight() / bg->getHeight());
		bg->attachTo(_content);
		bg->setPriority(10);
		_whichBackground++;
	}

	if (true) {
		//animateBackground();
	}
}


void CustomFrame::animateBackground() {
	spActor background = _content->getChild("_background");
	spTween tween = new Tween();
	tween = createTween(Actor::TweenAlpha(0), 500);
	tween->setDoneCallback(CLOSURE(this, &CustomFrame::onBackgroundTweenCompleted));
	background->addTween(tween);

	spSprite newBg = new Sprite;
	newBg->setResAnim(_resources.getResAnim(getBackground()));
	newBg->setScale(_content->getWidth() / newBg->getWidth(), _content->getHeight() / newBg->getHeight());
	newBg->setName("_new_background");
	newBg->attachTo(_content);
	newBg->setPriority(0);
	_whichBackground++;
	if (_whichBackground == 2) {
		_whichBackground = 0;
	}
}

void CustomFrame::onBackgroundTweenCompleted(Event *event) {
	_content->removeChild(_content->getChild(event->currentTarget->getName()));
	_content->getChild("_new_background")->setName("_background");
	_content->getChild("_background")->setPriority(10);
}

string CustomFrame::getBackground() {
	return "back";
}