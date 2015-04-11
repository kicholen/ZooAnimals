#include "FarmServiceElement.h"
#include "SharedResources.h"

FarmServiceElement::FarmServiceElement(Vector2 size) {
	setSize(size);
}

FarmServiceElement::~FarmServiceElement() {

}

void FarmServiceElement::setData(spAnimalModel model) {
	int currentHappiness = model->happinessValue();
	int needed = model->happinessNeededForNextAnimal();
	int neededBefore = model->happinessNeededForCurrentAnimal();

	createHappinessProgressBar(float(currentHappiness - neededBefore) / float(needed - neededBefore));
	createGameButtonByType(topGame, model->topGame());
	createGameButtonByType(midGame, model->midGame());
	createGameButtonByType(lowGame, model->lowGame());
}

void FarmServiceElement::createAnimalSprite(spAnimalModel model) {
	_animalSprite = initActor(new Sprite,
		arg_anchor = Vector2(0.5f, 0.5f),
		arg_attachTo = this,
		arg_resAnim = animalsResources.getResAnim(model->animalName()));
	setActorScaleBySize(safeSpCast<Actor>(_animalSprite), getSize() * 0.2f);
	_animalSprite->setPosition(getSize() * 0.2f / 2);
}

void FarmServiceElement::createHappinessProgressBar(float happinessValue) {
	spSprite progressBarBackground = initActor(new Sprite,
		arg_attachTo = this,
		arg_anchor = Vector2(0.5f, 0.5f),
		arg_position = Vector2(getWidth() / 6, getHeight() / 2),
		arg_resAnim = gameResources.getResAnim("whiteProgressBarVertical"));
	progressBarBackground->setScale(getWidth() * 0.2f / progressBarBackground->getWidth(), getHeight() * 0.8f / progressBarBackground->getHeight());

	_happinessProgressBar = initActor(new ProgressBar,
		arg_attachTo = this,
		arg_anchor = Vector2(0.5f, 0.5f),
		arg_position = Vector2(getWidth() / 6, getHeight() / 2),
		arg_resAnim = gameResources.getResAnim("redProgressBarVertical"));
	_happinessProgressBar->setDirection(_happinessProgressBar->dir_90);
	_happinessProgressBar->setScale(progressBarBackground->getScale());
	_happinessProgressBar->setProgress(happinessValue);
}

void FarmServiceElement::createGameButtonByType(FarmServiceGameType type, const string& gameName) {
	if (type == topGame) {
		_topGameButton = createGameButton("back_button", gameName);
		_topGameButton->setPosition(Vector2(getWidth() * 2 / 3, getHeight() / 4));
	}
	else if (type == midGame) {
		_midGameButton = createGameButton("back_button", gameName);
		_midGameButton->setPosition(Vector2(getWidth() / 2, getHeight() * 3 / 4));
	}
	else if (type == lowGame) {
		_lowGameButton = createGameButton("back_button", gameName);
		_lowGameButton->setPosition(Vector2(getWidth() * 4 / 5, getHeight() * 3 / 4));
	}
	else {
		oxygine::log::warning("FarmServiceElement::createGameButton: wrong type.");
	}
}

spTweenButton FarmServiceElement::createGameButton(const string& resourceName, const string& buttonName) {
	spTweenButton button = initActor(new TweenButton,
		arg_resAnim = gameResources.getResAnim(resourceName),
		arg_name = buttonName,
		arg_anchor = Vector2(0.5f, 0.5f),
		arg_attachTo = this);
	button->addEventListener(TouchEvent::CLICK, CLOSURE(this, &FarmServiceElement::onGameChosen));
	setActorScaleBySize(safeSpCast<Actor>(button), getSize() * 0.3f);

	return button;
}

void FarmServiceElement::onGameChosen(Event *event) {
	const string &name = event->target->getName();

	FarmServiceElementEvent gameEvent(FarmServiceElementEvent::PLAY_GAMES, name);
	dispatchEvent(&gameEvent);
}