#pragma once
#include "TweenButton.h"
#include "TextActor.h"
#include "ShaderTweenButton.h"

#include "oxygine-framework.h"
using namespace oxygine;

extern Resources gameResources;
extern Resources animalsResources;
extern Resources editResources;

extern const string FARM[12];
extern const string WINTER[6];
extern const string UNDERWATER[5];
extern const string STEPPE[7];
extern const string ASIA[8];
extern const string AUSTRALIA[8];
extern const string ALL_ANIMALS[46];

spTextActor createTextfield(const string &txt, bool isHtml, int scale2size, bool multiline);
spTweenButton createButton(const string &name, const string &txt);
spShaderTweenButton createShaderButton(const string &name, const string &txt);

TextStyle createTextStyle(Font* fontType, Color color, bool multiline, TextStyle::HorizontalAlign hAlign, TextStyle::VerticalAlign vAling);