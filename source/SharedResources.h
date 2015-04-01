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

extern const string FARM[11];
extern const string WINTER[7];
extern const string UNDERWATER[5];
extern const string STEPPE[7];
extern const string ASIA[8];
extern const string AUSTRALIA[8];
extern const string ALL_ANIMALS[45];

spTextActor createTextfield(const string &txt, bool isHtml, bool multiline);
spTweenButton createButton(const string &name, const string &txt);
spShaderTweenButton createShaderButton(const string &name, const string &txt);

TextStyle createTextStyle(Font* fontType, Color color, bool multiline, TextStyle::HorizontalAlign hAlign, TextStyle::VerticalAlign vAling);

void setSpriteScaleBySize(spSprite sprite, const Vector2& size);
float getActorScaleBySize(spActor sprite, const Vector2& size);
void setActorScaleBySize(spActor sprite, const Vector2& size);

bool isOnScreen(spActor actor);