#ifndef _DISCOVERIMAGEFRAME_
#define _DISCOVERIMAGEFRAME_

#include "oxygine-framework.h"
#include "CustomFrame.h"
#include "DiscoverImageField.h"
#include "MathQuizElement.h"

using namespace oxygine;

DECLARE_SMART(DiscoverImageFrame, spDiscoverImageFrame);

class DiscoverImageFrame : public CustomFrame
{
public:
	DiscoverImageFrame(const std::string& difficulty);
	~DiscoverImageFrame();

	void selectTransitions();

	Action loop();

protected:
	void _postHiding(Event *);
	void _preShowing(Event *);

	void onFinished(Event *event);
	void onCorrectAnswer(Event *event);

	void setData();
private:
	spDiscoverImageField _field;
	spMathQuizElement _quizElement;
	int _previousAnimal;
	int _totalScore;

	std::string _difficulty;
};

#endif