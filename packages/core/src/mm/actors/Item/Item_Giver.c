#include <combo.h>
#include <combo/item.h>

static int ItemGiver_Common(Actor* this, GameState_Play* play, s16 gi)
{
    if (Actor_HasParent(this))
    {
        ActorDestroy(this);
        return 1;
    }

    comboGiveItemNpc(this, play, gi, this->variable, 10000.f, 5000.f);
    return 0;
}

static void ItemGiver_Init(Actor* this, GameState_Play* play)
{
}

static void ItemGiver_Update(Actor* this, GameState_Play* play)
{
    switch (this->variable)
    {
    case NPC_MM_SONG_EPONA:
        if (gMmExtraFlags.songEpona)
            ActorDestroy(this);
        else if (ItemGiver_Common(this, play, GI_MM_SONG_EPONA))
            gMmExtraFlags.songEpona = 1;
        break;
    case NPC_MM_MASK_TROUPE_LEADER:
        if (gMmExtraFlags2.maskTroupeLeader)
            ActorDestroy(this);
        else if (ItemGiver_Common(this, play, GI_MM_MASK_TROUPE_LEADER))
            gMmExtraFlags2.maskTroupeLeader = 1;
        break;
    case NPC_MM_SONG_HEALING:
        if (gMmExtraFlags2.songHealing)
            ActorDestroy(this);
        else if (ItemGiver_Common(this, play, GI_MM_SONG_HEALING))
            gMmExtraFlags2.songHealing = 1;
        break;
    default:
        ActorDestroy(this);
        break;
    }
}

ActorInit ItemGiver_gActorInit = {
    AC_ITEM_GIVER,
    0x6,
    0x10,
    0x1,
    sizeof(Actor),
    (ActorFunc)ItemGiver_Init,
    NULL,
    (ActorFunc)ItemGiver_Update,
    NULL,
};

void comboSpawnItemGiver(GameState_Play* play, u16 npcId)
{
    SpawnActor(
        (char*)play + 0x1ca0,
        play,
        AC_ITEM_GIVER,
        0, 0, 0,
        0, 0, 0,
        npcId
    );
}

void comboSpawnItemGivers(GameState_Play* play)
{
    /* Song of Healing */
    if (!gMmExtraFlags2.songHealing && gSave.entranceIndex == 0xd800)
    {
        comboSpawnItemGiver(play, NPC_MM_SONG_HEALING);
    }
}
