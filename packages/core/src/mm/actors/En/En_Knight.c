#include <combo.h>
#include <combo/item.h>
#include <combo/actor.h>

void EnKnight_OnDestroy(Actor* this, GameState_Play* play)
{
    Actor* knightMain;
    Actor* knightSide1;
    Actor* knightSide2;

    knightMain = *(Actor**)actorAddr(0x115, 0x809befd0);

    if (this != knightMain)
        return;

    if (Actor_HasParentZ(this) || gMmExtraFlags.songEmptiness)
    {
        gMmExtraFlags.songEmptiness = 1;
        knightSide1 = *(Actor**)actorAddr(0x115, 0x809befd4);
        knightSide2 = *(Actor**)actorAddr(0x115, 0x809befd8);
        Actor_Kill(knightMain);
        Actor_Kill(knightSide1);
        Actor_Kill(knightSide2);
        return;
    }
    comboGiveItemNpc(this, play, GI_MM_SONG_EMPTINESS, NPC_MM_SONG_EMPTINESS, 9999.f, 9999.f);
}

PATCH_FUNC(0x809b58d4, EnKnight_OnDestroy);
