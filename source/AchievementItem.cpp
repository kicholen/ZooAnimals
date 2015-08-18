#include "AchievementItem.h"
#include "SharedResources.h"
#include "LanguageManager.h"

AchievementItem::AchievementItem(const Vector2& size) {
	setSize(size);
	setTouchEnabled(false);
	setAnchor(0.5f, 0.5f);
}

AchievementItem::~AchievementItem() {

}

void AchievementItem::setData(spAchievementModel model) {
	setTitle(model->getLockitTitle());
	setDescription(model->getLockitDescription());
	setIcon(model->getResourceName());
	createBackground();

	float previousProgress = model->getCurrentPart() - 1 < 0 ? 0 : (float)model->getProgressNeededByPart(model->getCurrentPart() - 1);
	float neededProgress = (float)model->getProgressNeededByPart(std::min(model->getAllPartsCount() - 1, model->getCurrentPart() + 1));
	float currentProgress = (float)model->getProgress();
	spProgressBarBox9 progressBar = createProgressBar((currentProgress - previousProgress) / (neededProgress - previousProgress));
	spTextField partTextfield = setPartTextField(model->getCurrentPart(), model->getAllPartsCount());
	partTextfield->setPosition(progressBar->getX() + progressBar->getWidth() / 2.0f, progressBar->getY() - partTextfield->getDerivedHeight());
}

void AchievementItem::setTitle(int lockitId) {
	spTextField textField = createText(LanguageManager::instance.getText(lockitId), Vector2(getWidth() * 0.66f, getHeight() * 0.2f), true);
	textField->setX(getWidth() * 0.33f);
	textField->attachTo(this);
}

void AchievementItem::setDescription(int lockitId) {
	spTextField textField = createText(LanguageManager::instance.getText(lockitId), Vector2(getWidth(), getHeight() * 0.2f), true);
	textField->setPosition(getWidth() / 2.0f, getHeight() / 2.0f);
	textField->setAnchor(0.5f, 0.5f);
	textField->attachTo(this);
}

spTextField AchievementItem::setPartTextField(int currentPart, int maxPart) {
	spTextField textField = createText(CMath::intToString(currentPart) + "/" + CMath::intToString(maxPart), Vector2(getWidth(), getHeight() * 0.1f), true);
	textField->attachTo(this);

	return textField;
}

void AchievementItem::setIcon(const std::string& resourceName) {
	spSprite iconSprite = new Sprite();
	iconSprite->setResAnim(gameResources.getResAnim(resourceName));
	setSpriteScaleBySize(iconSprite, Vector2(getWidth() *  0.33f, getHeight()));
	iconSprite->attachTo(this);
}

void AchievementItem::createBackground() {
	spBox9Sprite cardBackground = new Box9Sprite;
	cardBackground->setHorizontalMode(Box9Sprite::STRETCHING);
	cardBackground->setVerticalMode(Box9Sprite::STRETCHING);
	cardBackground->setAnchor(0.5f, 0.5f);
	cardBackground->setResAnim(gameResources.getResAnim("greyBox9"));
	cardBackground->setSize(getSize());
	cardBackground->setPosition(getWidth() / 2.0f, getHeight() / 2.0f);
	cardBackground->setGuides(9, 20, 9, 15);
	cardBackground->attachTo(this);
	cardBackground->setPriority(-1);
}

spProgressBarBox9 AchievementItem::createProgressBar(float progress) {
	spProgressBarBox9 progressBar = new ProgressBarBox9(Vector2(getWidth() * 0.7f, getHeight() * 0.2f));
	progressBar->setPosition(getWidth() - progressBar->getWidth() - 20.0f, getHeight() - progressBar->getHeight() - 20.0f); // todo remove magical number
	progressBar->setPriority(2);
	progressBar->setProgress(progress);
	addChild(progressBar);

	return progressBar;
}

spTextField AchievementItem::createText(const std::string& text, const Vector2& boundries, bool multiline) {
	TextStyle style;
	style.font = gameResources.getResFont("nobile_bold")->getFont();
	style.vAlign = TextStyle::VALIGN_MIDDLE;
	style.hAlign = TextStyle::HALIGN_CENTER;
	style.multiline = multiline;
	style.color = Color(35, 145, 245);

	return createTextFieldInBoundries(text, boundries, style);
}