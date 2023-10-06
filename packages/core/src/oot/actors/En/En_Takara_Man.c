#include <combo.h>
#include <combo/item.h>

#define SET_HANDLER(a, h) do { *(void**)(((char*)(a)) + 0x13c) = (h); } while (0)

void EnTakaraMan_InitHandler(Actor* this, GameState_Play* play)
{
    ActorFunc handler;
    u32 handlerAddr;

    if (gOotExtraFlags.chestGameKey)
        handlerAddr = 0x80b19558;
    else
        handlerAddr = 0x80b19370;
    handler = actorAddr(0x17c, handlerAddr);
    SET_HANDLER(this, handler);
    handler(this, play);
}

void EnTakaraMan_GiveItem(Actor* this, GameState_Play* play, s16 gi, float a, float b)
{
    comboGiveItemNpc(this, play, gi, NPC_OOT_CHEST_GAME_KEY, a, b);
}

int EnTakaraMan_HasGivenItem(Actor* this)
{
    if (!Actor_HasParent(this))
        return 0;

    gOotExtraFlags.chestGameKey = 1;
    return 1;
}
