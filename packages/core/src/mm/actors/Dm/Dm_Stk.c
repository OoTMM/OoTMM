#include <combo.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x290) = (h); } while (0)

static void DmStk_Start(Actor* this, GameState_Play* play)
{
    if (comboSpecialCond(SPECIAL_MOON) && !comboConfig(CFG_MM_OPEN_MOON))
    {
        PlayerDisplayTextBox(play, 0x2013, this);
        comboTextHijackOathToOrder(play);
    }
    PlayMusic(0, 0x38, 0, 7, 0xff);
}

void DmStk_StartCutsceneWithOcarina(Actor* this, GameState_Play* play)
{
    DmStk_Start(this, play);
    SET_HANDLER(this, actorAddr(0x191, 0x80aa192c));
}

PATCH_FUNC(0x80aa18d8, DmStk_StartCutsceneWithOcarina);

void DmStk_StartCutsceneWithoutOcarina(Actor* this, GameState_Play* play)
{
    DmStk_Start(this, play);
    SET_HANDLER(this, actorAddr(0x191, 0x80aa19ec));
}

PATCH_FUNC(0x80aa1998, DmStk_StartCutsceneWithOcarina);
