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

void Horse_ForceUnmount(GameState_Play* play)
{
    if (AREG(6) != 0)
    {
        /* Link is on Epona, needs to dismount */
        AREG(6) = 0;

        /* Reset the TempB */
        if (EV_OOT_IS_SWORDLESS())
            gSave.equips.buttonItems[0] = ITEM_NONE;
        else
            gSave.equips.buttonItems[0] = gSaveContext.buttonStatus[0];
        gSaveContext.buttonStatus[0] = ITEM_NONE;

        /* Reload the B button icon */
        Interface_LoadItemIconImpl(play, 0);
    }
}

void Horse_SpawnWrapper(GameState_Play* play, Actor_Player* link)
{
    if (!Horse_IsValidEntrance(gSave.entrance))
        Horse_ForceUnmount(play);
    Horse_Spawn(play, link);
}

PATCH_CALL(0x800218b0, Horse_SpawnWrapper);
