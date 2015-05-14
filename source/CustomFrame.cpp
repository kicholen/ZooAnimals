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
	}
}