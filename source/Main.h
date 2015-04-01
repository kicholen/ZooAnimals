#include "Frame.h"
#include "Sprite.h"
#include "res/Resources.h"
#include "SoundPlayer.h"

using namespace oxygine;

void main_preinit();
void pause(Event *);
void resume(Event *);
void main_init();
void main_destroy();
void main_update();

extern SoundPlayer sPlayer;

DECLARE_SMART(LandingPageFrame, spLandingPageFrame);
DECLARE_SMART(BuyItemPopup, spBuyItemPopup);
DECLARE_SMART(MaskFrame, spMaskFrame);
DECLARE_SMART(SlideFrame, spSlideFrame);
DECLARE_SMART(WallPostMessagePopup, spWallPostMessagePopup);

extern spLandingPageFrame landingPageFrame;
extern spSlideFrame slideFrame;
extern spBuyItemPopup buyItemPopup;
extern spWallPostMessagePopup wallPostPopup;