#include "SlideFrame.h"


SlideFrame::SlideFrame() {
	init("SlideFrame.xml");

	selectTransitions();
}

void SlideFrame::selectTransitions() {
	spTransition transition = new TransitionSimpleBlende;
	setTransitionIn(transition);
	setTransitionOut(transition);
}

Action SlideFrame::loop(){
	while (1) {
		Action action = waitAction();
		if (action.id == "close") {
			break;
		}
		else if (action.id == "_btn_back_") {
			//break;
		}
		else if (action.id == "go_back") {
			GoBackToPreviousFrameEvent goBackToPreviousFrameEvent(GoBackToPreviousFrameEvent::GO_BACK);
			dispatchEvent(&goBackToPreviousFrameEvent);
			break;
		}
	}

	return _lastAction;
}

void SlideFrame::onSliderTouchDown(Event *event) {
	_slider->setLocked(false);
}

void SlideFrame::onSliderTouchUp(Event *event) {
	_slider->setLocked(true);
}

void SlideFrame::onSliderCompleted(Event *event) {
	generateAction("go_back");
}

void SlideFrame::_postHiding(Event *) {
	_view->removeChildren();
	
	_resources.unload();
}

void SlideFrame::_preShowing(Event *) {
	selectTransitions();
	setData();
	_resources.load();
}

void SlideFrame::setData() {
	spColorRectSprite touchQuad = new ColorRectSprite();
	touchQuad->setSize(_view->getWidth(), _view->getHeight());
	touchQuad->setColor(Color(0, 0, 255, 0));
	touchQuad->setPosition(0.0f, 0.0f);
	touchQuad->attachTo(_view);
	touchQuad->setName("close");
	handleClick(touchQuad);

	spButton button = new Button();
	button->setResAnim(_resources.getResAnim("back_button"));
	button->setScale(_content->getWidth() * 8 / 100 / button->getWidth());
	button->addEventListener(TouchEvent::TOUCH_DOWN, CLOSURE(this, &SlideFrame::onSliderTouchDown));
	button->addEventListener(TouchEvent::TOUCH_UP, CLOSURE(this, &SlideFrame::onSliderTouchUp));

	float defaultOffset = 12;

	_slider = new Slider(button->getDerivedHeight());
	_slider->setSize(button->getDerivedWidth() + defaultOffset * 2, _content->getHeight() * 90 / 100);

	spColorRectSprite rectangleContainer = new ColorRectSprite();
	rectangleContainer->setSize(_slider->getWidth(), _slider->getHeight() * 2 - button->getDerivedHeight());
	rectangleContainer->setColor(Color(2,2,2,0));

	button->setY(rectangleContainer->getHeight() / 2 - button->getDerivedHeight() / 2);
	button->setX(defaultOffset);

	spColorRectSprite bottomColorSprite = new ColorRectSprite();
	bottomColorSprite->setSize(rectangleContainer->getWidth() - 8.0f, rectangleContainer->getHeight() / 2);
	bottomColorSprite->setColor(Color(94, 94, 94, 255));
	bottomColorSprite->setPosition((rectangleContainer->getWidth() - bottomColorSprite->getWidth()) / 2 + 4.0f, rectangleContainer->getHeight() / 2);
	bottomColorSprite->attachTo(rectangleContainer);

	button->attachTo(rectangleContainer);

	_slider->setContent(rectangleContainer);
	_slider->setPosition(_content->getWidth() * 0.98f - _slider->getWidth(), (_content->getHeight() - _slider->getHeight()) / 2);

	spColorRectSprite topColorSprite = new ColorRectSprite();
	topColorSprite->setSize(rectangleContainer->getWidth() - 6.0f, rectangleContainer->getHeight() / 2);
	topColorSprite->setColor(Color(2, 2, 2, 255));
	topColorSprite->setPosition(_slider->getX() + 1.0f, _slider->getY());
	topColorSprite->attachTo(_view);

	spSprite buttonSprite = new Sprite();
	buttonSprite->setResAnim(_resources.getResAnim("back_button"));
	buttonSprite->setAlpha(130);
	buttonSprite->setScale(button->getScale());
	buttonSprite->setPosition(_slider->getX() + defaultOffset, _slider->getY());
	buttonSprite->attachTo(_view);
	buttonSprite->addTween(Sprite::TweenColor(Color(255,0,0,130)), 2000, -1, true, 2000);

	_slider->attachTo(_view);
	_slider->addEventListener(Slider::CompletedEvent::COMPLETED, CLOSURE(this, &SlideFrame::onSliderCompleted));
	_slider->startDrag();

	spBox9Sprite sliderBackground;
	sliderBackground = new Box9Sprite;
	sliderBackground->setResAnim(_resources.getResAnim("slider_box9"));
	sliderBackground->setSize(_slider->getWidth(), _slider->getHeight() + defaultOffset * 2);
	sliderBackground->setPosition(_slider->getX(), _slider->getY() - defaultOffset);
	sliderBackground->setTouchEnabled(false);
	sliderBackground->setVerticalMode(Box9Sprite::TILING_FULL);
	sliderBackground->setHorizontalMode(Box9Sprite::TILING_FULL);
	sliderBackground->setGuides(50, 200, 40, 200);
	sliderBackground->attachTo(_view);
}