#include <combo.h>
#include <combo/item.h>

void EnKnight_OnDestroy(Actor* this, GameState_Play* play)
{
    Actor* knightMain;
    Actor* knightSide1;
    Actor* knightSide2;

    knightMain = *(Actor**)actorAddr(0x115, 0x809befd0);

    if (this != knightMain)
        return;

    if (Actor_HasParent(this) || gMmExtraFlags.songEmptiness)
    {
        gMmExtraFlags.songEmptiness = 1;
        knightSide1 = *(Actor**)actorAddr(0x115, 0x809befd4);
        knightSide2 = *(Actor**)actorAddr(0x115, 0x809befd8);
        ActorDestroy(knightMain);
        ActorDestroy(knightSide1);
        ActorDestroy(knightSide2);
        return;
    }
    comboGiveItemNpc(this, play, GI_MM_SONG_EMPTINESS, NPC_MM_SONG_EMPTINESS, 9999.f, 9999.f);
}

PATCH_FUNC(0x809b58d4, EnKnight_OnDestroy);
