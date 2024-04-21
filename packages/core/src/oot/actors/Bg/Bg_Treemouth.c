#include <combo.h>
#include <combo/config.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x15c) = (h); } while (0)

static void BgTreemouth_HandlerNull(Actor* this, GameState_Play* play)
{
}

static void BgTreemouth_SetHandler(Actor* this, void* unused)
{
    int isOpen;

    /* Never make the deku tree dynamic */
    SET_HANDLER(this, BgTreemouth_HandlerNull);

    if (gSave.age == AGE_CHILD && !Config_Flag(CFG_OOT_CLOSED_DEKU))
        isOpen = 1;
    else if (!Config_Flag(CFG_OOT_ADULT_DEKU) && (gSave.age == AGE_ADULT))
        isOpen = 0;
    else
        isOpen = GetEventChk(EV_OOT_CHK_DEKU_MIDO_SWORD_SHIELD);

    *(float*)((char*)this + 0x158) = isOpen ? 1.0f : 0.0f;
}

PATCH_FUNC(0x808d5360, BgTreemouth_SetHandler);
