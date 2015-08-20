#include "SettingsFrame.h"
#include "Settings.h"
#include "LanguageManager.h"
#include "SharedResources.h"

SettingsFrame::SettingsFrame() {
	init("LandingPageFrame.xml", false);
	selectTransitions();
}

SettingsFrame::~SettingsFrame() {

}

void SettingsFrame::selectTransitions() {
	spTransition transition = new TransitionMove;
	setTransitionIn(transition);
	setTransitionOut(transition);
}

void SettingsFrame::_postHiding(Event *) {
	//FlurryAnalytics::instance.onLevelLeaveEvent(_whichLevel.c_str());
	_view->removeChildren();
	_resources.unload();
	Settings::instance.getValue("music").set_value(_music ? 1 : 0);
	Settings::instance.getValue("sound").set_value(_sound ? 1 : 0);
	Settings::instance.save();
}

void SettingsFrame::_preShowing(Event *) {
	//FlurryAnalytics::instance.onLevelEnterEvent(_whichLevel.c_str());
	selectTransitions();
	_resources.load();
	setData();
}

Action SettingsFrame::loop() {
	while (1) {
		Action action = waitAction();
		if (action.id == "back" || action.id == "_btn_back_" || action.id == "close") {
			break;
		}
	}

	return _lastAction;
}

void SettingsFrame::setData() {
	_music = Settings::instance.getValue("music").as_int();
	_sound = Settings::instance.getValue("sound").as_int();
	
	createPopupBackground();
	spTextField titleTextfield = createTitleTextfield();
	
	float yPosition = titleTextfield->getY() + titleTextfield->getDerivedHeight() + 10.0f;
	float xTextPosition = _view->getWidth() / 2.0f - _view->getWidth() / 8.0f;
	float xButtonPosition = _view->getWidth() / 2.0f + _view->getWidth() / 8.0f;

	titleTextfield = createNormalTextfield(100);
	titleTextfield->setPosition(Vector2(xTextPosition, yPosition));
	createSwitchButton("music", _music)->setPosition(xButtonPosition, yPosition);
	yPosition += _view->getHeight() * 0.2f;

	titleTextfield = createNormalTextfield(100);
	titleTextfield->setPosition(Vector2(xTextPosition, yPosition));
	createSwitchButton("sound", _sound)->setPosition(xButtonPosition, yPosition);
}

void SettingsFrame::createPopupBackground() {
	spBox9Sprite cardBackground = new Box9Sprite;
	cardBackground->setHorizontalMode(Box9Sprite::STRETCHING);
	cardBackground->setVerticalMode(Box9Sprite::STRETCHING);
	cardBackground->setAnchor(0.5f, 0.5f);
	cardBackground->setResAnim(gameResources.getResAnim("greyBox9"));
	cardBackground->setSize(_view->getSize() / 2.0f);
	cardBackground->setPosition(_view->getWidth() / 2.0f, _view->getHeight() / 2.0f);
	cardBackground->setGuides(9, 20, 9, 15);
	cardBackground->attachTo(_view);
	cardBackground->setPriority(-1);
}

spTextField SettingsFrame::createTitleTextfield() {
	TextStyle style;
	style.font = gameResources.getResFont("nobile_bold")->getFont();
	style.vAlign = TextStyle::VALIGN_MIDDLE;
	style.hAlign = TextStyle::HALIGN_CENTER;
	style.multiline = true;
	style.color = Color(35, 145, 245);

	spTextField textField = createTextFieldInBoundries(LanguageManager::instance.getText(100), Vector2(_view->getWidth() / 2.0f, _view->getHeight() * 0.2f), style);
	textField->setAnchorX(0.5f);
	textField->setX(_view->getWidth() / 2.0f);
	textField->setY(_view->getHeight() * 0.2f);
	_view->addChild(textField);

	return textField;
}

spTextField SettingsFrame::createNormalTextfield(int lockitId) {
	TextStyle style;
	style.font = gameResources.getResFont("nobile_bold")->getFont();
	style.vAlign = TextStyle::VALIGN_MIDDLE;
	style.hAlign = TextStyle::HALIGN_CENTER;
	style.multiline = true;
	style.color = Color(35, 145, 245);

	spTextField textField = createTextFieldInBoundries(LanguageManager::instance.getText(100), Vector2(_view->getWidth() / 2.0f, _view->getHeight() * 0.2f), style);
	textField->setX(_view->getWidth() / 2.0f);
	textField->setAnchor(0.5f, 0.5f);
	_view->addChild(textField);

	return textField;

}
spButton SettingsFrame::createSwitchButton(const std::string& name, bool isOn) {
	spButton button = new Button();
	button->setName(name);
	button->setResAnim(gameResources.getResAnim(isOn ? "button_true" : "button_false"));
	button->attachTo(_view);
	button->addEventListener(TouchEvent::CLICK, CLOSURE(this, &SettingsFrame::onButtonClicked));
	button->setTouchChildrenEnabled(false);
	button->setAnchor(0.5f, 0.5f);
	setSpriteScaleBySize(button, Vector2(_view->getWidth() / 6.0f, _view->getHeight() / 6.0f));

	return button;
}

void SettingsFrame::onButtonClicked(Event *event) {
	const std::string& name = event->currentTarget->getName();
	spButton button = safeSpCast<Button>(event->currentTarget);

	if (name == "music") {
		_music = !_music;
		button->setResAnim(gameResources.getResAnim(_music ? "button_true" : "button_false"));
	}
	else if (name == "sound") {
		_sound = !_sound;
		button->setResAnim(gameResources.getResAnim(_sound ? "button_true" : "button_false"));
	}
}