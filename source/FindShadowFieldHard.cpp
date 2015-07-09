#include "FindShadowFieldHard.h"
#include "Content.h"
#include "SharedResources.h"

FindShadowFieldHard::FindShadowFieldHard(Vector2 size, int difficulty) {
	setSize(size);
	setParametersFromContent(difficulty);
	createElementsInCanvasContainer();

	restart();
}

FindShadowFieldHard::~FindShadowFieldHard() {

}

void FindShadowFieldHard::restart() {
	_elementsCount = CMath::random(_minElements, _maxElements);
	_elementsInCanvasContainer->removeChildren();
	_rolledNamesVector.clear();
	_rolledNamesMap.clear();
	int index = 0;
	pugi::xml_node game = Content::instance.getGameNode("shadow_match");
	VectorArray<spActor> dupaArray;
	dupaArray._vector.resize(0);

	while (index < _elementsCount) {
		pugi::xml_node child = game.first_child();

		while (!child.empty()) {
			std::string name = child.first_attribute().as_string();

			if (CMath::Rand(0.0f, 1.0f) < 0.3f && _rolledNamesMap.count(name) <= 0) {
				_rolledNamesMap.insert(std::make_pair(name, index));
				index++;
				_rolledNamesVector.push(name);
				dupaArray.push(createTouchableSprite(name));
			}

			if (index == _elementsCount) {
				break;
			}

			child = child.next_sibling();
		}
	}
	std::random_shuffle(dupaArray._vector.begin(), dupaArray._vector.end());

	_elementsInCanvasContainer->addChildren(dupaArray);
	_elementsInCanvasContainer->randomizeChildrenViewParams();
	_elementsInCanvasContainer->hideContainerElements();

	createSpriteToBeFoundIfDoesntExist(_rolledNamesVector[_rolledNamesVector.length() - 1]);
	_elementsInCanvasContainer->showContainerElements(true);
}

void FindShadowFieldHard::setParametersFromContent(int difficulty) {
	pugi::xml_node params = Content::instance.getGameNode(difficulty == 1 ? "shadow_match_hard" : "shadow_match_medium");
	pugi::xml_attribute attribute = params.first_attribute();

	while (!attribute.empty()) {
		const char *name = attribute.name();

		if (!strcmp(name, "min")) {
			_minElements = attribute.as_int();
		}
		else if (!strcmp(name, "max")) {
			_maxElements = attribute.as_int();
		}

		attribute = attribute.next_attribute();
	}
}

void FindShadowFieldHard::createElementsInCanvasContainer() {
	if (!_elementsInCanvasContainer) {
		_elementsInCanvasContainer = new AnimatableElementContainer(Vector2(getWidth() * 0.5f, getHeight()));
		_elementsInCanvasContainer->setAnchor(0.5f, 0.5f);
		_elementsInCanvasContainer->setPosition(getWidth() / 2.0f + _elementsInCanvasContainer->getWidth() / 2.0f, getRoot()->getHeight() / 2.0f);
		_elementsInCanvasContainer->attachTo(this);
	}
	_elementsInCanvasContainer->setAnimationType(aecScale);
	_elementsInCanvasContainer->hideContainerElements();
}

spSprite FindShadowFieldHard::createTouchableSprite(const std::string name) {
	spSprite sprite = initActor(new Sprite,
		arg_anchor = Vector2(0.5f, 0.5f),
		arg_name = name,
		arg_color = Color(0, 0, 0),
		arg_resAnim = animalsResources.getResAnim(name));

	sprite->addEventListener(TouchEvent::CLICK, CLOSURE(this, &FindShadowFieldHard::onSpriteClicked));

	return sprite;
}

void FindShadowFieldHard::createSpriteToBeFoundIfDoesntExist(const std::string name) {
	if (_spriteToBeFound == NULL) {
		_spriteToBeFound = initActor(new Sprite,
			arg_anchor = Vector2(0.5, 0.5f),
			arg_attachTo = this);
		_spriteToBeFound->setPosition(getWidth() / 4.0f, getHeight() / 2.0f);
	}
	_spriteToBeFound->setName(name);
	_spriteToBeFound->setResAnim(animalsResources.getResAnim(name));
	setActorScaleBySize(_spriteToBeFound, Vector2(getWidth() / 4.0f, getHeight() / 4.0f));
}

void FindShadowFieldHard::onSpriteClicked(Event *ev) {
	spSprite sprite = safeSpCast<Sprite>(ev->currentTarget);

	if (sprite->getName() == _spriteToBeFound->getName()) {
		dispatchGoodShadowClicked();
		_rolledNamesMap.erase(sprite->getName());
		_rolledNamesVector._vector.pop_back();
		animateCorrectSprite(sprite);

		if (isEnd()) {
			dispatchLevelCompletedEvent();
		}
		else {
			createSpriteToBeFoundIfDoesntExist(_rolledNamesVector[_rolledNamesVector.length() - 1]);
		}
	}
	else {
		animateIncorrectSprite(sprite);
		dispatchWrongShadowClicked();
	}
	
}

void FindShadowFieldHard::animateCorrectSprite(spSprite sprite) {
	sprite->setColor(Color());
	sprite->addTween(TweenScale(sprite->getScaleX() * 1.4f), 400)->setDoneCallback(CLOSURE(this, &FindShadowFieldHard::onCorrectSpriteFinishedAnimating));
}

void FindShadowFieldHard::animateIncorrectSprite(spSprite sprite) {

}

void FindShadowFieldHard::onCorrectSpriteFinishedAnimating(Event *ev) {
	spSprite sprite = safeSpCast<Sprite>(ev->currentTarget);
	sprite->detach();
}

bool FindShadowFieldHard::isEnd() {
	return _rolledNamesVector.length() == 0;
}

void FindShadowFieldHard::dispatchLevelCompletedEvent() {
	FindShadowFieldHardEvent findEvent(FindShadowFieldHardEvent::LEVEL_ENDED);
	dispatchEvent(&findEvent);
}

void FindShadowFieldHard::dispatchWrongShadowClicked() {
	FindShadowFieldHardEvent findEvent(FindShadowFieldHardEvent::SHADOW_WRONG);
	dispatchEvent(&findEvent);
}

void FindShadowFieldHard::dispatchGoodShadowClicked() {
	FindShadowFieldHardEvent findEvent(FindShadowFieldHardEvent::SHADOW_FOUND);
	dispatchEvent(&findEvent);
}