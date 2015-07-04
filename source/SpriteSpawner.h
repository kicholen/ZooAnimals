#pragma once
#include "FlashUtils.h"
#include "core/STDFileSystem.h"
#include "pugixml/pugixml.hpp"
#include "core/file.h"
#include "SpriteModel.h"
#include "SpriteContainer.h"
#include "World.h"

using namespace FlashUtils;

//class SpriteModel;

class WaveData
{
public:
	WaveData(double d, SpriteModel *g1 = NULL, SpriteModel *g2 = NULL, SpriteModel *g3 = NULL, SpriteModel *g4 = NULL, SpriteModel *g5 = NULL, SpriteModel *g6 = NULL, SpriteModel *g7 = NULL, SpriteModel *g8 = NULL, SpriteModel *g9 = NULL, SpriteModel *g10= NULL, SpriteModel *g11= NULL, SpriteModel *g12= NULL, SpriteModel *g13= NULL)
	{
		this->d = d;
		if (g1)
			groups.push(g1);
		if (g2)
			groups.push(g2);
		if (g3)
			groups.push(g3);
		if (g4)
			groups.push(g4);
		if (g5)
			groups.push(g5);
		if (g6)
			groups.push(g6);
		if (g7)
			groups.push(g7);
		if (g8)
			groups.push(g8);
		if (g9)
			groups.push(g9);
		if (g10)
			groups.push(g10);
		if (g11)
			groups.push(g11);
		if (g12)
			groups.push(g12);
		if (g13)
			groups.push(g13);
	}

	WaveData *concat(VectorArray<SpriteModel*> *groups1, VectorArray<SpriteModel*> *groups2 = NULL, VectorArray<SpriteModel*> *groups3 = NULL, VectorArray<SpriteModel*> *groups4 = NULL, VectorArray<SpriteModel*> *groups5 = NULL)
	{
		if (groups1)
			add(groups1);
		if (groups2)
			add(groups2);
		if (groups3)
			add(groups3);
		if (groups4)
			add(groups4);
		if (groups5)
			add(groups5);
		return this;
	}

	WaveData *concat(SpriteModel *groups1, SpriteModel *groups2 = NULL, SpriteModel *groups3 = NULL, SpriteModel *groups4 = NULL, SpriteModel *groups5 = NULL, SpriteModel *groups6 = NULL)
	{
		if (groups1)
			add(groups1);
		if (groups2)
			add(groups2);
		if (groups3)
			add(groups3);
		if (groups4)
			add(groups4);
		if (groups5)
			add(groups5);
		if (groups6)
			add(groups6);
		return this;
	}

	WaveData *add(VectorArray<SpriteModel*> *g)
	{
		for (int i = 0; i < g->length(); ++i)
		{
			groups.push((*g)[i]);
		}
		return this;
	}

	WaveData *add(SpriteModel *g)
	{
		groups.push(g);

		return this;
	}

	double d;
	VectorArray<SpriteModel *> groups;

};

class WorldData
{
public:
	VectorArray<WaveData*> waves;

	WorldData(WaveData *wave, WaveData *wave2 = NULL, WaveData *wave3 = NULL, WaveData *wave4 = NULL, WaveData *wave5 = NULL, WaveData *wave6 = NULL, WaveData *wave7 = NULL, WaveData *wave8 = NULL, WaveData *wave9 = NULL)
	{
		if (wave)
			waves.push(wave);
		if (wave2)
			waves.push(wave2);
		if (wave3)
			waves.push(wave3);
		if (wave4)
			waves.push(wave4);
		if (wave5)
			waves.push(wave5);
		if (wave6)
			waves.push(wave6);
		if (wave7)
			waves.push(wave7);
		if (wave8)
			waves.push(wave8);
		if (wave9)
			waves.push(wave9);
	}
};

class SpriteSpawner
{
public:
	static VectorArray<World*> *levelArray;
	/*
		flash.Array levelArray = new Array(
			[waveLength, [groupx, groupy, spawnWidth, spawnHeight, number, color, col_rand, rel_pos, xvel, yvel], [ANOTHER GROUP DEFINITION]]
		);
	*/
	static void AddLevel(double delay, WorldData *levelArray);

protected:
	static VectorArray<SpriteModel*> *ReturnTransport(double xpos, double ypos, double rad, unsigned int speed);

	static VectorArray<SpriteModel*> *ReturnWall(double x1, double y1, double x2, double y2, int enum_, int rnum, unsigned int col);

public:

	static void Initialize(const Point &size);

	static void Think(double dt);

	static VectorArray<Group*>* getLevelData(int levelNumber);

	static void Start();

private:
	static void readLevelsXml(const std::string &filePath, float stageWidth, float stageHeight );
	static void readLevel(pugi::xml_node& child, float stageWidth, float stageHeight);
	static WaveData* ReadWave(pugi::xml_node& child, float stageWidth, float stageHeight);
	static VectorArray<SpriteModel*>* ReadGroup(pugi::xml_node& child, float stageWidth, float stageHeight);
	static unsigned int ReadColor(pugi::xml_node& child);
};

