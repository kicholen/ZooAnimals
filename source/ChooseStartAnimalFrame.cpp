#include "ChooseStartAnimalFrame.h"
#include "ItemPickerElement.h"
#include "LanguageManager.h"
#include "SharedResources.h"
#include "StartGameConfig.h"
#include "ZooGateFrame.h"
#include "AnimalsManager.h"

ChooseStartAnimalFrame::ChooseStartAnimalFrame() {
	init("LandingPageFrame.xml", false);
	selectTransitions();
}

void ChooseStartAnimalFrame::selectTransitions() {
	spTransition transition = new TransitionScale;
	setTransitionIn(transition);
	spTransition transitionOut = new TransitionInstant();
	setTransitionOut(transitionOut);
}

void ChooseStartAnimalFrame::_postHiding(Event *) {
	//FlurryAnalytics::instance.onLevelLeaveEvent(_whichLevel.c_str());
	_view->removeChildren();
	_resources.unload();
	_inPool.clear();
}

void ChooseStartAnimalFrame::_preShowing(Event *) {
	//FlurryAnalytics::instance.onLevelEnterEvent(_whichLevel.c_str());
	selectTransitions();
	setData();
	_resources.load();
}

Action ChooseStartAnimalFrame::loop(){
	while (1) {
		Action action = waitAction();
		if (action.id == "play") {
			AnimalsManager::instance.increaseAnimalCount(_choosenAnimal, 1);
			break;
		}
		else if (action.id == "back" || action.id == "_btn_back_" || action.id == "close") {
			break;
		}
	}

	return _lastAction;
}

void ChooseStartAnimalFrame::setData() {
	spBox9Sprite popupBackground = new Box9Sprite;
	popupBackground->setAnchor(0.5f, 0.5f);
	popupBackground->setResAnim(gameResources.getResAnim("popup_box9_ad"));
	popupBackground->setSize(_view->getWidth() * 0.8f, _view->getHeight() * 0.8f);
	popupBackground->setPosition(_view->getWidth() / 2.0f, _view->getHeight() / 2.0f);
	popupBackground->setTouchEnabled(false);
	popupBackground->setVerticalMode(Box9Sprite::STRETCHING);
	popupBackground->setHorizontalMode(Box9Sprite::STRETCHING);
	popupBackground->setGuides(50, 200, 50, 200);
	popupBackground->attachTo(_view);

	spTextField chooseAnimal = createTextfield(LanguageManager::instance.getText(50), false, true);
	chooseAnimal->setFontSize2Scale(25 * (int)_view->getWidth() / 640);
	chooseAnimal->setSize(popupBackground->getWidth() * 0.9f, popupBackground->getHeight() * 0.3f);
	chooseAnimal->setPosition(_view->getWidth() / 2, _view->getHeight() * 0.2f);
	_view->addChild(chooseAnimal);

	_okButton = addShaderButton("play", LanguageManager::instance.getText(49), Vector2(_view->getWidth() / 2.0f, _view->getHeight() * 0.8f));
	_okButton->removeEventListener(TouchEvent::CLICK, CLOSURE(this, &Frame::generateActionEvent));
	_okButton->setInvertShaderProgram();
	_okButton->setVal(Vector4(0.5f, 0.5f, 0.5f, 0.5f));

	VectorArray<spActor> dupaArray;
	dupaArray._vector.resize(0);
	for (int i = 0; i < 3; i++) {
		dupaArray.push(createRandomAnimalSprite());
	}
	
	spItemPickerElement picker = new ItemPickerElement(Vector2(popupBackground->getWidth() * 0.8f, popupBackground->getHeight() * 0.5f));
	picker->setAnchor(0.5f, 0.5f);
	picker->setPosition(_view->getWidth() / 2.0f, _view->getHeight() / 2.0f);
	picker->setData(dupaArray);
	picker->addEventListener(ItemPickerElement::ItemPickerElementEvent::ITEM_CHOOSEN, CLOSURE(this, &ChooseStartAnimalFrame::onAnimalPicked));
	_view->addChild(picker);
}

spSprite ChooseStartAnimalFrame::createRandomAnimalSprite() {
	int drawnInt = 0;
	int size;
	string name;
	while (isInPool(drawnInt)) {
		drawnInt = CMath::random(0, 5);
	}
	
	switch (drawnInt) {
		case 0:
			size = sizeof(START_FARM) / sizeof(START_FARM[0]);
			name = START_FARM[CMath::random(0, size)];
			break;
		case 1:
			size = sizeof(START_WINTER) / sizeof(START_WINTER[0]);
			name = START_WINTER[CMath::random(0, size)];
			break;
		case 2:
			size = sizeof(START_STEPPE) / sizeof(START_STEPPE[0]);
			name = START_STEPPE[CMath::random(0, size)];
			break;
		case 3:
			size = sizeof(START_ASIA) / sizeof(START_ASIA[0]);
			name = START_ASIA[CMath::random(0, size)];
			break;
		case 4:
			size = sizeof(START_AUSTRALIA) / sizeof(START_AUSTRALIA[0]);
			name = START_AUSTRALIA[CMath::random(0, size)];
			break;
	}

	_inPool.push(drawnInt);

	spSprite animalSprite = new Sprite();
	animalSprite->setAnchor(0.5f, 0.5f);
	animalSprite->setName(name);
	animalSprite->setResAnim(animalsResources.getResAnim(name));

	return animalSprite;
}

bool ChooseStartAnimalFrame::isInPool(int id) {
	for (int i = 0; i < _inPool.length(); i++) {
		if (id == _inPool[i]) {
			return true;
		}
	}

	return false;
}

void ChooseStartAnimalFrame::onAnimalPicked(Event *ev) {
	ItemPickerElement::ItemPickerElementEvent *itemPickedEvent = safeCast<ItemPickerElement::ItemPickerElementEvent*>(ev);

	_choosenAnimal = AnimalsManager::instance.getAnimalModel(itemPickedEvent->itemName);
	unblockOkButton();
}

void ChooseStartAnimalFrame::unblockOkButton() {
	_okButton->setVal(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	_okButton->addEventListener(TouchEvent::CLICK, CLOSURE(this, &Frame::generateActionEvent));
}
