#ifndef _ACHIEVEMENTITEM_
#define _ACHIEVEMENTITEM_

#include "oxygine-framework.h"
#include "AchievementModel.h"
#include "ProgressBarBox9.h"

using namespace oxygine;

DECLARE_SMART(AchievementItem, spAchievementItem);

class AchievementItem : public Actor
{
public:
	AchievementItem(const Vector2& size);
	~AchievementItem();

	void setData(spAchievementModel model);

private:
	void setTitle(int lockitId);
	void setDescription(int lockitId);
	spTextField setPartTextField(int currentPart, int maxPart);
	void setIcon(const std::string& resourceName);
	spProgressBarBox9 createProgressBar(float progress);

	void createBackground();

	spTextField createText(const std::string& text, const Vector2& boundries, bool multiline);

};

#endif