#include "AttachHatToAnimalFrame.h"
#include "SharedResources.h"
#include "LanguageManager.h"
#include "HatManager.h"
#include "AnimalsManager.h"

AttachHatToAnimalFrame::AttachHatToAnimalFrame(const std::string& animal) {
	init("LandingPageFrame.xml", false);
	selectTransitions();
	_animal = animal;
}

AttachHatToAnimalFrame::~AttachHatToAnimalFrame() {

}

void AttachHatToAnimalFrame::selectTransitions() {
	spTransition transition = new TransitionScale;
	setTransitionIn(transition);
	setTransitionOut(transition);
}

void AttachHatToAnimalFrame::_postHiding(Event *) {
	//FlurryAnalytics::instance.onLevelLeaveEvent(_whichLevel.c_str());
	_view->removeChildren();
	_resources.unload();
}

void AttachHatToAnimalFrame::_preShowing(Event *) {
	//FlurryAnalytics::instance.onLevelEnterEvent(_whichLevel.c_str());
	selectTransitions();
	_resources.load();
	setData();
}

Action AttachHatToAnimalFrame::loop() {
	while (1) {
		Action action = waitAction();
		if (action.id == "back" || action.id == "_btn_back_" || action.id == "close") {
			break;
		}
	}

	return _lastAction;
}

void AttachHatToAnimalFrame::setData() {
	createPopupBackground();
	createTitleTextfield();
	createHats();
	addButton("back", "X", Vector2(_view->getWidth() / 2.0f, _view->getHeight() / 2.0f + _view->getHeight() / 4.0f));
}

void AttachHatToAnimalFrame::createPopupBackground() {
	spBox9Sprite cardBackground = new Box9Sprite;
	cardBackground->setHorizontalMode(Box9Sprite::STRETCHING);
	cardBackground->setVerticalMode(Box9Sprite::STRETCHING);
	cardBackground->setAnchor(0.5f, 0.5f);
	cardBackground->setResAnim(gameResources.getResAnim("greyBox9"));
	cardBackground->setSize(_view->getSize() / 2.0f);
	cardBackground->setPosition(_view->getWidth() / 2.0f, _view->getHeight() / 2.0f);
	cardBackground->setGuides(9, 20, 9, 15);
	cardBackground->attachTo(_view);
	cardBackground->setPriority(-1);
}

void AttachHatToAnimalFrame::createTitleTextfield() {
	TextStyle style;
	style.font = gameResources.getResFont("nobile_bold")->getFont();
	style.vAlign = TextStyle::VALIGN_MIDDLE;
	style.hAlign = TextStyle::HALIGN_CENTER;
	style.multiline = true;
	style.color = Color(35, 145, 245);

	spTextField textField = createTextFieldInBoundries(LanguageManager::instance.getText(100), Vector2(_view->getWidth() / 2.0f, _view->getHeight() * 0.2f), style);
	textField->setAnchorX(0.5f);
	textField->setPosition(_view->getWidth() / 2.0f, _view->getHeight() / 2.0f - _view->getHeight() / 4.0f);
	_view->addChild(textField);
}

void AttachHatToAnimalFrame::createHats() {
	const std::map<std::string, int > &hats = HatManager::instance.getFreeHats();
	if (hats.size() == 0) {
		return;
	}
	spSlidingActor slidingActor = new SlidingActor();
	slidingActor->setSize(_view->getWidth() / 2.0f, _view->getHeight() / 2.0f);

	spColorRectSprite content = new ColorRectSprite();
	content->setSize(slidingActor->getWidth(), slidingActor->getHeight());
	content->setColor(Color(0, 0, 0, 0));
	float positionY = slidingActor->getHeight() / 2.0f + OFFSET_ATTACH_HAT;
	float itemHeight = 0.0f;
	 
	int count = 0;
	bool shouldCreate = true;

	for (map<string, int >::const_iterator innerIterator = hats.begin(); innerIterator != hats.end(); ++innerIterator) {
		if (count == 0 && shouldCreate) {
			_container = new StackContainer(Vector2(content->getWidth(), content->getHeight() / 3.0f), 1);
			content->addChild(_container);
			_container->setY(positionY);
			positionY += _container->getHeight() + OFFSET_ATTACH_HAT;
			shouldCreate = false;
		}

		if (innerIterator->second > 0) {
			_container->addChild(createHatButton(innerIterator->first));
			count += 1;
			if (count == HATS_IN_ROW) {
				count = 0;
				shouldCreate = true;
			}
		}
	}
	
	content->setSize(slidingActor->getWidth(), positionY - _container->getHeight() / 2.0f);
	slidingActor->setContent(content);
	slidingActor->setPosition(_view->getSize() / 2 - slidingActor->getSize() / 2);
	slidingActor->attachTo(_view);
}

spTweenButton AttachHatToAnimalFrame::createHatButton(const std::string& name) {
	spTweenButton button = new TweenButton();
	button->setName(name);
	button->setResAnim(gameResources.getResAnim(name));
	button->setAnchor(Vector2(0.5, 0.5));
	button->setTouchChildrenEnabled(false);
	button->setBaseScale(_view->getHeight() / 6.0f / button->getHeight());
	button->addEventListener(TouchEvent::CLICK, CLOSURE(this, &AttachHatToAnimalFrame::onHatClicked));
	
	return button;
}

void AttachHatToAnimalFrame::onHatClicked(Event *event) {
	const std::string &name = event->target->getName();

	HatManager::instance.addWearableToAnimal(_animal, name);
	generateAction("close");
}