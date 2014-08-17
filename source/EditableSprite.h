#ifndef _EDITABLESPRITE_
#define _EDITABLESPRITE_

#include "Sprite.h"
#include "Draggable.h"
#include "TweenButton.h"

using namespace oxygine;

DECLARE_SMART(EditableSprite, spEditableSprite);

class EditableSprite : public Sprite
{
public:
	EditableSprite();

protected:
	void onTouchEvent(Event *event);

	void showAndCreateButtonsIfNecessary();
	void addButton(const string &name, const string &text, Vector2 position, Vector2 anchor);
	void createButtons(bool shouldShow);
	void onButtonTouch(Event *event);
	void showButtons();
	void hideButtons();

private:
	Draggable drag;

	bool _isDoubleClick;
	spTween _doubleClickTimer;
	
	bool _areButtonsCreated;
	bool _areButtonsShown;

	string _state;
};

#endif
