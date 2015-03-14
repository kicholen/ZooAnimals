#include "MaskFrame.h"
#include "EditableSprite.h"
#include "ChooseSpriteFrame.h"
#include "s3eOSExec.h"
#include "FlashUtils.h"
#include "ParticleEmitter.h"
#include "FileSaver.h"

using namespace FlashUtils;

MaskFrame::MaskFrame() {
	init("MaskFrame.xml", true);

	slideFrame->addEventListener(SlideFrame::GoBackToPreviousFrameEvent::GO_BACK, CLOSURE(this, &MaskFrame::onGoBack));

	selectTransitions();
}

void MaskFrame::selectTransitions() {
	spTransition transition = new TransitionQuads;
	setTransitionIn(transition);
	setTransitionOut(transition);
}

Action MaskFrame::loop() {
	while (1) {
		Action action = waitAction();
		if (action.id == "slideFrame") {
			transitionShowFrameAsDialog(slideFrame);
		}
		else if (action.id == "back") {
			break;
		}
		else if (action.id == "close") {
			break;
		}
		else if (action.id == "CLEAR") {
			_view->removeChildren();
			setData();
		}
		else if (action.id == "ADD") {
			spChooseSpriteFrame chooseSpriteFrame = new ChooseSpriteFrame;
			chooseSpriteFrame->addEventListener(ChooseSpriteFrame::SpawnSpriteEvent::SPAWN_SPRITE, CLOSURE(this, &MaskFrame::onSpawnSprite));
			transitionShowFrameAsDialog(chooseSpriteFrame);
		}
		else if (action.id == "ADD_POINT") {
			addDraggableSprite("quad_40", Vector2(0.5f, 0.5f), 0.5f, 0.5f,_view->getWidth() / 2, _view->getHeight() / 2, 0, false);
		}
		else if (action.id == "SAVE") {
			spActor child = _spritesHolder->getFirstChild();
			string firstMessage[] = {"", "", "", "", "", "", "", ""};
			int i = 0;
			string bodyString = "<w><c>";
			while(child) {
				bodyString += "<g>";	
				bodyString += "<sX>" + FlashUtils::CMath::doubleToString(child->getDerivedWidth() / getRoot()->getWidth());
				bodyString += "</sX>";
				bodyString += "<sY>" + FlashUtils::CMath::doubleToString(child->getDerivedHeight() / getRoot()->getHeight());
				bodyString += "</sY>";
				bodyString += "<x>" + FlashUtils::CMath::doubleToString(child->getX() / getRoot()->getWidth());
				bodyString += "</x>";
				bodyString += "<y>" + FlashUtils::CMath::doubleToString(child->getY() / getRoot()->getHeight());
				bodyString += "</y>";
				bodyString += "<z>" + FlashUtils::CMath::doubleToString(child->getPriority());
				bodyString += "</z>";
				bodyString += "<s>" + child->getName();
				bodyString += "</s></g>";
				child = child->getNextSibling();
			}
			bodyString += "</c></w>";
			printf(bodyString.c_str());
			FileSaver *_saver = new FileSaver();
			_saver->init("1.0");
			_saver->setPath(_currentTextfield->getText() + ".xml");
			_saver->getFirstNodeByName("bodies").append_attribute(bodyString.c_str());
			_saver->save();
		}
	}

	return _lastAction;
}

void MaskFrame::onGoBack(Event *event) {
	const string &name ="close";
	generateAction(name);
}

void MaskFrame::onSpawnSprite(Event *event) {
	addDraggableSprite(event->userDataObject->getName(), Vector2(0.5f, 0.5f), 0.5f, 0.5f,_view->getWidth() / 2, _view->getHeight() / 2, 0, false);
}

void MaskFrame::onShowSliderFrame(Event *event) {
	const string &name ="slideFrame";
	generateAction(name);
}

void MaskFrame::_postHiding(Event *) {
	slideFrame->removeEventListener(SlideFrame::GoBackToPreviousFrameEvent::GO_BACK, CLOSURE(this, &MaskFrame::onGoBack));
	_view->removeChildren();
	_resources.unload();
}

void MaskFrame::_preShowing(Event *) {
	selectTransitions();
	setData();
	_resources.load();
}

string MaskFrame::getBackground() {
	string backgroundsList[] = {"back", "background2", "background3", "background4" };
	return backgroundsList[_whichBackground];
}

void MaskFrame::_clicked(Event *event) {
}

void MaskFrame::setData() {
	_count = 0;
	_spritesHolder = new Actor;
	_spritesHolder->attachTo(_view);
	_spritesHolder->setSize(getRoot()->getSize());

	spButton sliderButton = new Button();
	sliderButton->setResAnim(_resources.getResAnim("back_button"));
	sliderButton->setScale(_content->getWidth() * 8 / 100 / sliderButton->getWidth());
	sliderButton->setPosition(Vector2(_content->getWidth() * 9 / 10 - 12, _content->getHeight() * 95 / 100 - sliderButton->getDerivedHeight()));

	sliderButton->attachTo(_view);
	sliderButton->setName("slideFrame");
	sliderButton->addEventListener(TouchEvent::TOUCH_DOWN, CLOSURE(this, &MaskFrame::onShowSliderFrame));
	
	spTween tween = new Tween();
	tween = createTween(TweenDummy(), 3000, -1);
	tween->setDoneCallback(CLOSURE(this, &MaskFrame::switchBackground));
	sliderButton->addTween(tween);

	spTweenButton button = addButton("CLEAR", "CLEAR", Vector2(_view->getWidth(), 50));
	button->setScale(button->getScale().x / 2);
	//button->setScale(button->getScale() / 2);
	button->setX(_view->getWidth() - button->getDerivedWidth() / 2);
	button = addButton("ADD", "ADD", Vector2(_view->getWidth() - button->getDerivedWidth() / 2, 50 + (button->getDerivedHeight() + 5)* 2));
	button->setScale(button->getScale().x / 2);

	button = addButton("SAVE", "SAVE", Vector2(_view->getWidth() - button->getDerivedWidth() / 2,  _content->getHeight() * 80 / 100 - button->getDerivedHeight()));
	button->setScale(button->getScale().x / 2);

	button = addButton("ADD_POINT", "ADD_POINT", Vector2(_view->getWidth() - button->getDerivedWidth() / 2, 50 + (button->getDerivedHeight() + 5)* 3));
	button->setScale(button->getScale().x / 2);

	_currentTextfield = createTextfield("FILE_PATH", false, false);
	_currentTextfield->setFontSize2Scale(40 * (int)_view->getWidth() / 640);
	_currentTextfield->setSize(button->getDerivedWidth(), button->getDerivedHeight());
	_currentTextfield->setPosition(button->getX(), button->getY() - button->getDerivedHeight() * 3 / 2);
	_currentTextfield->setColor(Color(35, 145, 245));
	_currentTextfield->attachTo(_view);
	_currentTextfield->addEventListener(TouchEvent::CLICK, CLOSURE(this, &MaskFrame::onClick));
	_inputTextfield = new InputText;
	_inputTextfield->addEventListener(Event::COMPLETE, CLOSURE(this, &MaskFrame::onComplete));
}

void MaskFrame::switchBackground(Event *event) {
	animateBackground();
}

void MaskFrame::addDraggableSprite(string spriteName, Vector2 anchorPoint, float scaleX, float scaleY, float positionX, float positionY, int priorityZ, bool fromEditResources = false) {
	if (fromEditResources) {
		spSprite sprite = new EditableSprite;
		sprite->setResAnim(editResources.getResAnim(spriteName));
		sprite->setAnchor(anchorPoint);
		sprite->setScale(scaleX * getRoot()->getWidth() / sprite->getWidth(), scaleY * getRoot()->getHeight() / sprite->getHeight());
		sprite->setPosition(positionX * getRoot()->getWidth(), positionY * getRoot()->getHeight());
		sprite->setName(spriteName);
		sprite->setPriority(priorityZ);
		_spritesHolder->addChild(sprite);
	}
	else {
		spSprite sprite = new EditableSprite;
		sprite->setResAnim(editResources.getResAnim(spriteName));
		sprite->setAnchor(anchorPoint);
		sprite->setScale(scaleX, scaleY);
		sprite->setPosition(positionX, positionY);
		sprite->setName(spriteName);
		sprite->setPriority(priorityZ);
		_spritesHolder->addChild(sprite);
	}
}

void MaskFrame::onClick(Event *ev) {
	if (_currentTextfield) {
		_currentTextfield->setColor(Color::White);
	}

	_currentTextfield = safeSpCast<TextActor>(ev->currentTarget);
	_inputTextfield->start(_currentTextfield);
	_currentTextfield->setColor(Color::Red);
}

void MaskFrame::onComplete(Event *ev) {
//	if (_currentTextfield) {
//		_currentTextfield->setColor(Color::White);
//	}
//	_currentTextfield = 0;
//	InputText::stopAnyInput();
}