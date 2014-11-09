#include "FarmControlPanel.h"
#include "FlashUtils.h"
#include "AnimatableElementContainer.h"

FarmControlPanel::FarmControlPanel(Vector2 size, Vector2 protrudeSize) {
	setAnchor(0.5f, 0.5f);
	setSize(Vector2(size.x, size.y));
	setPosition(-getWidth() / 2, getRoot()->getDerivedHeight() / 2);
	_score = 2225; // inject this
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
	_elementsContainer = createStackContainer();
	_elementsContainer->attachTo(this);
	_scoreTextField = createScoreTextField();
	_elementsContainer->addChild(createButton("play", "back_button"));
	_elementsContainer->addChild(createButton("shop", "back_button"));
	_elementsContainer->addChild(_scoreTextField);
}

void FarmControlPanel::handleClick(Event *event) {
	const string &name = event->target->getName();
	
	if (name == "back" || name == "_btn_back_") {

	}
	else if (name == "play") {
		showGamesView();
	}
	else if (name == "shop") {
		showShopView();
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

spStackContainer FarmControlPanel::createStackContainer() {
	spStackContainer elementsContainer = new StackContainer(Vector2(_protrudeSize.x, _protrudeSize.y));
	elementsContainer->setAnchor(0.0f, 0.0f);
	elementsContainer->setPosition(Vector2(getWidth() - _protrudeSize.x, 0.0f));

	return elementsContainer;
}

spTextActor FarmControlPanel::createScoreTextField() {
	spTextActor scoreTextField = createTextfield(FlashUtils::CMath::intToString(_score), true, 0, false);
	scoreTextField->setStyle(createTextStyle(gameResources.getResFont("halycon")->getFont(), Color(144, 217, 88), false, TextStyle::HALIGN_RIGHT, TextStyle::VALIGN_MIDDLE));
	scoreTextField->setFontSize2Scale(20.0f * getRoot()->getWidth() / 480.0f);
	scoreTextField->setSize(_protrudeSize.x * 0.4f, _protrudeSize.y * 0.2f);
	scoreTextField->setPriority(3);

	return scoreTextField;
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

		_gameContainer = new AnimatableElementContainer(Vector2(getWidth() - _protrudeSize.x, getHeight()));
		_gameContainer->addChildren(dupaArray);
		_gameContainer->setAnchor(0.5f, 0.5f);
		_gameContainer->setPosition(getWidth() / 2 - _protrudeSize.x / 2, getRoot()->getHeight() / 2);
		_gameContainer->attachTo(this);
	}
	_gameContainer->hideContainerElements();
}

void FarmControlPanel::updateScore(int score) {
	removeTweensByName("score_tween");
	addTween(FarmControlPanel::TweenScore(score), 200)->setName("score_tween");
}

void FarmControlPanel::setScore(int score) {
	if (_score == score) {
		return;
	}

	_score = score;
	_scoreTextField->setText(FlashUtils::CMath::intToString(_score));
}

int FarmControlPanel::getScore() const {
	return _score;
}