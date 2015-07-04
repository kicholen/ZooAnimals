#include "FarmControlPanel.h"
#include "FlashUtils.h"
#include "AnimatableElementContainer.h"

FarmControlPanel::FarmControlPanel(Vector2 size, Vector2 protrudeSize) {
	setAnchor(0.5f, 0.5f);
	setSize(Vector2(size.x, size.y));
	setPosition(-getWidth() / 2, getRoot()->getDerivedHeight() / 2);
	_state = fcpHidden;
	_protrudeSize = protrudeSize;

	/*spColorRectSprite bg = new ColorRectSprite();
	bg->setColor(Color(222, 100, 100));
	bg->setSize(getSize());
	bg->setAnchor(0.0f, 0.0f);
	bg->setPosition(0.0f, 0.0f);
	bg->setPriority(-1000);
	addChild(bg);*/

	addChild(createBackground());
	_farmBasePanel = createFarmBasePanel();
	_farmBasePanel->attachTo(this);
}

void FarmControlPanel::handleClick(Event *event) {
	const std::string &name = event->target->getName();
	
	if (name == "back" || name == "_btn_back_") {

	}
	else if (name == "farm_base_panel") {
		FarmBasePanel::FarmBasePanelEvent *fbpe = safeCast<FarmBasePanel::FarmBasePanelEvent*>(event);
		const std::string eventName = fbpe->_name;

		if (eventName == "play") {
			showGamesView();
		}
		else if (eventName == "shop") {
			showShopView();
		}
	}
	else if (name == "hide_shop_game_view") {
		hideFullView();
	}
	else {
		FarmControlPanelEvent gameEvent(FarmControlPanelEvent::PLAY_GAMES, name);
		dispatchEvent(&gameEvent);
	}
}

void FarmControlPanel::show() {
	if (_state != fcpHidden) {
		return;
	}
	_state = fcpAnimating;
	
	spTween tween = addTween(Sprite::TweenX(-(getDerivedWidth() / 2 - _protrudeSize.x)), TWEEN_DURATION, 1, false, 0, Tween::ease_inBack);
	tween->setName("show_part");
	tween->setDoneCallback(CLOSURE(this, &FarmControlPanel::onTweenEnded));
}

void FarmControlPanel::hide() {
	if (_state != fcpPartShown) {
		return;
	}
	_state = fcpAnimating;

	spTween tween = addTween(Sprite::TweenX(-getDerivedWidth() / 2), TWEEN_DURATION, 1, false, 0, Tween::ease_inExpo);
	tween->setName("hide");
	tween->setDoneCallback(CLOSURE(this, &FarmControlPanel::onTweenEnded));
}

void FarmControlPanel::onTweenEnded(Event *event) {
	TweenEvent *te = safeCast<TweenEvent*>(event);
	const std::string &name = te->tween->getName();

	if (name == "show_part") {
		_state = fcpPartShown;
	}
	else if (name == "hide") {
		_state = fcpHidden;
	}
	else if (name == "game_view") {
		_state = fcpGameView;
	}
	else if (name == "shop_view") {
		_state = fcpShopView;
	}
	else if (name == "show_game_elements") {
		_gameContainer->showContainerElements(true);
	}
}

void FarmControlPanel::showGamesView() {
	if (_state != fcpPartShown) {
		return;
	}
	_state = fcpAnimating;

	createElementContainerIfNeeded();
	spTween tween = addTween(Sprite::TweenX(getRoot()->getDerivedWidth() / 2), TWEEN_DURATION, 1, false, 0, Tween::ease_outBack);

	tween->setName("game_view");
	tween->setDoneCallback(CLOSURE(this, &FarmControlPanel::onTweenEnded));

	tween = addTween(TweenDummy(), TWEEN_DURATION / 2, 1, false, 0, Tween::ease_outBack);
	tween->setName("show_game_elements");
	tween->setDoneCallback(CLOSURE(this, &FarmControlPanel::onTweenEnded));

	switchFarmBasePanel();
}

void FarmControlPanel::showShopView() {
	if (_state != fcpPartShown) {
		return;
	}
	_state = fcpAnimating;

	spTween tween = addTween(Sprite::TweenX(getRoot()->getDerivedWidth() / 2), TWEEN_DURATION, 1, false, 0, Tween::ease_outBack);

	tween->setName("shop_view");
	tween->setDoneCallback(CLOSURE(this, &FarmControlPanel::onTweenEnded));

	switchFarmBasePanel();
}

void FarmControlPanel::switchFarmBasePanel() {
	unsigned char panelAlpha = _farmBasePanel->getAlpha() == 0 ? 255 : 0;
	unsigned char closeButtonAlpha = panelAlpha == 0 ? 255 : 0;

	_farmBasePanel->addTween(TweenAlpha(panelAlpha), TWEEN_DURATION);
	createCloseButtonIfNeeded()->addTween(TweenAlpha(closeButtonAlpha), TWEEN_DURATION);
}

void FarmControlPanel::hideFullView() {
	if (_state == fcpGameView || _state == fcpShopView) {
		_state = fcpAnimating;

		spTween tween = addTween(Sprite::TweenX(-(getDerivedWidth() / 2 - _protrudeSize.x)), TWEEN_DURATION, 1, false, 0, Tween::ease_outBack);

		tween->setName("show_part");
		tween->setDoneCallback(CLOSURE(this, &FarmControlPanel::onTweenEnded));

		switchFarmBasePanel();

		if (_gameContainer) {
			_gameContainer->hideContainerElements();
		}
	}
}

spBox9Sprite FarmControlPanel::createBackground() {
	spBox9Sprite background = new Box9Sprite;
	background->setAnchor(0.5f, 0.5f);
	background->setResAnim(gameResources.getResAnim("progress_bar_box9"));
	background->setSize(getSize());
	background->setPosition(getWidth() / 2, getHeight() / 2);
	background->setGuides(7, 37, 7, 37);
	background->setPriority(-1);

	return background;
}

spFarmBasePanel FarmControlPanel::createFarmBasePanel() {
	spFarmBasePanel farmBasePanel = new FarmBasePanel(Vector2(_protrudeSize.x * 0.8f, _protrudeSize.y * 0.8f));
	farmBasePanel->setAnchor(0.0f, 0.0f);
	farmBasePanel->setPosition(Vector2(getWidth() - _protrudeSize.x * 0.9f, _protrudeSize.y * 0.1f));
	farmBasePanel->setName("farm_base_panel");
	farmBasePanel->addEventListener(FarmBasePanel::FarmBasePanelEvent::CLICK_EVENT, CLOSURE(this, &FarmControlPanel::handleClick));

	return farmBasePanel;
}

spTweenButton FarmControlPanel::createButton(const std::string& actionName, const std::string& buttonResAnim) {
	spTweenButton button = new TweenButton();
	button->setName(actionName);
	button->setResAnim(gameResources.getResAnim(buttonResAnim));
	button->setTouchChildrenEnabled(false);
	button->addEventListener(TouchEvent::CLICK, CLOSURE(this, &FarmControlPanel::handleClick));

	return button;
}

spTweenButton FarmControlPanel::createCloseButtonIfNeeded() {
	spTweenButton closeButton = getChildT<TweenButton>("hide_shop_game_view", ep_ignore_error);
	if (!closeButton) {
		closeButton = createButton("hide_shop_game_view", "back_button");
		closeButton->attachTo(this);
		closeButton->setAnchor(0.5f, 0.5f);
		closeButton->setScale(getActorScaleBySize(closeButton, _protrudeSize));
		closeButton->setPosition(getWidth() - _protrudeSize.x + closeButton->getDerivedWidth() / 2, closeButton->getDerivedHeight() / 2);
	}

	return closeButton;
}

void FarmControlPanel::createElementContainerIfNeeded() {
	if (!_gameContainer) {
		VectorArray<spActor> dupaArray;
		dupaArray._vector.resize(0);

		dupaArray.push(createButton("memory", "back_button")); 
		dupaArray.push(createButton("dots", "back_button"));
		dupaArray.push(createButton("shadow", "back_button"));
		dupaArray.push(createButton("pop", "back_button"));
		dupaArray.push(createButton("match", "back_button"));
		dupaArray.push(createButton("discover", "back_button"));

		_gameContainer = new AnimatableElementContainer(Vector2(getWidth() - _protrudeSize.x, getHeight()));
		_gameContainer->addChildren(dupaArray);
		_gameContainer->setAnchor(0.5f, 0.5f);
		_gameContainer->setPosition(getWidth() / 2 - _protrudeSize.x / 2, getRoot()->getHeight() / 2);
		_gameContainer->attachTo(this);
	}
	_gameContainer->hideContainerElements();
}

void FarmControlPanel::updateScore(int score) {
	_farmBasePanel->updateScore(score);
}