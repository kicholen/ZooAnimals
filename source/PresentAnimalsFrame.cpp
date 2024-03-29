#include "PresentAnimalsFrame.h"
#include "FlurryAnalytics.h"
#include "AnimalCardElement.h"
#include "SwipeActor.h"
#include "AnimalsManager.h"
#include "SharedResources.h"

PresentAnimalsFrame::PresentAnimalsFrame(const std::string& region) {
	init("RealAnimalsThumb.xml", false);

	_region = region;
	selectTransitions();
}

void PresentAnimalsFrame::selectTransitions() {
	spTransition transition = new TransitionMove;
	setTransitionIn(transition);
	setTransitionOut(transition);
}

void PresentAnimalsFrame::_postHiding(Event *) {
	//FlurryAnalytics::instance.onLevelLeaveEvent(_whichLevel.c_str());
	_view->removeChildren();
	_resources.unload();
}

void PresentAnimalsFrame::_preShowing(Event *) {
	//FlurryAnalytics::instance.onLevelEnterEvent(_whichLevel.c_str());
	selectTransitions();
	_resources.load();
	setData();
}

Action PresentAnimalsFrame::loop() {
	while (1) {
		Action action = waitAction();
		if (action.id == "_btn_back_") {
			break;
		}
		else if (action.id == "back") {
			break;
		}
		else if (action.id == "close") {
			break;
		}
	}

	return _lastAction;
}

void PresentAnimalsFrame::setData() {
	_cardNavigator = new CardNavigator(1, 30.0f);
	_cardNavigator->setPriority(5);
	_cardNavigator->setPosition(_view->getSize() / 2.0f);
	_cardNavigator->setTouchEnabled(false);
	_view->addChild(_cardNavigator);

	animalMap animaMap = AnimalsManager::instance.getPossesedAnimalsByRegion(_region);
	for (animalMap::iterator innerIterator = animaMap.begin(); innerIterator != animaMap.end(); ++innerIterator) {
		spAnimalCardElement animalCard = new AnimalCardElement(Vector2(_view->getHeight() * 0.6f, _view->getHeight() * 0.8f), innerIterator->second, _resources.getResAnim(innerIterator->second->animalName()));
		animalCard->setPosition(_view->getSize() / 2.0f);
		animalCard->setCull(true);
		_cardNavigator->addCard(animalCard);
	}
	
	spSwipeActor swipeActor = new SwipeActor(1.0f, 0.30f);
	swipeActor->setPriority(4);
	swipeActor->setSize(_view->getSize());
	swipeActor->addEventListener(SwipeActor::SwipeEvent::LEFT, CLOSURE(this, &PresentAnimalsFrame::onSwipeLeft));
	swipeActor->addEventListener(SwipeActor::SwipeEvent::RIGHT, CLOSURE(this, &PresentAnimalsFrame::onSwipeRight));
	_view->addChild(swipeActor);
}

void PresentAnimalsFrame::onSwipeLeft(Event *ev) {
	_cardNavigator->animateToNextCard();
}

void PresentAnimalsFrame::onSwipeRight(Event *ev) {
	_cardNavigator->animateToPreviousCard();
}
