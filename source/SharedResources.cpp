#include "SharedResources.h"

Resources gameResources;
Resources animalsResources;
Resources editResources;
Resources tilesResources;

const std::string FARM[11] = {"bee", "cat", "cow", "dog", "duck", "goat", "horse", "sheep", "snail", "squirell", "mouse"};

const std::string WINTER[7] = {"fox", "penguin", "polarbear", "walrus", "wolf", "young_seal", "owl_winter"};

const std::string UNDERWATER[5] = {"dolphin", "octopus", "fish_1", "whale", "shark"};

const std::string STEPPE[7] = {"elephant", "girafee", "gnu", "leopard", "rhino", "zebra", "lion"};

const std::string ASIA[7] = {"yak", "tiger", "snake", "peacock", "panda", "monkey_2", "parrot"};

const std::string AUSTRALIA[8] = {"crocodile", "kangaroo", "kiwi", "koala", "ostrich", "tealplatypus", "turtle", "dingo"};

const std::string ALL_ANIMALS[45] = {"bee", "cat", "cow", "dog", "duck", "goat", "horse", "sheep", "snail", "squirell", "owl_winter", "mouse",
								"fox", "penguin", "polarbear", "walrus", "wolf", "young_seal",
								"dolphin", "octopus", "fish_1", "whale", "shark",
								"elephant", "girafee", "gnu", "leopard", "rhino", "zebra", "lion",
								"yak", "tiger", "snake", "peacock", "panda", "monkey_2", "parrot",
								"crocodile", "kangaroo", "kiwi", "koala", "ostrich", "tealplatypus", "turtle", "dingo"};

spTextActor createTextfield(const std::string &txt, bool isHtml, bool multiline) {
	spTextActor text = new TextActor();
	text->setAnchor(0.5f, 0.5f);
	text->setStyle(createTextStyle(gameResources.getResFont("nobile_bold")->getFont(),  Color(255, 255, 255, 255), true, TextStyle::HALIGN_CENTER, TextStyle::VALIGN_MIDDLE));
	if (isHtml) {
		text->setHtmlText(txt);
	}
	else {
		text->setText(txt);
	}
	return text;
}

spTweenButton createButton(const std::string &name, const std::string &txt) {
	spTweenButton button = new TweenButton();
	button->setName(name);
	bool shouldAddText = true;
	bool shouldAddSprite = false;
	const char *spriteName = "";

	int r = rand() % 3 + 1;
	const char *str;
	switch(r) {
		case 1:
			str = "greenButton";
			break;
		case 2:
			str = "lightBlueButton";
			break;
		case 3:
			str = "redButton";
			break;
	}

	if (name == "facebook") {
		shouldAddText = false;
		str = "button_facebook";
	}
	else if (name == "rate") {
		shouldAddSprite = true;
		spriteName = "star";
	}

	button->setResAnim(gameResources.getResAnim(str));
	button->setAnchor(Vector2(0.5,0.5));
	button->setTouchChildrenEnabled(false);
	button->setBaseScale(getRoot()->getHeight() * 0.075f / button->getHeight());

	if (shouldAddText) {
		spTextActor text = new TextActor();

		text->setStyle(createTextStyle(gameResources.getResFont("nobile_bold")->getFont(),  Color(255, 255, 255, 255), false, TextStyle::HALIGN_CENTER, TextStyle::VALIGN_MIDDLE));
		text->setText(txt);

		text->setSize(button->getSize());
		button->addChild(text);
	}

	return button;
}

spShaderTweenButton createShaderButton(const std::string &name, const std::string &txt) {
	spShaderTweenButton button = new ShaderTweenButton();
	button->setName(name);
	bool shouldAddText = true;
	bool shouldAddSprite = false;
	const char *spriteName = "";

	int r = rand() % 3 + 1;
	const char *str;
	switch (r) {
	case 1:
		str = "greenButton";
		break;
	case 2:
		str = "lightBlueButton";
		break;
	case 3:
		str = "redButton";
		break;
	}

	if (name == "facebook") {
		shouldAddText = false;
		str = "button_facebook";
	}
	else if (name == "rate") {
		shouldAddSprite = true;
		spriteName = "star";
	}

	button->setResAnim(gameResources.getResAnim(str));
	button->setAnchor(Vector2(0.5,0.5));
	button->setTouchChildrenEnabled(false);
	button->setBaseScale(getRoot()->getHeight() * 0.1f / button->getHeight());

	if (shouldAddText) {
		spTextActor text = new TextActor();

		text->setStyle(createTextStyle(gameResources.getResFont("halycon")->getFont(),  Color(255, 255, 255, 255), false, TextStyle::HALIGN_CENTER, TextStyle::VALIGN_MIDDLE));
		text->setText(txt);

		text->setSize(button->getSize());
		button->addChild(text);
	}

	return button;
}

TextStyle createTextStyle(Font* fontType, Color color, bool multiline, TextStyle::HorizontalAlign hAlign, TextStyle::VerticalAlign vAling) {
	TextStyle style;
	style.font = fontType;
	style.color = color;
	style.hAlign = hAlign;
	style.vAlign = vAling;
	style.fontSize2Scale = 0;
	style.multiline = multiline;

	return style;
}

spTextField createTextFieldInBoundries(const std::string& text, const Vector2& boundries, TextStyle style) {
	spTextField textField = initActor(new TextField,
		arg_style = style,
		arg_width = boundries.x,
		arg_height = boundries.y,
		arg_input = false,
		arg_priority = 10,
		arg_text = text);

	setTextFieldRectToSize(textField, boundries);

	return textField;
}

/*
* todo find better way to set fontsize2scale, much hacks here
*/
void setTextFieldRectToSize(spTextField textField, const Vector2& size) {
	if (textField->getFontSize2Scale() == 0) {
		textField->setFontSize2Scale(textField->getMultiline() ? 20 : 50);
	}
	
	int textSize = 0;
	if (textField->getTextRect().getWidth() > size.x) {
		textSize = int(size.x / textField->getTextRect().getWidth() * textField->getFontSize2Scale());
		textField->setFontSize2Scale(textSize);
	}

	if (textField->getTextRect().getHeight() > size.y) {
		textSize = int(size.y / textField->getTextRect().getHeight() * textField->getFontSize2Scale());
		textField->setFontSize2Scale(textSize);
	}
}

void setSpriteScaleBySize(spSprite sprite, const Vector2& size) {
	float animalScale = 1.0f;
	if (sprite->getWidth() > sprite->getHeight()) {
		animalScale = size.x / sprite->getWidth();
	}
	else {
		animalScale = size.y / sprite->getHeight();
	}
	sprite->setScale(animalScale);
}

float getActorScaleBySize(spActor actor, const Vector2& size) {
	float actorScale = 1.0f;
	if (actor->getWidth() > actor->getHeight()) {
		actorScale = size.x / actor->getWidth();
		if (actorScale * actor->getHeight() > size.y) {
			actorScale = size.y / actor->getHeight();
		}
	}
	else {
		actorScale = size.y / actor->getHeight();
		if (actorScale * actor->getWidth() > size.x) {
			actorScale = size.x / actor->getWidth();
		}
	}

	return actorScale;
}

void setActorScaleBySize(spActor actor, const Vector2& size) {
	actor->setScale(getActorScaleBySize(actor, size));
}

bool isOnScreen(spActor actor) {
	Vector2 startPosition = convert_local2root(actor, Vector2(0, 0));
	RectF intersection = RectF(startPosition, actor->getSize());

	intersection.clip(RectF(Vector2(0.0f, 0.0f), getRoot()->getSize()));
	if (!intersection.isEmpty()) {
		return true;
	}

	return false;
}