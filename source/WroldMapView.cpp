#include "WroldMapView.h"
#include "SharedResources.h"

WroldMapView::WroldMapView(Vector2 size) {
	setSize(size);

	spColorRectSprite bg = new ColorRectSprite();
	bg->setColor(Color::Aqua);
	bg->setSize(getSize());
	bg->setAnchor(0.0f, 0.0f);
	bg->setPosition(0.0f, 0.0f);
	bg->setPriority(-1000);
	addChild(bg);
}

WroldMapView::~WroldMapView() {

}

void WroldMapView::setData() {
	addMapSprite();

	showNorthAmerica();
	showSouthAmerica();
	showGreenland();
	showAfrica();
	showAsia();
	showEurope();
	showAustralia();
	animateSprite(_europe);
}

void WroldMapView::onContinentClicked(Event *event) {
	spSprite sprite = safeSpCast<Sprite>(event->currentTarget);
	animateSprite(sprite);

	WroldMapViewEvent switchedEvent(WroldMapViewEvent::CONTINENT_SWITCHED, sprite->getName());
	dispatchEvent(&switchedEvent);
}

void WroldMapView::animateSprite(spSprite sprite) {
	if (!_animatedSprite) {
		_animatedSprite = initActor(new Sprite,
			arg_priority = -10,
			arg_input = false,
			arg_attachTo = this,
			arg_anchor = Vector2(0.5, 0.5f),
			arg_color = Color::Azure);
	}
	else {
		_animatedSprite->removeTweens();
		_animatedSprite->setAlpha(255);
	}

	_animatedSprite->setPosition(sprite->getPosition());
	_animatedSprite->setResAnim(sprite->getResAnim());
	_animatedSprite->setScale(sprite->getScale());

	float scaleModifier = getMapScaleModifier(sprite->getResAnim()->getName());

	_animatedSprite->addTween(TweenScale(_animatedSprite->getScale() * scaleModifier), 1500, -1, false);
	_animatedSprite->addTween(TweenAlpha(0), 1500, -1, false);
}

void WroldMapView::addMapSprite() {
	_map = initActor(new Sprite,
		arg_input = false,
		arg_attachTo = this,
		arg_color = Color::AntiqueWhite,
		arg_resAnim = gameResources.getResAnim("world_map"));
	_map->setScale(getActorScaleBySize(_map, getSize()));
}

void WroldMapView::showNorthAmerica() {
	if (!_northAmerica) {
		_northAmerica = initActor(new Sprite,
			arg_attachTo = this,
			arg_color = Color::Beige,
			arg_anchor = Vector2(0.5f, 0.5f),
			arg_name = "underwater",
			arg_resAnim = gameResources.getResAnim("north_america"));
		_northAmerica->setScale(_map->getScale());
		_northAmerica->setPosition(_map->getScaleX() * 130.0f, _map->getScaleY() * 122.0f);
		_northAmerica->addEventListener(TouchEvent::CLICK, CLOSURE(this, &WroldMapView::onContinentClicked));
	}
}

void WroldMapView::showSouthAmerica() {
	if (!_southAmerica) {
		_southAmerica = initActor(new Sprite,
			arg_attachTo = this,
			arg_color = Color::Bisque,
			arg_anchor = Vector2(0.5f, 0.5f),
			arg_name = "underwater",
			arg_resAnim = gameResources.getResAnim("south_america"));
		_southAmerica->setScale(_map->getScale());
		_southAmerica->setPosition(_map->getScaleX() * 232.0f, _map->getScaleY() * 324.0f);
		_southAmerica->addEventListener(TouchEvent::CLICK, CLOSURE(this, &WroldMapView::onContinentClicked));
	}
}

void WroldMapView::showGreenland() {
	if (!_greenland) {
		_greenland = initActor(new Sprite,
			arg_attachTo = this,
			arg_color = Color::PaleVioletRed,
			arg_anchor = Vector2(0.5f, 0.5f),
			arg_name = "winter",
			arg_resAnim = gameResources.getResAnim("greenland"));
		_greenland->setScale(_map->getScale());
		_greenland->setPosition(_map->getScaleX() * 277.0f, _map->getScaleY() * 47.0f);
		_greenland->addEventListener(TouchEvent::CLICK, CLOSURE(this, &WroldMapView::onContinentClicked));
	}
}

void WroldMapView::showAfrica() {
	if (!_africa) {
		_africa = initActor(new Sprite,
			arg_attachTo = this,
			arg_color = Color::Turquoise,
			arg_anchor = Vector2(0.5f, 0.5f),
			arg_name = "steppe",
			arg_resAnim = gameResources.getResAnim("africa"));
		_africa->setScale(_map->getScale());
		_africa->setPosition(_map->getScaleX() * 398.5f, _map->getScaleY() * 256.5f);
		_africa->addEventListener(TouchEvent::CLICK, CLOSURE(this, &WroldMapView::onContinentClicked));
	}
}

void WroldMapView::showEurope() {
	if (!_europe) {
		_europe = initActor(new Sprite,
			arg_attachTo = this,
			arg_color = Color::Tomato,
			arg_anchor = Vector2(0.5f, 0.5f),
			arg_name = "farm",
			arg_resAnim = gameResources.getResAnim("europe"));
		_europe->setScale(_map->getScale());
		_europe->setPosition(_map->getScaleX() * 417.5f, _map->getScaleY() * 90.0f);
		_europe->addEventListener(TouchEvent::CLICK, CLOSURE(this, &WroldMapView::onContinentClicked));
	}
}

void WroldMapView::showAustralia() {
	if (!_australia) {
		_australia = initActor(new Sprite,
			arg_attachTo = this,
			arg_color = Color::LightSteelBlue,
			arg_anchor = Vector2(0.5f, 0.5f),
			arg_name = "australia",
			arg_resAnim = gameResources.getResAnim("australia"));
		_australia->setScale(_map->getScale());
		_australia->setPosition(_map->getScaleX() * 692.5f, _map->getScaleY() * 332.0f);
		_australia->addEventListener(TouchEvent::CLICK, CLOSURE(this, &WroldMapView::onContinentClicked));
	}
}

void WroldMapView::showAsia() {
	if (!_asia) {
		_asia = initActor(new Sprite,
			arg_attachTo = this,
			arg_color = Color::OldLace,
			arg_anchor = Vector2(0.5f, 0.5f),
			arg_name = "asia",
			arg_resAnim = gameResources.getResAnim("asia"));
		_asia->setScale(_map->getScale());
		_asia->setPosition(_map->getScaleX() * 599.5f, _map->getScaleY() * 151.0f);
		_asia->addEventListener(TouchEvent::CLICK, CLOSURE(this, &WroldMapView::onContinentClicked));
	}
}

float WroldMapView::getMapScaleModifier(const string& name) {
	if (name == "north_america") {
		return 1.2f;
	}
	else if (name == "south_america") {
		return 1.2f;
	}
	else if (name == "greenland") {
		return 1.4f;
	}
	else if (name == "africa") {
		return 1.2f;
	}
	else if (name == "europe") {
		return 1.3f;
	}
	else if (name == "australia") {
		return 1.3f;
	}
	else if (name == "asia") {
		return 1.1f;
	}

	return 1.0f;
}