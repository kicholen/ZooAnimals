#include "FarmBasePanel.h"


FarmBasePanel::FarmBasePanel(Vector2 size, StackContainerAlign alignType) {
	setData(size, alignType);
	_score = 2222; // todo
	_scoreTextField = createScoreTextField();
	addChild(createTweenButton("play", "back_button"));
	addChild(createTweenButton("shop", "back_button"));
	addChild(_scoreTextField);
}

spTextActor FarmBasePanel::createScoreTextField() {
	spTextActor scoreTextField = createTextfield(FlashUtils::CMath::intToString(_score), true, false);
	TextStyle style = createTextStyle(gameResources.getResFont("halycon")->getFont(), Color(144, 217, 88), true, TextStyle::HALIGN_CENTER, TextStyle::VALIGN_MIDDLE);
	scoreTextField->setStyle(style);
	scoreTextField->setFontSize2Scale(15);
	scoreTextField->setSize(getSize().x * 0.9f, getSize().y * 0.2f);
	scoreTextField->setFontSize2Scale(getSize().x * 0.9f / scoreTextField->getTextRect().getWidth() * scoreTextField->getFontSize2Scale());
	scoreTextField->setPriority(3);

	return scoreTextField;
}

spTweenButton FarmBasePanel::createTweenButton(const string& actionName, const string& buttonResAnim) {
	spTweenButton button = new TweenButton();
	button->setName(actionName);
	button->setResAnim(gameResources.getResAnim(buttonResAnim));
	button->setTouchChildrenEnabled(false);
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