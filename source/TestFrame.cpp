#include "TestFrame.h"

#include "ChooseGameDifficultyFrame.h"
#include "JumpOverFrame.h"
#include "FindShadowFrame.h"
#include "CloseFrameElement.h"
#include "PopObjectsFrame.h"
#include "MemoryFrame.h"
#include "MatchTwoFrame.h"
#include "ConnectDotsFrame.h"
#include "DiscoverImageFrame.h"

#include "LevelRewardsFrame.h"
#include "LevelUpPopup.h"
#include "ShopFrame.h"
#include "AttachHatToAnimalFrame.h"
#include "AchievementFrame.h"

#include "ChooseStartAnimalFrame.h"
#include "WorldMapFrame.h"

TestFrame::TestFrame()
{
	init("LandingPageFrame.xml", true);
	selectTransitions();
}


TestFrame::~TestFrame()
{
}

void TestFrame::selectTransitions() {
	spTransition TransitionSimple;
	setTransitionIn(TransitionSimple);
	setTransitionOut(TransitionSimple);
}

void TestFrame::_postHiding(Event *) {
	_view->removeChildren();
	_resources.unload();
}

void TestFrame::_preShowing(Event *) {
	selectTransitions();
	setData();
	_resources.load();
}

Action TestFrame::loop(){
	while (1) {
		Action action = waitAction();
		if (action.id == "play") {

		}
		else if (action.id == "map") {
			spWorldMapFrame worldFrame = new WorldMapFrame();
			transitionShowFrame(worldFrame);
		}
		else if (action.id == "level_up") {
			spLevelUpPopup levelUpPopup = new LevelUpPopup();
			transitionShowFrameAsDialog(levelUpPopup);
		}
		else if (action.id == "show_rewards") {
			spLevelRewardsFrame rewardFrame = new LevelRewardsFrame();
			transitionShowFrame(rewardFrame);
		}
		else if (action.id == "shop") {
			spShopFrame shopFrame = new ShopFrame();
			transitionShowFrame(shopFrame);
		}
		else if (action.id == "attach_hat") {
			spAttachHatToAnimalFrame attachFrame = new AttachHatToAnimalFrame("bee");
			transitionShowFrameAsDialog(attachFrame);
		}
		else if (action.id == "choose_start") {
			spChooseStartAnimalFrame chooseAnimal = new ChooseStartAnimalFrame();
			transitionShowFrameAsDialog(chooseAnimal);
		}
		else if (action.id == "achievement") {
			spAchievementFrame achievFrame = new AchievementFrame();
			transitionShowFrame(achievFrame);
		}
		else if (action.id == "memory" || action.id == "dots" || action.id == "shadow" || action.id == "match" || action.id == "pop" || action.id == "discover" || action.id == "jump") {
			spChooseGameDifficultyFrame chooserFrame = new ChooseGameDifficultyFrame();

			Action innerAction = transitionShowFrameAsDialog(chooserFrame, 0, 0);

			if (innerAction.id == "back" || innerAction.id == "_btn_back_") {
				int asd = 10;
			}
			else {
				if (action.id == "memory") {
					spMemoryFrame memoryFrame = new MemoryFrame(innerAction.id);
					transitionShowFrame(memoryFrame);
				}
				else if (action.id == "dots") {
					spConnectDotsFrame connectFrame = new ConnectDotsFrame(innerAction.id);
					transitionShowFrame(connectFrame);
				}
				else if (action.id == "shadow") {
					spFindShadowFrame findShadow = new FindShadowFrame(innerAction.id);
					transitionShowFrame(findShadow);
				}
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
				}
				else if (action.id == "jump") {
					spJumpOverFrame jumpFrame = new JumpOverFrame(innerAction.id);
					transitionShowFrame(jumpFrame);
				}

			}
		}
		else if (action.id == "back" || action.id == "_btn_back_" || action.id == "close") {
			break;
		}
	}

	return _lastAction;
}

void TestFrame::setData() {
	VectorArray<spActor> dupaArray;
	dupaArray._vector.resize(0);

	dupaArray.push(createButtonWithListener("memory", "memory"));
	dupaArray.push(createButtonWithListener("dots", "dots"));
	dupaArray.push(createButtonWithListener("shadow", "shadow"));
	dupaArray.push(createButtonWithListener("match", "match"));
	dupaArray.push(createButtonWithListener("pop", "pop"));
	dupaArray.push(createButtonWithListener("discover", "discover"));
	dupaArray.push(createButtonWithListener("jump", "jump"));
	dupaArray.push(createButtonWithListener("choose_start", "choose_start"));
	dupaArray.push(createButtonWithListener("shop", "shop"));
	dupaArray.push(createButtonWithListener("show_rewards", "show_rewards"));
	dupaArray.push(createButtonWithListener("level_up", "level_up"));
	dupaArray.push(createButtonWithListener("map", "map"));
	dupaArray.push(createButtonWithListener("attach_hat", "attach_hat"));
	dupaArray.push(createButtonWithListener("achievement", "achievement"));
	
	spAnimatableElementContainer gameContainer = new AnimatableElementContainer(Vector2(_view->getWidth(), _view->getHeight()));
	gameContainer->addChildren(dupaArray);
	gameContainer->setAnchor(0.5f, 0.5f);
	gameContainer->setPosition(_view->getWidth() / 2, _view->getHeight() / 2);
	gameContainer->attachTo(_view);
}

spTweenButton TestFrame::createButtonWithListener(const std::string& actionName, const std::string& text) {
	spTweenButton button = createButton(actionName, text);
	handleClick(button);
	return button;
}