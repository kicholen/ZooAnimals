#include "LevelChooserFrame.h"
#include "RotatingContainer.h"
#include "FlurryAnalytics.h"
#include "FlurryAds.h"
#include "GooglePlayInAppPurchaseManager.h"
#include "Settings.h"
#include "ShaderSprite.h"
#include "ChooseMemoryDifficultyFrame.h"

LevelChooserFrame::LevelChooserFrame() {
	init("LevelChooserFrame.xml", true);
	_counter = 0;
	selectTransitions();
}

void LevelChooserFrame::selectTransitions() {
	spTransition transition = new TransitionMove;
	setTransitionIn(transition);
	setTransitionOut(transition);
}

Action LevelChooserFrame::loop() {
	while (1) {
		Action action = waitAction();
		string::size_type levelChecker = action.id.find("level", 0);
		string::size_type availableChecker = action.id.find("level", 0);

		if (levelChecker != string::npos && availableChecker == string::npos) {
			transitionShowFrameAsDialog(buyItemPopup);
		}
		else if (levelChecker != string::npos && availableChecker != string::npos) {
			spChooseMemoryDifficultyFrame chooserFrame = new ChooseMemoryDifficultyFrame();
			transitionShowFrame(chooserFrame);
			//Settings::instance.getValue("level_enter").set_value(Settings::instance.getValue("level_enter").as_int() + 1);
			//spPresentAnimalsFrame presentAnimalsFrame = new PresentAnimalsFrame(action.id.c_str());
			//transitionShowFrame(presentAnimalsFrame);
		}
		else if (action.id == "close") {
			break;
		}
		else if (action.id == "back") {
			break;
		}
		if (action.id == "wall_post") {
			transitionShowFrameAsDialog(wallPostPopup);
		}
	}

	return _lastAction;
}

void LevelChooserFrame::_postHiding(Event *) {
	_view->removeChildren();
	buyItemPopup->removeEventListener(BuyItemPopup::WasPurchaseCompletedEvent::PURCHASE_COMPLETED, CLOSURE(this, &LevelChooserFrame::onPurchaseCompleted));
	wallPostPopup->removeEventListener(WallPostMessagePopup::FacebookPostMessage::FACEBOOK_POST_MESSAGE_SUCCESS, CLOSURE(this, &LevelChooserFrame::onPostSuccess));
	
	_resources.unload();
}

void LevelChooserFrame::_preShowing(Event *) {
	_counter++;
	selectTransitions();
	setData();
	_resources.load();
	buyItemPopup->addEventListener(BuyItemPopup::WasPurchaseCompletedEvent::PURCHASE_COMPLETED, CLOSURE(this, &LevelChooserFrame::onPurchaseCompleted));
	wallPostPopup->addEventListener(WallPostMessagePopup::FacebookPostMessage::FACEBOOK_POST_MESSAGE_SUCCESS, CLOSURE(this, &LevelChooserFrame::onPostSuccess));
	if (Settings::instance.getValue("level_4").as_int() == 0 && Settings::instance.getValue("level_enter").as_int() % 10 == 0) {
		Settings::instance.getValue("level_enter").set_value(Settings::instance.getValue("level_enter").as_int() + 1);
		generateAction("wall_post");
	}
	else if (_counter == 3) {
		_counter = 0;
		FlurryAds::instance.tryToShowFullScreen();
	}
}

void LevelChooserFrame::onPurchaseCompleted(Event *) {
	addRotatingContainer();
}

void LevelChooserFrame::onPostSuccess(Event *) {
	Settings::instance.getValue("level_4").set_value(1);
	Settings::instance.save();
	addRotatingContainer();
}

void LevelChooserFrame::setData() {
	addRotatingContainer();

	addButton("back", "BACK", Vector2(_view->getWidth() / 2,  _content->getHeight() * 0.9f ));

	spTextActor text = createTextfield("In which place do you want to play?", true, 0, true);
	text->setFontSize2Scale(40 * _view->getWidth() / 640);
	text->setSize(_view->getWidth() * 0.75f, _view->getHeight() * 0.15f);
	text->setPosition(_view->getWidth() / 2, _view->getHeight() * 0.09f);
	text->setColor(Color(35, 145, 245));
	text->attachTo(_view);
	/*float xOffset = 20.0f;
	spTweenButton button = addButton("queryShop", "queryShop", Vector2(xOffset,  _content->getHeight() * 0.8f ));
	xOffset += button->getDerivedWidth() + 20.0f;
	addButton("restorePurchases", "restorePurchases",  Vector2(xOffset,  _content->getHeight() * 0.8f ));
	xOffset += button->getDerivedWidth() + 20.0f;
	addButton("purchaseProductTestProduct", "purchaseProductTestProduct",  Vector2(xOffset,  _content->getHeight() * 0.8f ));*/
}

void LevelChooserFrame::addRotatingContainer() {
	if (_rotatingContainer) {
		_rotatingContainer->detach();
		_rotatingContainer = 0;
	}
	_rotatingContainer = new RotatingContainer();
	_rotatingContainer->setSize(_view->getWidth(), _view->getHeight());
	_rotatingContainer->setPriority(-5);

	string buttons[] = {"level_1", "level_2", "level_3", "level_4", "level_5", "level_6"};
	string buttonsResAnim[] = {"farm_2", "winter_2", "steppe_2", "underwater_2", "asia_2", "australia_2"};
	string iconInsideAnim[] = {"goat", "polarbear", "elephant", "octopus", "peacock", "kangaroo"};

	spColorRectSprite rectangleContainer = new ColorRectSprite();
	rectangleContainer->setSize(_rotatingContainer->getWidth(), _rotatingContainer->getHeight());
	rectangleContainer->setColor(Color(0, 0, 0, 0));
	float offset = 15.0f;
	float positionX = rectangleContainer->getWidth() / 4;
	int sizeOfArray = sizeof(buttons) / sizeof( buttons[0]);
	bool shouldMakeUntouchable = true;
	float lastButtonWidth = 0.0f;

	for (int i = 0; i < sizeOfArray; i++) {
		bool isLevelAvailable = Settings::instance.getValue(buttons[i]).as_int() == 1;
		string buttonName = isLevelAvailable ? buttons[i] : buttons[i].append("_not");
		int randomDelay = FlashUtils::CMath::random(3000, 5000);

		if (!isLevelAvailable) {
			spShaderTweenButton shaderButton = new ShaderTweenButton();
			shaderButton->setName(buttonName);
			shaderButton->setResAnim(gameResources.getResAnim(buttonsResAnim[i]));
			shaderButton->setAnchor(Vector2(0.5,0.5));
			shaderButton->setInputChildrenEnabled(false);
			shaderButton->setBaseScale(getRoot()->getHeight() * 0.40f / shaderButton->getHeight());
			shaderButton->setY(rectangleContainer->getHeight() / 2);
			shaderButton->setX(positionX);
			shaderButton->setMonoShaderProgram();

			spShaderSprite shaderSprite = new ShaderSprite();
			shaderSprite->setResAnim(gameResources.getResAnim(iconInsideAnim[i]));
			spTween shaderTweenDispatcher = createTween(TweenDummy(), randomDelay);
			shaderSprite->addTween(shaderTweenDispatcher)->setDoneCallback(CLOSURE(this, &LevelChooserFrame::onTweenDispatcherDone));
			shaderSprite->setName(iconInsideAnim[i]);
			shaderSprite->setAnchor(0.5f, 0.5f);
			shaderSprite->setScale(shaderButton->getWidth() / shaderSprite->getWidth() * 0.75f, shaderButton->getHeight() / shaderSprite->getHeight() * 0.75f);
			shaderSprite->setPosition(shaderButton->getWidth() / 2, shaderButton->getHeight() / 2);
			shaderSprite->attachTo(shaderButton);
			shaderButton->attachTo(rectangleContainer);
			shaderSprite->setMonoShaderProgram();
			handleClick(shaderButton);
			positionX += shaderButton->getDerivedWidth() + offset;
			lastButtonWidth = shaderButton->getDerivedWidth() + offset;
		}
		else {
			spTweenButton button = new TweenButton();
			button->setName(buttonName);
			button->setResAnim(gameResources.getResAnim(buttonsResAnim[i]));
			button->setAnchor(Vector2(0.5,0.5));
			button->setInputChildrenEnabled(false);
			button->setBaseScale(getRoot()->getHeight() * 0.40f / button->getHeight());
			button->setY(rectangleContainer->getHeight() / 2);
			button->setX(positionX);

			spSprite sprite = new Sprite();
			sprite->setResAnim(gameResources.getResAnim(iconInsideAnim[i]));
			spTween tweenDispatcher = createTween(TweenDummy(), randomDelay);
			sprite->addTween(tweenDispatcher)->setDoneCallback(CLOSURE(this, &LevelChooserFrame::onTweenDispatcherDone));
			sprite->setName(iconInsideAnim[i]);
			sprite->setAnchor(0.5f, 0.5f);
			sprite->setScale(button->getWidth() / sprite->getWidth() * 0.75f, button->getHeight() / sprite->getHeight() * 0.75f);
			sprite->setPosition(button->getWidth() / 2, button->getHeight() / 2);
			sprite->attachTo(button);
			button->attachTo(rectangleContainer);
			handleClick(button);
			positionX += button->getDerivedWidth() + offset;
			lastButtonWidth = button->getDerivedWidth() + offset;
		}
	}
	rectangleContainer->setSize(positionX + _view->getWidth() / 4 - lastButtonWidth, _rotatingContainer->getHeight());
	_rotatingContainer->setContent(rectangleContainer);
	_rotatingContainer->setPosition(_view->getSize() / 2 - _rotatingContainer->getSize() / 2);
	_rotatingContainer->attachTo(_view);
}

void LevelChooserFrame::onTweenDispatcherDone(Event *event) {
	spActor actor = safeSpCast<Actor>(event->currentTarget);
	string name = event->currentTarget->getName();
	int duration = 1000;
	float scaleMultiplier = 1.1f;
	int randomDelay = FlashUtils::CMath::random(3000, 5000);

	spTween tweenDispatcher = createTween(TweenDummy(), randomDelay);
	actor->addTween(tweenDispatcher)->setDoneCallback(CLOSURE(this, &LevelChooserFrame::onTweenDispatcherDone));
	if (name == "goat") {
		actor->addTween(Actor::TweenScale(actor->getScale() * scaleMultiplier), duration / 2, 1, true);
		actor->addTween(Actor::TweenRotation(actor->getRotation() + (float)MATH_PI / 4), duration / 4, 1, true, 0, Tween::ease_inBack);
		actor->addTween(Actor::TweenRotation(actor->getRotation() - (float)MATH_PI / 4), duration / 4, 1, true, 0, Tween::ease_inBack);
	}
	else if (name == "polarbear") {
		actor->addTween(Actor::TweenScale(actor->getScale() * scaleMultiplier), duration / 2, 1, true);
	}
	else if (name == "elephant") {
		actor->addTween(Sprite::TweenScale(actor->getScaleX() * scaleMultiplier), duration / 2, 1, true);
	}
	else if (name == "octopus") {
		actor->addTween(Sprite::TweenRotation(actor->getRotation() + (float)MATH_PI * 2), duration, 1, true);
	}
	else if (name == "peacock") {
		actor->addTween(TweenAnim(gameResources.getResAnim("peacock")), duration, 1, false);
	}
	else if (name == "kangaroo") {
		actor->addTween(Actor::TweenRotation(actor->getRotation() + (float)MATH_PI / 4), duration / 4, 1, true, 0, Tween::ease_inBack);
		actor->addTween(Actor::TweenRotation(actor->getRotation() - (float)MATH_PI / 4), duration / 4, 1, true, 0, Tween::ease_inBack);
	}
	else {
		OX_ASSERT(false);
	}
}