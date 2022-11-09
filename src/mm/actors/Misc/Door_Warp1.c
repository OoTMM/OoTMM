#include <combo.h>

typedef struct
{
    s16 gi;
    s16 npc;
    s8  index;
}
BlueWarpData;

static const BlueWarpData kBlueWarpData[] = {
    { GI_MM_REMAINS_ODOLWA,   NPC_MM_REMAINS_ODOLWA,   0 },
    { GI_MM_REMAINS_GOHT,     NPC_MM_REMAINS_GOHT,     1 },
    { GI_MM_REMAINS_GYORG,    NPC_MM_REMAINS_GYORG,    2 },
    { GI_MM_REMAINS_TWINMOLD, NPC_MM_REMAINS_TWINMOLD, 3 },
};

static const BlueWarpData* DoorWarp1_GetData(Actor* this, GameState_Play* play)
{
    if ((this->variable & 0xff) != 0x01)
        return NULL;
    switch (play->sceneId)
    {
    case SCE_MM_LAIR_ODOLWA:
        return &kBlueWarpData[0];
    case SCE_MM_LAIR_GOHT:
        return &kBlueWarpData[1];
    case SCE_MM_LAIR_GYORG:
        return &kBlueWarpData[2];
    case SCE_MM_LAIR_TWINMOLD:
        return &kBlueWarpData[3];
    }
    return NULL;
}

int DoorWarp1_Collide(Actor* this, GameState_Play* play)
{
    float dist;

    dist = this->xzDistanceFromLink;
    if (dist < 0.f)
        dist = -dist;
    if (dist < 60.f)
    {
        dist = GET_LINK(play)->base.position.y - this->position.y;
        if (dist < 0.f)
            dist = -dist;
        if (dist < 20.f)
            return 1;
    }
    return 0;
}

int DoorWarp1_ShouldTrigger(Actor* this, GameState_Play* play)
{
    Actor_Player* link;
    const BlueWarpData* data;
    s16 gi;

    /* Check for collision */
    if (!DoorWarp1_Collide(this, play))
        return 0;

    /* Fetch extended blue warp data */
    data = DoorWarp1_GetData(this, play);
    if (data == NULL)
        return 1;

    /* Check if we are obtaining an item */
    link = GET_LINK(play);
    if (link->state & PLAYER_ACTOR_STATE_GET_ITEM)
        return 0;

    /* Check if we already have the item */
    if (gMmExtraBoss & (1 << data->index))
        return 1;

    if (Actor_HasParent(this))
    {
        if (!gMmExtraFlags2.songOath)
        {
            gMmExtraFlags2.songOath = 1;
            this->attachedA = NULL;
            return 0;
        }
        else
        {
            gMmExtraBoss |= (1 << data->index);
            return 1;
        }
    }

    /* Give the correct item */
    if (!gMmExtraFlags2.songOath)
        gi = comboOverride(OV_NPC, 0, NPC_MM_SONG_ORDER, GI_MM_SONG_ORDER);
    else
        gi = comboOverride(OV_NPC, 0, data->npc, data->gi);
    GiveItem(this, play, gi, 9999.f, 9999.f);
    return 0;
}

PATCH_FUNC(0x808b866c, DoorWarp1_ShouldTrigger);
