#pragma once
#include "TweenButton.h"
#include "TextActor.h"
#include "ShaderTweenButton.h"

#include "oxygine-framework.h"
using namespace oxygine;

extern Resources gameResources;
extern Resources animalsResources;
extern Resources editResources;
extern Resources tilesResources;

extern const std::string FARM[11];
extern const std::string WINTER[7];
extern const std::string UNDERWATER[5];
extern const std::string STEPPE[7];
extern const std::string ASIA[7];
extern const std::string AUSTRALIA[8];
extern const std::string ALL_ANIMALS[45];

spTextActor createTextfield(const std::string &txt, bool isHtml, bool multiline);
spTextField createTextFieldInBoundries(const std::string& text, const Vector2& boundries, TextStyle style);

spTweenButton createButton(const std::string &name, const std::string &txt);
spShaderTweenButton createShaderButton(const std::string &name, const std::string &txt);

TextStyle createTextStyle(Font* fontType, Color color, bool multiline, TextStyle::HorizontalAlign hAlign, TextStyle::VerticalAlign vAling);

void setTextFieldRectToSize(spTextField sprite, const Vector2& size);
void setSpriteScaleBySize(spSprite sprite, const Vector2& size);
float getActorScaleBySize(spActor sprite, const Vector2& size);
void setActorScaleBySize(spActor sprite, const Vector2& size);

bool isOnScreen(spActor actor);
