#include "FarmBasePanel.h"


FarmBasePanel::FarmBasePanel(Vector2 size, StackContainerAlign alignType) {
	setData(size, alignType);
	_score = 74239042; // todo
	_scoreTextField = createScoreTextField();
	addChild(createButton("play", "back_button"));
	addChild(createButton("shop", "back_button"));
	addChild(_scoreTextField);
}

spTextActor FarmBasePanel::createScoreTextField() {
	spTextActor scoreTextField = createTextfield(FlashUtils::CMath::intToString(_score), true, 0, false);
	scoreTextField->setStyle(createTextStyle(gameResources.getResFont("halycon")->getFont(), Color(144, 217, 88), false, TextStyle::HALIGN_RIGHT, TextStyle::VALIGN_MIDDLE));
	scoreTextField->setFontSize2Scale(20.0f * getRoot()->getWidth() / 480.0f);
	scoreTextField->setSize(getSize().x * 0.4f, getSize().y * 0.2f);
	scoreTextField->setPriority(3);

	return scoreTextField;
}

spButton FarmBasePanel::createButton(const string& actionName, const string& buttonResAnim) {
	spButton button = new Button();
	button->setName(actionName);
	button->setResAnim(gameResources.getResAnim(buttonResAnim));
	button->setInputChildrenEnabled(false);
	button->addEventListener(TouchEvent::CLICK, CLOSURE(this, &FarmBasePanel::handleClick));

	return button;
}

void FarmBasePanel::handleClick(Event *event) {
	const string &name = event->target->getName();

	FarmBasePanelEvent gameEvent(FarmBasePanelEvent::CLICK_EVENT, name);
	dispatchEvent(&gameEvent);
}

void FarmBasePanel::updateScore(int score) {
	removeTweensByName("score_tween");
	addTween(FarmBasePanel::TweenScore(score), 200)->setName("score_tween");
}

void FarmBasePanel::setScore(int score) {
	if (_score == score) {
		return;
	}

	_score = score;
	_scoreTextField->setText(FlashUtils::CMath::intToString(_score));
}

int FarmBasePanel::getScore() const {
	return _score;
}