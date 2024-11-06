#include <combo.h>
#include <combo/entrance.h>

static int Horse_IsValidEntrance(u32 entranceId)
{
    switch (entranceId)
    {
    case ENTR_OOT_FIELD_FROM_GERUDO_VALLEY:
    case ENTR_OOT_GERUDO_VALLEY_FROM_FIELD:
    case ENTR_OOT_FIELD_FROM_LON_LON_RANCH:
    case ENTR_OOT_LON_LON_RANCH_FROM_FIELD:
    case ENTR_OOT_FIELD_FROM_LAKE_HYLIA:
    case ENTR_OOT_LAKE_HYLIA_FROM_FIELD:
    case ENTR_OOT_VALLEY_FROM_GERUDO_FORTRESS:
    case ENTR_OOT_GERUDO_FORTRESS_FROM_VALLEY:
        return 1;
    default:
        return 0;
    }
}

void ReloadSword(PlayState* play)
{
    static const u8 kSwords[] = { ITEM_NONE, ITEM_OOT_SWORD_KOKIRI, ITEM_OOT_SWORD_MASTER, ITEM_OOT_SWORD_KNIFE_BIGGORON };

    u8 itemId;

    /* Reload the sword */
    itemId = kSwords[gSave.info.equips.equipment.swords];
    if (itemId == ITEM_NONE)
        EV_OOT_SET_SWORDLESS();
    else
        EV_OOT_UNSET_SWORDLESS();
    if (itemId == ITEM_OOT_SWORD_KNIFE_BIGGORON && !gSave.info.isBiggoronSword && !gSave.info.playerData.swordHealth)
        itemId = ITEM_OOT_SWORD_KNIFE_BROKEN;
    gSave.info.equips.buttonItems[0] = itemId;
    Interface_LoadItemIconImpl(play, 0);
}

void Horse_ForceUnmount(PlayState* play)
{
    if (AREG(6) != 0)
    {
        /* Link is on Epona, needs to dismount */
        AREG(6) = 0;

        /* Reload the sword */
        ReloadSword(play);
    }
}

void Horse_SpawnWrapper(PlayState* play, Player* link)
{
    if (!Horse_IsValidEntrance(gSave.entrance))
        Horse_ForceUnmount(play);
    Horse_Spawn(play, link);
}

PATCH_CALL(0x800218b0, Horse_SpawnWrapper);
