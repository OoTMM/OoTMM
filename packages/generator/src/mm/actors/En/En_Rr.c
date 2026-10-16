#include <combo.h>
#include <combo/item.h>
#include <combo/inventory.h>
#include <combo/player.h>

#include "combo/draw.h"

static u8 EnRr_DeleteShield(PlayState* play, s16 equipment)
{
    MmShieldId shield;
    if (equipment != 1 || gMmSave.info.itemEquips.shield != 1)
        return 0;

    shield = MmShield_GetEquipped();
    if (shield != MM_SHIELD_DEKU &&
        shield != MM_SHIELD_HERO &&
        shield != MM_SHIELD_HYLIAN)
        return 0;

    if (!MmShield_IsOwned(shield))
        return 0;

    MmShield_Lose(play, shield);
    return (u8)shield;
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
    const Vec3f* pos,
    int params)
{
    const u8* likeLike = (const u8*)pos - 0x24;
    u8 stolen = likeLike[0x1e2];

    Actor* item;
    s16 gi;

    switch (stolen)
    {
    case MM_SHIELD_DEKU:
        gi = GI_MM_SHIELD_DEKU;
        break;

    case MM_SHIELD_HYLIAN:
        gi = GI_MM_SHIELD_HYLIAN;
        break;

    case MM_SHIELD_HERO:
    default:
        gi = GI_MM_SHIELD_HERO;
        break;
    }

    item = Item_DropCollectible(play, pos, params);

    if (item && item->id == ACTOR_EN_ITEM00)
    {
        Actor_EnItem00* drop = (Actor_EnItem00*)item;

        if (!drop->isExtended)
        {
            drop->xflagGi = gi;
            if (stolen == MM_SHIELD_HYLIAN)
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

PATCH_CALL(0x808fa5ac, EnRr_DeleteShield);
PATCH_CALL(0x808fb554, EnRr_DropShield);
