#include <combo.h>
#include <combo/item.h>
#include <combo/inventory.h>
#include <combo/player.h>
#include <combo/oot/actor.h>

#include "combo/draw.h"

#define ENRR_SHIELD_RECORDS 32

typedef struct {
    Actor* owner;
    MmShieldId shield;
} EnRrShieldRecord;

static EnRrShieldRecord sShieldRecords[ENRR_SHIELD_RECORDS];
static u8 sNextShieldRecord;

static void EnRr_RecordShield(Actor* owner, MmShieldId shield)
{
    s32 i;

    if (!owner || owner->id != ACTOR_EN_RR)
        return;

    for (i = 0; i < ENRR_SHIELD_RECORDS; ++i) {
        if (sShieldRecords[i].owner == owner) {
            sShieldRecords[i].shield = shield;
            return;
        }
    }

    sShieldRecords[sNextShieldRecord].owner = owner;
    sShieldRecords[sNextShieldRecord].shield = shield;
    sNextShieldRecord = (sNextShieldRecord + 1) % ENRR_SHIELD_RECORDS;
}

static Actor* EnRr_FindOwner(PlayState* play, const Vec3f* dropPos)
{
    Actor* actor;

    for (actor = play->actorCtx.actors[ACTORCAT_ENEMY].first;
         actor;
         actor = actor->next)
    {
        if (actor->id == ACTOR_EN_RR &&
            actor->world.pos.x == dropPos->x &&
            actor->world.pos.y == dropPos->y &&
            actor->world.pos.z == dropPos->z)
            return actor;
    }
    return NULL;
}

static MmShieldId EnRr_RecordedShield(Actor* owner)
{
    s32 i;
    for (i = 0; i < ENRR_SHIELD_RECORDS; ++i) {
        if (sShieldRecords[i].owner == owner)
            return sShieldRecords[i].shield;
    }
    return MM_SHIELD_NONE;
}

static u8 EnRr_DeleteShield(PlayState* play, s16 equipment)
{
    Player* player;
    Actor* owner;
    MmShieldId shield;
    u8 nativeShield;
    if (equipment != 1)
        return 0;

    nativeShield = gOotSave.info.equips.equipment.shields;
    switch (nativeShield) {
    case 1:
        switch (OotChildShield_GetEquippedVariant()) {
        case OOT_CHILD_SHIELD_DEKU:
            shield = MM_SHIELD_DEKU;
            break;
        case OOT_CHILD_SHIELD_HERO:
            shield = MM_SHIELD_HERO;
            break;
        default:
            return 0;
        }
        break;
    case 2:
        shield = MM_SHIELD_HYLIAN;
        break;
    default:
        return 0;
    }

    if (!OotShield_IsOwned(shield))
        return 0;

    player = GET_PLAYER(play);
    owner = player ? player->actor.parent : NULL;
    if (!owner || owner->id != ACTOR_EN_RR)
        owner = player ? Actor_FindNearby(play, &player->actor, ACTOR_EN_RR, ACTORCAT_ENEMY, 160.f) : NULL;

    EnRr_RecordShield(owner, shield);
    OotShield_Lose(play, shield);
    return nativeShield;
}

static ActorCallback sNormalShieldDropUpdate;

static void EnRr_UpdateShieldDrop(Actor* actor, PlayState* play)
{
    Actor_EnItem00* item = (Actor_EnItem00*)actor;
    ComboItemQuery q = ITEM_QUERY_INIT;
    f32 originalDist;
    if (Actor_HasParent(actor, play))
    {
        Flags_SetCollectible(play, item->collectibleFlag);
        Actor_Kill(actor);
        return;
    }
    if (sNormalShieldDropUpdate)
    {
        originalDist = actor->xzDistToPlayer;
        actor->xzDistToPlayer = 1000.f;
        sNormalShieldDropUpdate(actor, play);
        actor->xzDistToPlayer = originalDist;
    }
    if (actor->update == NULL)
        return;
    if (item->xflagGi <= 0 || item->unk_144 > 0 || actor->xzDistToPlayer > 30.f || actor->yDistanceFromLink < -50.f || actor->yDistanceFromLink > 50.f)
        return;

    if (!Item_SafeToReceive(play))
        return;
    q.gi = item->xflagGi;
    comboGiveItem(actor, play, &q, 50.f, 80.f);
}



static void EnRr_DrawShieldDrop(Actor* actor, PlayState* play)
{
    Actor_EnItem00* item = (Actor_EnItem00*)actor;

    if (item->blinkTimer & item->blinkMask)
        return;
    Draw_Gi(play, NULL, item->xflagGi, DRAW_RAW);
}


static Actor* EnRr_DropShield(
    PlayState* play,
    const Vec3f* dropPos,
    int params)
{
    Actor* item;
    Actor* owner;
    MmShieldId variant;
    s16 gi;

    owner = EnRr_FindOwner(play, dropPos);
    variant = EnRr_RecordedShield(owner);

    if (params == ITEM00_SHIELD_HYLIAN)
        gi = GI_OOT_SHIELD_HYLIAN;
    else if (variant == MM_SHIELD_HERO)
        gi = GI_OOT_SHIELD_HERO;
    else
        gi = GI_OOT_SHIELD_DEKU;

    item = Item_DropCollectible(play, dropPos, params);

    if (item && item->id == ACTOR_EN_ITEM00)
    {
        Actor_EnItem00* drop = (Actor_EnItem00*)item;

        if (!drop->isExtended)
        {
            drop->xflagGi = gi;
            if (params == ITEM00_SHIELD_DEKU)
            {
                if (!sNormalShieldDropUpdate)
                    sNormalShieldDropUpdate = item->update;

                item->update = EnRr_UpdateShieldDrop;
                item->draw = EnRr_DrawShieldDrop;
            }
        }
    }

    return item;
}

PATCH_CALL(0x809e7220, EnRr_DeleteShield);
PATCH_CALL(0x809e820c, EnRr_DropShield);
PATCH_CALL(0x809e8224, EnRr_DropShield);
