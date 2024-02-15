#include <combo.h>
#include <combo/item.h>

void EnGe1_AfterCaught(void)
{
    s32 entrance;

    if (gPlay->sceneId == SCE_OOT_GERUDO_VALLEY)
        entrance = 0x1a5;
    else if (gSave.age == AGE_CHILD || gSave.inventory.items[ITS_OOT_HOOKSHOT] == ITEM_NONE)
        entrance = 0x129;
    else
        entrance = 0x5f8;

    gPlay->nextEntranceIndex = entrance;
}

int EnGe1_IsPeaceful(void)
{
    return gSave.inventory.quest.gerudoCard;
}

PATCH_FUNC(0x80a8f554, EnGe1_IsPeaceful);
PATCH_FUNC(0x80b23e68, EnGe1_IsPeaceful); /* En_Ge2 */

int EnGe1_HasGivenItem(Actor* this, GameState_Play* play)
{
    Actor_Player* link;

    if (gSave.highScores[0] >= 1500 && !BITMAP16_GET(gSave.eventsItem, EV_OOT_ITEM_HBA_1500) && !BITMAP16_GET(gSave.eventsMisc, EV_OOT_INF_HBA_1000))
    {
        /* Give two items */
        link = GET_LINK(play);
        if (link->state & PLAYER_ACTOR_STATE_GET_ITEM)
        {
            this->parent = NULL;
            BITMAP16_SET(gSave.eventsItem, EV_OOT_ITEM_HBA_1500);
            BITMAP16_SET(gSave.eventsMisc, EV_OOT_INF_HBA_1000);
            *(u16*)((char*)this + 0x29c) |= 2;
        }
    }

    return Actor_HasParent(this);
}

PATCH_CALL(0x80a90104, EnGe1_HasGivenItem);

void EnGe1_GiveItem(Actor* actor, GameState_Play* play, s16 gi, float a, float b)
{
    Actor_Player* link;
    int npc;

    link = GET_LINK(play);
    npc = -1;
    if (link->state & PLAYER_ACTOR_STATE_GET_ITEM)
        return;

    switch (gi)
    {
    case GI_OOT_HEART_PIECE:
        npc = NPC_OOT_GERUDO_ARCHERY_1;
        break;
    case GI_OOT_QUIVER2:
    case GI_OOT_QUIVER3:
        npc = NPC_OOT_GERUDO_ARCHERY_2;
        break;
    }

    comboGiveItemNpc(actor, play, gi, npc, a, b);
}

PATCH_CALL(0x80a901d0, EnGe1_GiveItem);
PATCH_CALL(0x80a90298, EnGe1_GiveItem);
