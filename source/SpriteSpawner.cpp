#include "SpriteSpawner.h"

#define READ_XML 1


using namespace FlashUtils;

Array<World*> *SpriteSpawner::levelArray = 0;

void SpriteSpawner::AddLevel(double delay, WorldData *ldata) {
	//return;
	World *wrld = new World();
	for (int i = 0; i < ldata->waves.length(); ++i) {
		WaveData *wd = ldata->waves[i];
		//unsigned int len = static_cast<unsigned int>((*curWave)->length);
		double q = wd->d;
		SpriteContainer *wv = new SpriteContainer();
		for(int j = 0; j < wd->groups.length(); j++) {
			SpriteModel *ga = wd->groups[j];
			wv->addGroup(*ga);
		}
		wrld->addWave(wv);
	}
	SpriteSpawner::levelArray->push(wrld);
}

void SpriteSpawner::readLevel(pugi::xml_node& child, float stageWidth, float stageHeight) {
	float delayAmount = 0.0f;
	Array<WaveData*> waves;
	WaveData* wavePtrs[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

	pugi::xml_node levelContents = child.first_child();
	while(!levelContents.empty()) {
		const char *name = levelContents.name();
		pugi::xml_attribute attr;

		if (!strcmp(name, "delay" )) {
			pugi::xml_node delayChild = levelContents.first_child();
			while(!delayChild.empty()) {
				delayAmount = atof(delayChild.value());
				delayChild = delayChild.first_child();
			}
		}
		else if(!strcmp(name, "c")) {
			waves.push(ReadWave(levelContents, stageWidth, stageHeight));
		}
		levelContents = levelContents.next_sibling();
	}

	int waveLength = waves.length();
	for(int i = 0; i < waveLength; ++i)
	{
		wavePtrs[i] = waves[i];
	}
	WorldData* data = new WorldData(wavePtrs[0], wavePtrs[1], wavePtrs[2], 
									wavePtrs[3], wavePtrs[4], wavePtrs[5], 
									wavePtrs[6], wavePtrs[7], wavePtrs[8]);
	AddLevel(delayAmount,data);
}
WaveData* SpriteSpawner::ReadWave(pugi::xml_node& child, float stageWidth, float stageHeight) {
	float durationAmount = 0.0f;
	Array<SpriteModel*>* groups = new Array<SpriteModel*>();

	pugi::xml_node waveContents = child.first_child();
	while(!waveContents.empty()) {
		const char *name = waveContents.name();

		if (!strcmp(name, "duration" )) {
			pugi::xml_node durationChild = waveContents.first_child();
			while(!durationChild.empty())
			{
				durationAmount = atof(durationChild.value());
				durationChild = durationChild.first_child();
			}
		}
		else if(!strcmp(name, "g" )) {
			//this "group" needs to be added to the array
			Array<SpriteModel*>* readGroups = ReadGroup(waveContents, stageWidth, stageHeight);
			int NumberOfGroupsRead = readGroups->length();
			for(int i = 0; i < NumberOfGroupsRead; ++i) {
				groups->push((*readGroups)[i]);
			}
		}

		waveContents = waveContents.next_sibling();
	}

	WaveData* data = new WaveData(durationAmount);
	data->concat(groups);

	return data;
}
unsigned int getFinalColor(int red, int green, int blue) {
	unsigned int r = (0xFF0000 & (red << 16));
	unsigned int g = (0x00FF00 & (green << 8));
	unsigned int b = (0x0000FF & blue);

	return r | g | b;
}

Array<SpriteModel*>* SpriteSpawner::ReadGroup(pugi::xml_node& child, float stageWidth, float stageHeight) {
	Array<SpriteModel*>* groupArray = new Array<SpriteModel*>();

	float scaleX = 0.0f;
	float scaleY = 0.0f;
	float positionX = 0.0f;
	float positionY = 0.0f;
	int zPriority = 0;
	string spriteName;

	int idx = 0;
	// get real data here

	pugi::xml_node groupContents = child.first_child();
	while(!groupContents.empty()) {
		const char *name = groupContents.name();
		
		if (!strcmp(name, "sX" )) {
			pugi::xml_node subChild = groupContents.first_child();
			while(!subChild.empty()) {
				scaleX = atof(subChild.value());
				subChild = subChild.first_child();
			}
		}
		else if (!strcmp(name, "sY" )) {
			pugi::xml_node subChild = groupContents.first_child();
			while(!subChild.empty()) {
				scaleY = atof(subChild.value());
				subChild = subChild.first_child();
			}
		}
		else if (!strcmp(name, "x" ) || !strcmp(name, "x1")) {
			pugi::xml_node subChild = groupContents.first_child();
			while(!subChild.empty()) {
				positionX = atof(subChild.value());
				subChild = subChild.first_child();
			}
		}
		else if(!strcmp(name, "y" ) || !strcmp(name, "y1")) {
			pugi::xml_node subChild = groupContents.first_child();
			while(!subChild.empty()) {
				positionY = atof(subChild.value());
				subChild = subChild.first_child();
			}
		}
		else if (!strcmp(name, "z")) {
			pugi::xml_node subChild = groupContents.first_child();
			while(!subChild.empty()) {
				zPriority = atoi(subChild.value());
				subChild = subChild.first_child();
			}
		}
		else if(!strcmp(name, "s" )) {
			pugi::xml_node subChild = groupContents.first_child();
			while(!subChild.empty()) {
				spriteName = subChild.value();
				subChild = subChild.first_child();
			}
		}

		groupContents = groupContents.next_sibling();
	}

	//===================

	SpriteModel* data = new SpriteModel(scaleX, scaleY, positionX, positionY, zPriority, spriteName.c_str());
	groupArray->push(data);
		

	return groupArray;
}

unsigned int SpriteSpawner::ReadColor(pugi::xml_node& child)
{
	struct clr {
		clr():red(0), green(0), blue(0){}
		int red;
		int green;
		int blue;
	};

	clr color;

	pugi::xml_node colorContents = child.first_child();
	while(!colorContents.empty()) {
		const char *name = colorContents.name();
		if(!strcmp(name, "r" )) {
			pugi::xml_node redChild = colorContents.first_child();
			color.red = atoi(redChild.value());
		}
		else if(!strcmp(name, "g")) {
			pugi::xml_node greenChild = colorContents.first_child();
			color.green = atoi(greenChild.value());
		}
		else if(!strcmp(name, "b")) {
			pugi::xml_node blueChild = colorContents.first_child();
			color.blue = atoi(blueChild.value());
		}

		colorContents = colorContents.next_sibling();
	}

	return getFinalColor(color.red, color.green, color.blue);
}

/*
*	Loads all levels from file or mockuped
*
*/
void SpriteSpawner::readLevelsXml(const string &filePath, float stageWidth, float stageHeight) {
	file::buffer fb;
	file::read(filePath.c_str(), fb);

	pugi::xml_document doc;
	doc.load_buffer_inplace(&fb.data[0], fb.data.size());

	pugi::xml_node levelData = doc.first_child();
	pugi::xml_node child = levelData.first_child();
	
	int levelNumber = 0;
	while(!child.empty()) {
		const char *name = child.name();

		if (!strcmp(name, "w")) {
			++levelNumber;
			readLevel(child, stageWidth, stageHeight);
		}
		child = child.next_sibling();
	}

}

#define RGB CMath::RGBToHex
void SpriteSpawner::Initialize(const Point &size) {
	levelArray = new Array<World*>();

	double halfx = getRoot()->getWidth() / 2;
	double halfy = getRoot()->getHeight() / 2;

//#if READ_XML
	readLevelsXml("xmls/WorldData.xml", halfx * 2, halfy * 2);
//#else
	
//	AddLevel(3, new WorldData(new WaveData(2, new SpriteModel(1.0, 1.0, halfx, halfy, "bee.png"))));//, halfy, 0, 0, 5, RGB(126, 0, 0), 0, 0, 0, 0, -1))));
	
	//2
	//AddLevel(0.5, new LevelData(new WaveData(2, new SpriteModel(halfx, halfy, 0, 0, 5, RGB(0, 126, 0), 0, 0, 0, 0, -1)) ));
	//3
	//AddLevel(0.5, new LevelData(new WaveData(2, new SpriteModel(halfx, halfy, 0, 0, 5, RGB(0, 0, 126), 0, 0, 0, 0, -1)) ));
//#endif
}

Array<Group*>* SpriteSpawner::getLevelData(int levelNumber = 0) {
	return (*levelArray)[levelNumber]->getLevelData();//World::levelNumber
}