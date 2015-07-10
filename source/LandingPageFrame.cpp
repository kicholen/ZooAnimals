#include "LandingPageFrame.h"
//#include "SoundInstance.h"
#include "s3eOSExec.h"
#include "GooglePlayInAppPurchaseManager.h"
#include "SwipeTrailElement.h"
#include "WorldMapFrame.h"
#include "ZooFrame.h"
#include "ZooGateFrame.h"
#include "DatabaseManager.h"
#include "AnimalsManager.h"
#include "MoneyCounterElement.h"
#include "ShopFrame.h"
#include "ChooseStartAnimalFrame.h"
#include "ZooSettings.h"
#include "JumpOverFrame.h"
#include "FindShadowFrame.h"
#include "ChooseGameDifficultyFrame.h"

LandingPageFrame::LandingPageFrame() {
	init("LandingPageFrame.xml", true);
	playLoopedMusic();
	selectTransitions();
}

void LandingPageFrame::selectTransitions() {
	spTransition TransitionSimple;
	setTransitionIn(TransitionSimple);
	setTransitionOut(TransitionSimple);
}

void LandingPageFrame::_postHiding(Event *) {
	_view->removeChildren();
	_resources.unload();
}

void LandingPageFrame::_preShowing(Event *) {
	//GooglePlayInAppPurchaseManager::instance.restorePurchases();
	selectTransitions();
	setData();
	_resources.load();
}

Action LandingPageFrame::loop(){
	while (1) {
		Action action = waitAction();
		if (action.id == "play") {
			if (ZooSettings::instance.shouldShowChooseAnimalPopup()) {
				spChooseStartAnimalFrame chooseAnimal = new ChooseStartAnimalFrame();
				Action innerAction = transitionShowFrameAsDialog(chooseAnimal);

				if (innerAction.id == "play") {
					ZooSettings::instance.finishStartAnimalChoice();
					spZooGateFrame zooGateFrame = new ZooGateFrame();
					transitionShowFrame(zooGateFrame);
				}
			}
			else {
				spZooGateFrame zooGateFrame = new ZooGateFrame();
				transitionShowFrame(zooGateFrame);
			}
		}
		else if (action.id == "map") {
			spWorldMapFrame worldFrame = new WorldMapFrame();
			transitionShowFrame(worldFrame);
		}
		else if (action.id == "facebook") {
			spShopFrame shopFrame = new ShopFrame();
			transitionShowFrame(shopFrame);
			//s3eOSExecExecute("https://www.facebook.com/JellyBeanApps", false);
		}
		else if (action.id == "rate") {
			s3eOSExecExecute("https://play.google.com/store/apps/details?id=com.Zelek.ToddlersAnimals", false);
		}
		/*else if (action.id == "test") {
			transitionShowFrame(maskFrame);
		}*/
		//else if (action.id == "edit") {
			
			//spMaskFrame maskFrame = new MaskFrame();
			//transitionShowFrame(maskFrame);
		//}
		else if (action.id == "test" || action.id == "edit") {
			spChooseGameDifficultyFrame chooserFrame = new ChooseGameDifficultyFrame();

			Action innerAction = transitionShowFrameAsDialog(chooserFrame, 0, 0);

			if (innerAction.id == "back" || innerAction.id == "_btn_back_") {
				// do nothing
				int asd = 10;
			}
			else {
				/*if (action.id == "memory") {
					spMemoryFrame memoryFrame = new MemoryFrame(innerAction.id);
					transitionShowFrame(memoryFrame);
				}
				else if (action.id == "dots") {
					spConnectDotsFrame connectFrame = new ConnectDotsFrame(innerAction.id);
					transitionShowFrame(connectFrame);
				}
				else */if (action.id == "test") {
					spFindShadowFrame findShadow = new FindShadowFrame(innerAction.id);
					transitionShowFrame(findShadow);
				}
				else if (action.id == "edit") {
					spJumpOverFrame jumpFrame = new JumpOverFrame(innerAction.id);
					transitionShowFrame(jumpFrame);
				}/*
				else if (action.id == "match") {
					spMatchTwoFrame matchTwo = new MatchTwoFrame(innerAction.id);
					transitionShowFrame(matchTwo);
				}
				else if (action.id == "pop") {
					spPopObjectsFrame pop = new PopObjectsFrame(innerAction.id);
					transitionShowFrame(pop);
				}
				else if (action.id == "discover") {
					spDiscoverImageFrame disc = new DiscoverImageFrame(innerAction.id);
					transitionShowFrame(disc);
				}*/
			}
		}
		else if (action.id == "back" || action.id == "_btn_back_" || action.id == "close") {
			DatabaseManager::instance.save(0);
			break;
		}
	}

	return _lastAction;
}

void LandingPageFrame::setData() {
	spTweenButton button = addButton("play", "PLAY", Vector2(_view->getWidth() / 2, _view->getHeight() / 2));
	addButton("facebook", "", Vector2(_view->getWidth() / 2, _view->getHeight() / 2 + button->getDerivedHeight() * 2 + 20));
	addButton("map", "MAP", Vector2(_view->getWidth() / 2, _view->getHeight() / 2 + button->getDerivedHeight() + 10));
	
	spTextActor text = createTextfield("Loveable animals cannot wait to play with you! :)", true, true);
	text->setFontSize2Scale(40 * (int)_view->getWidth() / 640);
	text->setSize(_view->getWidth() * 0.65f, _view->getHeight() * 0.4f);
	text->setPosition(_view->getWidth() / 2, _view->getHeight() * 0.2f);
	text->setColor(Color(35, 145, 245));
	text->attachTo(_view);
	addButton("edit", "edit", Vector2(_view->getWidth() / 2, _view->getHeight() / 2 + button->getDerivedHeight() * 3 + 30));

	addButton("test", "test", Vector2(_view->getWidth() / 2, _view->getHeight() / 2 + button->getDerivedHeight() * 4 + 30));

	spSwipeTrailElement swipe = new SwipeTrailElement(20);
	swipe->setSize(_view->getSize());
	swipe->setPriority(-1000);
	swipe->attachTo(_view);

	createGlobalElementsIfDoesntExist();
}

void LandingPageFrame::playLoopedMusic() {
	float musicBaseValue = 0.8f;

	/*int r = rand() % 3;
	const char *str = "track_01";
	switch(r) {
		case 1:
			str = "track_02";
			break;
	}*/

	//spSoundInstance instance = sPlayer.play("track_03", true, 200, 200);
	//instance->setVolume(musicBaseValue);
	//instance->setDoneCallback(CLOSURE(this, &LandingPageFrame::onMusicDone));
}

void LandingPageFrame::onMusicDone(Event *event) {
	playLoopedMusic();
}

void LandingPageFrame::createGlobalElementsIfDoesntExist() {
	if (!_moneyCounter) {
		_moneyCounter = new MoneyCounterElement();
		_moneyCounter->show();
		_moneyCounter->setName("money_counter");
		_moneyCounter->setPriority(1000);
		getRoot()->addChild(_moneyCounter);
	}
}