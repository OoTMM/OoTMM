#include <combo.h>
#include <combo/item.h>
#include <combo/souls.h>

int comboAddStrayFairyMm(u16 dungeonId)
{
    if (dungeonId == 4)
    {
        MM_SET_EVENT_WEEK(EV_MM_WEEK_TOWN_FAIRY);
        return 0;
    }
    else
    {
        gMmSave.inventory.strayFairies[dungeonId]++;
        return gMmSave.inventory.strayFairies[dungeonId];
    }
}

void comboAddMagicUpgradeMm(int level)
{
    gMmSave.playerData.magicAcquired = 1;
    if (level >= 2)
        gMmSave.playerData.doubleMagic = 1;
}

static void refillMagic(int level)
{
    gMmSave.playerData.magicLevel = level;
    gMmSave.playerData.magicAmount = level * 0x30;
}

void comboAddItemSharedMm(s16 gi, int noEffect)
{
    if (comboConfig(CFG_SHARED_MAGIC))
    {
        switch (gi)
        {
        case GI_MM_MAGIC_UPGRADE:
            comboAddMagicUpgradeOot(1);
            break;
        case GI_MM_MAGIC_UPGRADE2:
            comboAddMagicUpgradeOot(2);
            break;
        }
    }

    if (comboConfig(CFG_SHARED_SOULS_ENEMY))
    {
        switch (gi)
        {
        case GI_MM_SOUL_ENEMY_OCTOROK:
            comboAddSoulOot(GI_OOT_SOUL_ENEMY_OCTOROK);
            break;
        case GI_MM_SOUL_ENEMY_WALLMASTER:
            comboAddSoulOot(GI_OOT_SOUL_ENEMY_WALLMASTER);
            break;
        case GI_MM_SOUL_ENEMY_DODONGO:
            comboAddSoulOot(GI_OOT_SOUL_ENEMY_DODONGO);
            break;
        case GI_MM_SOUL_ENEMY_KEESE:
            comboAddSoulOot(GI_OOT_SOUL_ENEMY_KEESE);
            break;
        case GI_MM_SOUL_ENEMY_TEKTITE:
            comboAddSoulOot(GI_OOT_SOUL_ENEMY_TEKTITE);
            break;
        case GI_MM_SOUL_ENEMY_PEAHAT:
            comboAddSoulOot(GI_OOT_SOUL_ENEMY_PEAHAT);
            break;
        case GI_MM_SOUL_ENEMY_LIZALFOS_DINALFOS:
            comboAddSoulOot(GI_OOT_SOUL_ENEMY_LIZALFOS_DINALFOS);
            break;
        case GI_MM_SOUL_ENEMY_SKULLTULA:
            comboAddSoulOot(GI_OOT_SOUL_ENEMY_SKULLTULA);
            break;
        case GI_MM_SOUL_ENEMY_ARMOS:
            comboAddSoulOot(GI_OOT_SOUL_ENEMY_ARMOS);
            break;
        case GI_MM_SOUL_ENEMY_DEKU_BABA:
            comboAddSoulOot(GI_OOT_SOUL_ENEMY_DEKU_BABA);
            break;
        case GI_MM_SOUL_ENEMY_DEKU_SCRUB:
            comboAddSoulOot(GI_OOT_SOUL_ENEMY_DEKU_SCRUB);
            break;
        case GI_MM_SOUL_ENEMY_BUBBLE:
            comboAddSoulOot(GI_OOT_SOUL_ENEMY_BUBBLE);
            break;
        case GI_MM_SOUL_ENEMY_BEAMOS:
            comboAddSoulOot(GI_OOT_SOUL_ENEMY_BEAMOS);
            break;
        case GI_MM_SOUL_ENEMY_REDEAD_GIBDO:
            comboAddSoulOot(GI_OOT_SOUL_ENEMY_REDEAD_GIBDO);
            break;
        case GI_MM_SOUL_ENEMY_SKULLWALLTULA:
            comboAddSoulOot(GI_OOT_SOUL_ENEMY_SKULLWALLTULA);
            break;
        case GI_MM_SOUL_ENEMY_SHELL_BLADE:
            comboAddSoulOot(GI_OOT_SOUL_ENEMY_SHELL_BLADE);
            break;
        case GI_MM_SOUL_ENEMY_LIKE_LIKE:
            comboAddSoulOot(GI_OOT_SOUL_ENEMY_LIKE_LIKE);
            break;
        case GI_MM_SOUL_ENEMY_IRON_KNUCKLE:
            comboAddSoulOot(GI_OOT_SOUL_ENEMY_IRON_KNUCKLE);
            break;
        case GI_MM_SOUL_ENEMY_FREEZARD:
            comboAddSoulOot(GI_OOT_SOUL_ENEMY_FREEZARD);
            break;
        case GI_MM_SOUL_ENEMY_WOLFOS:
            comboAddSoulOot(GI_OOT_SOUL_ENEMY_WOLFOS);
            break;
        case GI_MM_SOUL_ENEMY_GUAY:
            comboAddSoulOot(GI_OOT_SOUL_ENEMY_GUAY);
            break;
        case GI_MM_SOUL_ENEMY_FLYING_POT:
            comboAddSoulOot(GI_OOT_SOUL_ENEMY_FLYING_POT);
            break;
        case GI_MM_SOUL_ENEMY_FLOORMASTER:
            comboAddSoulOot(GI_OOT_SOUL_ENEMY_FLOORMASTER);
            break;
        case GI_MM_SOUL_ENEMY_LEEVER:
            comboAddSoulOot(GI_OOT_SOUL_ENEMY_LEEVER);
            break;
        case GI_MM_SOUL_ENEMY_STALCHILD:
            comboAddSoulOot(GI_OOT_SOUL_ENEMY_STALCHILD);
            break;
        }
    }

    if (comboConfig(CFG_SHARED_OCARINA_BUTTONS))
    {
        switch (gi)
        {
        case GI_MM_BUTTON_A:
            gSharedCustomSave.ocarinaButtonMaskOot |= A_BUTTON;
            break;
        case GI_MM_BUTTON_C_RIGHT:
            gSharedCustomSave.ocarinaButtonMaskOot |= R_CBUTTONS;
            break;
        case GI_MM_BUTTON_C_LEFT:
            gSharedCustomSave.ocarinaButtonMaskOot |= L_CBUTTONS;
            break;
        case GI_MM_BUTTON_C_UP:
            gSharedCustomSave.ocarinaButtonMaskOot |= U_CBUTTONS;
            break;
        case GI_MM_BUTTON_C_DOWN:
            gSharedCustomSave.ocarinaButtonMaskOot |= D_CBUTTONS;
            break;
        }
    }
}

int comboAddItemMm(s16 gi, int noEffect)
{
    int count;

    count = 0;
    switch (gi)
    {
    case GI_MM_POWDER_KEG:
        gMmSave.inventory.items[ITS_MM_KEG] = ITEM_MM_POWDER_KEG;
        gMmSave.inventory.ammo[ITS_MM_KEG] = 1;
        break;
    case GI_MM_MAGIC_UPGRADE:
        comboAddMagicUpgradeMm(1);
        if (noEffect)
            refillMagic(1);
        break;
    case GI_MM_MAGIC_UPGRADE2:
        comboAddMagicUpgradeMm(2);
        if (noEffect)
            refillMagic(2);
        break;
    case GI_MM_GS_TOKEN_SWAMP:
        count = ++gMmSave.skullCountSwamp;
        break;
    case GI_MM_GS_TOKEN_OCEAN:
        count = ++gMmSave.skullCountOcean;
        break;
    case GI_MM_STRAY_FAIRY_WF:
        count = comboAddStrayFairyMm(0);
        break;
    case GI_MM_STRAY_FAIRY_SH:
        count = comboAddStrayFairyMm(1);
        break;
    case GI_MM_STRAY_FAIRY_GB:
        count = comboAddStrayFairyMm(2);
        break;
    case GI_MM_STRAY_FAIRY_ST:
        count = comboAddStrayFairyMm(3);
        break;
    case GI_MM_STRAY_FAIRY_TOWN:
        count = comboAddStrayFairyMm(4);
        break;
    case GI_MM_SPIN_UPGRADE:
        MM_SET_EVENT_WEEK(EV_MM_WEEK_SPIN_UPGRADE);
        break;
    case GI_MM_WORLD_MAP_CLOCK_TOWN:
        Inventory_SetWorldMapCloudVisibility(TINGLE_MAP_CLOCK_TOWN);
        break;
    case GI_MM_WORLD_MAP_WOODFALL:
        Inventory_SetWorldMapCloudVisibility(TINGLE_MAP_WOODFALL);
        break;
    case GI_MM_WORLD_MAP_SNOWHEAD:
        Inventory_SetWorldMapCloudVisibility(TINGLE_MAP_SNOWHEAD);
        break;
    case GI_MM_WORLD_MAP_ROMANI_RANCH:
        Inventory_SetWorldMapCloudVisibility(TINGLE_MAP_ROMANI_RANCH);
        break;
    case GI_MM_WORLD_MAP_GREAT_BAY:
        Inventory_SetWorldMapCloudVisibility(TINGLE_MAP_GREAT_BAY);
        break;
    case GI_MM_WORLD_MAP_STONE_TOWER:
        Inventory_SetWorldMapCloudVisibility(TINGLE_MAP_STONE_TOWER);
        break;
    case GI_MM_OWL_GREAT_BAY:
        gMmOwlFlags |= (1 << 0);
        break;
    case GI_MM_OWL_ZORA_CAPE:
        gMmOwlFlags |= (1 << 1);
        break;
    case GI_MM_OWL_SNOWHEAD:
        gMmOwlFlags |= (1 << 2);
        break;
    case GI_MM_OWL_MOUNTAIN_VILLAGE:
        gMmOwlFlags |= (1 << 3);
        break;
    case GI_MM_OWL_CLOCK_TOWN:
        gMmOwlFlags |= (1 << 4);
        break;
    case GI_MM_OWL_MILK_ROAD:
        gMmOwlFlags |= (1 << 5);
        break;
    case GI_MM_OWL_WOODFALL:
        gMmOwlFlags |= (1 << 6);
        break;
    case GI_MM_OWL_SOUTHERN_SWAMP:
        gMmOwlFlags |= (1 << 7);
        break;
    case GI_MM_OWL_IKANA_CANYON:
        gMmOwlFlags |= (1 << 8);
        break;
    case GI_MM_OWL_STONE_TOWER:
        gMmOwlFlags |= (1 << 9);
        break;
    case GI_MM_SOUL_ENEMY_OCTOROK:
    case GI_MM_SOUL_ENEMY_WALLMASTER:
    case GI_MM_SOUL_ENEMY_DODONGO:
    case GI_MM_SOUL_ENEMY_KEESE:
    case GI_MM_SOUL_ENEMY_TEKTITE:
    case GI_MM_SOUL_ENEMY_PEAHAT:
    case GI_MM_SOUL_ENEMY_LIZALFOS_DINALFOS:
    case GI_MM_SOUL_ENEMY_SKULLTULA:
    case GI_MM_SOUL_ENEMY_ARMOS:
    case GI_MM_SOUL_ENEMY_DEKU_BABA:
    case GI_MM_SOUL_ENEMY_DEKU_SCRUB:
    case GI_MM_SOUL_ENEMY_BUBBLE:
    case GI_MM_SOUL_ENEMY_BEAMOS:
    case GI_MM_SOUL_ENEMY_REDEAD_GIBDO:
    case GI_MM_SOUL_ENEMY_SKULLWALLTULA:
    case GI_MM_SOUL_ENEMY_SHELL_BLADE:
    case GI_MM_SOUL_ENEMY_LIKE_LIKE:
    case GI_MM_SOUL_ENEMY_IRON_KNUCKLE:
    case GI_MM_SOUL_ENEMY_FREEZARD:
    case GI_MM_SOUL_ENEMY_WOLFOS:
    case GI_MM_SOUL_ENEMY_GUAY:
    case GI_MM_SOUL_ENEMY_FLYING_POT:
    case GI_MM_SOUL_ENEMY_FLOORMASTER:
    case GI_MM_SOUL_ENEMY_CHUCHU:
    case GI_MM_SOUL_ENEMY_DEEP_PYTHON:
    case GI_MM_SOUL_ENEMY_SKULLFISH:
    case GI_MM_SOUL_ENEMY_DEXIHAND:
    case GI_MM_SOUL_ENEMY_DRAGONFLY:
    case GI_MM_SOUL_ENEMY_EENO:
    case GI_MM_SOUL_ENEMY_EYEGORE:
    case GI_MM_SOUL_ENEMY_HIPLOOP:
    case GI_MM_SOUL_ENEMY_REAL_BOMBCHU:
    case GI_MM_SOUL_ENEMY_TAKKURI:
    case GI_MM_SOUL_ENEMY_BOE:
    case GI_MM_SOUL_ENEMY_NEJIRON:
    case GI_MM_SOUL_ENEMY_BIO_BABA:
    case GI_MM_SOUL_ENEMY_GARO:
    case GI_MM_SOUL_ENEMY_WIZZROBE:
    case GI_MM_SOUL_ENEMY_GOMESS:
    case GI_MM_SOUL_ENEMY_GEKKO:
    case GI_MM_SOUL_ENEMY_BAD_BAT:
    case GI_MM_SOUL_ENEMY_SNAPPER:
    case GI_MM_SOUL_ENEMY_WART:
    case GI_MM_SOUL_ENEMY_CAPTAIN_KEETA:
    case GI_MM_SOUL_ENEMY_STALCHILD:
    case GI_MM_SOUL_ENEMY_LEEVER:
    case GI_MM_SOUL_BOSS_ODOLWA:
    case GI_MM_SOUL_BOSS_GOHT:
    case GI_MM_SOUL_BOSS_GYORG:
    case GI_MM_SOUL_BOSS_TWINMOLD:
    case GI_MM_SOUL_BOSS_IGOS:
        comboAddSoulMm(gi);
        break;
    case GI_MM_BUTTON_A:
        gSharedCustomSave.ocarinaButtonMaskMm |= A_BUTTON;
        break;
    case GI_MM_BUTTON_C_RIGHT:
        gSharedCustomSave.ocarinaButtonMaskMm |= R_CBUTTONS;
        break;
    case GI_MM_BUTTON_C_LEFT:
        gSharedCustomSave.ocarinaButtonMaskMm |= L_CBUTTONS;
        break;
    case GI_MM_BUTTON_C_UP:
        gSharedCustomSave.ocarinaButtonMaskMm |= U_CBUTTONS;
        break;
    case GI_MM_BUTTON_C_DOWN:
        gSharedCustomSave.ocarinaButtonMaskMm |= D_CBUTTONS;
        break;
    }

    return count;
}
