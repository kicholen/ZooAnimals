#include "DraggableSprite.h"


DraggableSprite::DraggableSprite() {
	drag.init(this);
}

void DraggableSprite::setDragBounds(Vector2 position, Vector2 size) {
	drag.setDragBounds(RectF(position, size));
}

void DraggableSprite::setDragEnabled(bool shouldEnable) {
	drag.setDragEnabled(shouldEnable);
}