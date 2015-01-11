#include "ConnectDotsField.h"
#include "SharedResources.h"
#include "FlashUtils.h"
#include "Particle.h"
#include "ParticleEmitter.h"

ConnectDotsField::ConnectDotsField(int levelNumber) {
	setSize(800.0f, 800.0f);//MEMORY_SIZE_X * _numberOfFields.x, MEMORY_SIZE_Y * _numberOfFields.y);
	
	/*spColorRectSprite bg = new ColorRectSprite();
	bg->setColor(Color(100, 100, 100));
	bg->setSize(getSize());
	bg->setAnchor(0.0f, 0.0f);
	bg->setPosition(0.0f, 0.0f);
	bg->setPriority(-1000);
	addChild(bg);*/
	reset(levelNumber);
}

void ConnectDotsField::reset(int level) {
	addEventListener(TouchEvent::MOVE, CLOSURE(this, &ConnectDotsField::onFieldMove));
	_animatedCount = 0;
	_state = cdAnimating;
	_currentSelected = 0;
	fillField(level);
}

void ConnectDotsField::fillField(int levelNumber) {
	int bufferIndex = 1;

	VectorArray<Group*> *groupArray = new VectorArray<Group*>();

	groupArray = SpriteSpawner::getLevelData(levelNumber);

	for(int i = 0; i < groupArray->length(); i++) {
		Group &g = *(*groupArray)[i];
		if (g.spriteName == "quad_40") {
			string name = FlashUtils::CMath::intToString(bufferIndex);

			spConnectDotElement dot = getChildT<ConnectDotElement>(name, oxygine::ep_ignore_error);
			if (!dot) {
				dot = new ConnectDotElement(name);
				dot->setAnchor(0.5f, 0.5f);
				dot->setName(name);
				dot->addEventListener(TouchEvent::CLICK, CLOSURE(this, &ConnectDotsField::onDotTap));
				dot->addEventListener(TouchEvent::OVER, CLOSURE(this, &ConnectDotsField::onDotOver));
				addChild(dot);
			}
			dot->setVisible(true);
			dot->setScale(0.0f);
			_animatedCount++;
			Vector2 baseDotScale = Vector2((float)g[0] * getWidth() / dot->getWidth(), (float)g[1] * getHeight() / dot->getHeight());
			dot->setBaseScale(baseDotScale);
			spTween tween = dot->addTween(Sprite::TweenScale(baseDotScale), 250, 1, false, 75 * bufferIndex, Tween::ease_inOutBack);
			tween->setDoneCallback(CLOSURE(this, &ConnectDotsField::onDotAnimationCompleted));
			dot->setPosition((float)g[2] * getWidth(), (float)g[3] * getHeight());
			bufferIndex++;
		}
		else {
			createOrChangeMaskedSprite(g);
		}
	}
	_numberOfDots = bufferIndex - 1;
}

void ConnectDotsField::onDotAnimationCompleted(Event *event) {
	_animatedCount--;

	if (_animatedCount == 0) {
		_state = cdWaiting;
		animateNextIfNecessary();
	}
}

void ConnectDotsField::onDotTap(Event *event) {
	spConnectDotElement dot = safeSpCast<ConnectDotElement>(event->currentTarget);

	if (isCorrectDotAndCanProceed(dot)) {
		onCorrectDotFound(dot);
	}
}

void ConnectDotsField::onDotOver(Event *event) {
	spConnectDotElement dot = safeSpCast<ConnectDotElement>(event->currentTarget);

	if (isCorrectDotAndCanProceed(dot)) {
		onCorrectDotFound(dot);
	}
}

void ConnectDotsField::onFieldMove(Event *event) {
	if (_state == cdFirstDotFound) {
		TouchEvent *touchEvent = safeCast<TouchEvent*>(event);
		setLineDimensionsAndRotation(_lineSprite, touchEvent->localPosition);
	}
}

bool ConnectDotsField::animateNextIfNecessary() {
	int nextDotIndex = _currentSelected + 1;

	if (nextDotIndex > _numberOfDots) {
		return false;
	}
	else {
		getChildT<ConnectDotElement>(FlashUtils::CMath::intToString(nextDotIndex))->playAnimation();
		return true;
	}
}

bool ConnectDotsField::isCorrectDotAndCanProceed(spConnectDotElement dot) {
	if (_state == cdWaiting || _state == cdFirstDotFound) {
		if (dot->getName() == FlashUtils::CMath::intToString(_currentSelected + 1)) {
			return true;
		}
	}
	return false;
}

void ConnectDotsField::onCorrectDotFound(spConnectDotElement dot) {
	if (_currentSelected > 0) {
		_previousDot = _currentDot;
	}
	_currentDot = dot;
	_currentDot->stopAnimation();
			
	setStrechingLineAndCreateIfNeeded(_currentDot->getPosition());
	if (_currentSelected > 0) {
		createStaticLine(_previousDot->getPosition(), _currentDot->getPosition());
	}
	_currentSelected = _currentSelected + 1;
	
	if (!animateNextIfNecessary()) {
		onComplete();
	}
}

void ConnectDotsField::drawLineBeetweenFirstAndLastDot() {
	spConnectDotElement lastDot = getChildT<ConnectDotElement>(FlashUtils::CMath::intToString(_currentSelected));
	spConnectDotElement firstDot = getChildT<ConnectDotElement>(FlashUtils::CMath::intToString(1));

	createStaticLine(lastDot->getPosition(), firstDot->getPosition());
}

void ConnectDotsField::createStaticLine(Vector2 startPosition, Vector2 endPosition) {
	spColorRectSprite previousLine = new ColorRectSprite();
	previousLine->setColor(Color(10, 150, 22));
	previousLine->setAnchor(0.0f, 0.5f);
	previousLine->setSize(5.0f, 5.0f);
	previousLine->setPosition(startPosition);
	previousLine->setName("static_line");
	setLineDimensionsAndRotation(previousLine, endPosition);
	addChild(previousLine);
}

void ConnectDotsField::setStrechingLineAndCreateIfNeeded(Vector2 _startPosition) {
	if (_lineSprite.get() == NULL) {
		_lineSprite = new ColorRectSprite();
		addChild(_lineSprite);
	}
	_lineSprite->setVisible(true);
	_lineSprite->setColor(Color(10, 150, 22));
	_lineSprite->setAnchor(0.0f, 0.5f);
	_lineSprite->setSize(5.0f, 5.0f);
	_lineSprite->setPosition(_startPosition);
	_state = cdFirstDotFound;
}

void ConnectDotsField::setLineDimensionsAndRotation(spColorRectSprite line, Vector2 to) {
	float distance = calculateDistance(line->getPosition(), to);
	line->setWidth(distance);
	float angle = calculateAngle(line->getPosition(), to);
	line->setRotation(angle);
}

float ConnectDotsField::calculateDistance(Vector2 from, Vector2 to) {
	return sqrt((to.x - from.x) * (to.x - from.x) + (to.y - from.y) * (to.y - from.y));
}

float ConnectDotsField::calculateAngle(Vector2 to, Vector2 from) {
	return atan2(from.y - to.y, from.x - to.x);
}

void ConnectDotsField::dispatchFinishedEvent(Event *event) {
	spParticleEmitter particleEmitter = getChildT<ParticleEmitter>("particle_emitter", oxygine::ep_ignore_error);
	particleEmitter->detach();
	ConnectDotsFieldEvent finishedEvent(ConnectDotsFieldEvent::FINISHED);
	dispatchEvent(&finishedEvent);
}

void ConnectDotsField::createOrChangeMaskedSprite(Group g) {
	spSprite mask = getChildT<Sprite>("object_mask", oxygine::ep_ignore_error);
	if (!mask) {
		mask = new Sprite();
		mask->setName("object_mask");
		mask->setAnchor(0.5f, 0.5f);
		mask->setAlpha(1);
		mask->setInputEnabled(false);
		mask->setVisible(false);
		addChild(mask);
	}
	mask->setResAnim(animalsResources.getResAnim(g.spriteName));
	mask->setScale((float)g[0] * getWidth() / mask->getWidth(), (float)g[1] * getHeight() / mask->getHeight());
	mask->setPosition((float)g[2] * getWidth(), (float)g[3] * getHeight());

	spMaskedSprite masked = getChildT<MaskedSprite>("object_masked", oxygine::ep_ignore_error);
	if (!masked) {
		masked = new MaskedSprite();
		masked->setName("object_masked");
		addChild(masked);
		masked->setMask(mask);
	}

	spShaderSprite object = masked->getChildT<ShaderSprite>("object", oxygine::ep_ignore_error);
	if (!object) {
		object = new ShaderSprite();
		object->setName("object");
		object->setAnchor(0.5f, 0.5f);
		object->setPriority(-10);
		object->setInvertShaderProgram();
		masked->addChild(object);
	}
	object->setVal(Vector4(0.5f, 0.5f, 0.5f, 1.0f));
	object->setResAnim(animalsResources.getResAnim(g.spriteName));
	object->setScale((float)g[0] * getWidth() / object->getWidth(), (float)g[1] * getHeight() / object->getHeight());
	object->setPosition((float)g[2] * getWidth(), (float)g[3] * getHeight());
}

void ConnectDotsField::onComplete() {
	removeEventListener(TouchEvent::MOVE, CLOSURE(this, &ConnectDotsField::onFieldMove));
	drawLineBeetweenFirstAndLastDot();
	_lineSprite->setVisible(false);
	_state = cdCompleted;
	animateOnComplete();
}

void ConnectDotsField::animateOnComplete() {
	spActor actor = getFirstChild();

	while (actor) {
		if (actor->getName() == "static_line") {
			actor->addTween(TweenAlpha(0), 300)->setDoneCallback(CLOSURE(this, &ConnectDotsField::onLineDissapear));
		}

		actor = actor->getNextSibling(); 
	}
	for (int i = 1; i <= _numberOfDots; i++) {
		spConnectDotElement dot = getChildT<ConnectDotElement>(FlashUtils::CMath::intToString(i));
		dot->setVisible(false);
	}

	spMaskedSprite object = getChildT<MaskedSprite>("object_masked");
	object->getChild("object")->addTween(ShaderSprite::TweenVal(Vector4(1, 1, 1, 0)), 500);
	addTween(TweenDummy(), 1000)->setDoneCallback(CLOSURE(this, &ConnectDotsField::dispatchFinishedEvent));

	spParticleEmitter particleEmitter = new ParticleEmitter(Vector2(getWidth() / 2, getWidth() / 2), Vector2(getHeight() / 2, getHeight() / 2), Vector2(-0.5, 0.5), Vector2(-0.5, 0.5), Vector2(1, 3), Vector2(9, 20), "sparkle");
	particleEmitter->setRadius(Vector2(getHeight() / 600, getHeight() / 550));
	particleEmitter->pushColor(0x90d958, true);
	particleEmitter->pushColor(0xd95890);
	particleEmitter->setName("particle_emitter");
	particleEmitter->attachTo(this);
}

void ConnectDotsField::onLineDissapear(Event *event) {
	spColorRectSprite quad = safeSpCast<ColorRectSprite>(event->currentTarget);
	removeChild(quad);
}