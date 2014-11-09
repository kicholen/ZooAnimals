#include "FarmControlPanel.h"
#include "FlashUtils.h"
#include "AnimatableElementContainer.h"

FarmControlPanel::FarmControlPanel(Vector2 size, Vector2 protrudeSize) {
	setAnchor(0.5f, 0.5f);
	setSize(Vector2(size.x, size.y));
	setPosition(-getWidth() / 2, getRoot()->getDerivedHeight() / 2);
	_state = fcpHidden;
	_protrudeSize = protrudeSize;

	spColorRectSprite bg = new ColorRectSprite();
	bg->setColor(Color(222, 100, 100));
	bg->setSize(getSize());
	bg->setAnchor(0.0f, 0.0f);
	bg->setPosition(0.0f, 0.0f);
	bg->setPriority(-1000);
	addChild(bg);

	addChild(createBackground());
	_farmBasePanel = createFarmBasePanel();
	_farmBasePanel->attachTo(this);
}

void FarmControlPanel::handleClick(Event *event) {
	const string &name = event->target->getName();
	
	if (name == "back" || name == "_btn_back_") {

	}
	else if (name == "farm_base_panel") {
		FarmBasePanel::FarmBasePanelEvent *fbpe = safeCast<FarmBasePanel::FarmBasePanelEvent*>(event);
		const string eventName = fbpe->_name;

		if (eventName == "play") {
			showGamesView();
		}
		else if (eventName == "shop") {
			showShopView();
		}
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
	const string &name = te->tween->getName();

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
}

void FarmControlPanel::showShopView() {
	if (_state != fcpPartShown) {
		return;
	}
	_state = fcpAnimating;

	spTween tween = addTween(Sprite::TweenX(getRoot()->getDerivedWidth() / 2), TWEEN_DURATION, 1, false, 0, Tween::ease_outBack);

	tween->setName("shop_view");
	tween->setDoneCallback(CLOSURE(this, &FarmControlPanel::onTweenEnded));
}

void FarmControlPanel::hideFullView() {
	if (_state == fcpGameView || _state == fcpShopView) {
		_state = fcpAnimating;

		spTween tween = addTween(Sprite::TweenX(-(getDerivedWidth() / 2 - _protrudeSize.x)), TWEEN_DURATION, 1, false, 0, Tween::ease_outBack);

		tween->setName("show_part");
		tween->setDoneCallback(CLOSURE(this, &FarmControlPanel::onTweenEnded));
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
	spFarmBasePanel farmBasePanel = new FarmBasePanel(Vector2(_protrudeSize.x, _protrudeSize.y));
	farmBasePanel->setAnchor(0.0f, 0.0f);
	farmBasePanel->setPosition(Vector2(getWidth() - _protrudeSize.x, 0.0f));
	farmBasePanel->setName("farm_base_panel");
	farmBasePanel->addEventListener(FarmBasePanel::FarmBasePanelEvent::CLICK_EVENT, CLOSURE(this, &FarmControlPanel::handleClick));

	return farmBasePanel;
}

spButton FarmControlPanel::createButton(const string& actionName, const string& buttonResAnim) {
	spButton button = new Button();
	button->setName(actionName);
	button->setResAnim(gameResources.getResAnim(buttonResAnim));
	button->setInputChildrenEnabled(false);
	button->addEventListener(TouchEvent::CLICK, CLOSURE(this, &FarmControlPanel::handleClick));

	return button;
}

void FarmControlPanel::createElementContainerIfNeeded() {
	if (_gameContainer == NULL) {
		Array<spActor> dupaArray;
		dupaArray._vector.resize(0);

		//Memory, connects dots, find shadow, match two
		dupaArray.push(createButton("memory", "back_button")); 
		dupaArray.push(createButton("dots", "back_button"));
		dupaArray.push(createButton("shadow", "back_button"));
		dupaArray.push(createButton("match", "back_button"));
		dupaArray.push(createButton("match", "back_button"));
		dupaArray.push(createButton("match", "back_button"));
		dupaArray.push(createButton("match", "back_button"));
		dupaArray.push(createButton("match", "back_button"));
		dupaArray.push(createButton("match", "back_button"));
		dupaArray.push(createButton("match", "back_button"));
		dupaArray.push(createButton("match", "back_button"));
		dupaArray.push(createButton("match", "back_button"));
		dupaArray.push(createButton("match", "back_button"));
		dupaArray.push(createButton("match", "back_button"));
		dupaArray.push(createButton("match", "back_button"));
		dupaArray.push(createButton("match", "back_button"));
		dupaArray.push(createButton("match", "back_button"));
		dupaArray.push(createButton("match", "back_button"));
		dupaArray.push(createButton("match", "back_button"));

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