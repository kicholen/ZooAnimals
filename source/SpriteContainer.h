#pragma once
#include "SpriteModel.h"
#include "FlashUtils.h"
#include "DraggableSprite.h"
#include "res/Resources.h"

using namespace FlashUtils;

class Group
{
public:	
	double &operator [](int i) {
		return v[i];
	}

	double v[15];
	string spriteName;
};

class SpriteContainer
{
public:
	SpriteContainer();

	//Vars:
	VectorArray<Group*> *groupArray;

	//Funcs:
	VectorArray<Group*>* getGroupData();

	void reset();

	void addGroup(SpriteModel &g);
};

