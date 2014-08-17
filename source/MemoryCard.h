#ifndef _MEMORYCARD_
#define _MEMORYCARD_

#include "oxygine-framework.h"
#include "AnimalSlot.h"

#define MEMORY_SIZE_X 75
#define MEMORY_SIZE_Y 75
#define DEG_TO_RAD 0.0174532925

using namespace oxygine;

DECLARE_SMART(MemoryCard, spMemoryCard);

typedef enum {mcNormal, mcSelected, mcScored, mcDropped} MemoryCardState;

class MemoryCard : public Actor
{
	MemoryCardState _state;
	bool _locked;

public:
	MemoryCard(string cardId);

	Point index;
	MemoryCardState getState() {
		return _state;
	}
	void setState(MemoryCardState new_state) {
		_state = new_state;
	}
	
	string getId() {
		return _animalSlot->getId();
	}
	
	void select();
	void unSelect();

	bool isID(spMemoryCard spCard) {
		return spCard->getId() == getId();
	}
	void set(string id);

	spTween playAnimation(bool show);
	spTween dropTo(Vector2 pos);
	void setCardScored();
private:
	spBox9Sprite createBackground(short zPriority);
	void createQuestionMark();
	void createMask();
	void createMaskedSprite();
	void createAnimalSprite(string cardId);

	spSprite _questionMark;
	spMaskedSprite _masked;
	spSprite _mask;
	spAnimalSlot _animalSlot;
};

#endif