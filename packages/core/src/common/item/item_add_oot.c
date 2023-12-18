#include <combo.h>
#include <combo/item.h>
#include <combo/sr.h>
#include <combo/souls.h>

int comboAddSmallKeyOot(u16 dungeonId)
{
    s8 keyCount;

    /* Check for max keys */
    if ((dungeonId != SCE_OOT_TREASURE_SHOP || comboConfig(CFG_OOT_CHEST_GAME_SHUFFLE)) && gOotSave.inventory.dungeonItems[dungeonId].maxKeys >= g.maxKeysOot[dungeonId])
        return 0;

    keyCount = gOotSave.inventory.dungeonKeys[dungeonId];
    if (keyCount < 0)
        keyCount = 1;
    else
        keyCount++;
    gOotSave.inventory.dungeonKeys[dungeonId] = keyCount;
    if (dungeonId == SCE_OOT_TREASURE_SHOP && !comboConfig(CFG_OOT_CHEST_GAME_SHUFFLE))
        return 0;
    else
        return ++gOotSave.inventory.dungeonItems[dungeonId].maxKeys;
}

void comboAddKeyRingOot(u16 dungeonId)
{
    for (int i = 0; i < g.maxKeysOot[dungeonId]; ++i)
        comboAddSmallKeyOot(dungeonId);
}

void comboAddSkeletonKeyOot(void)
{
    for (int i = 0; i < ARRAY_SIZE(g.maxKeysOot); ++i)
        comboAddKeyRingOot(i);
}

void comboAddBossKeyOot(u16 dungeonId)
{
    gOotSave.inventory.dungeonItems[dungeonId].bossKey = 1;
}

void comboAddCompassOot(u16 dungeonId)
{
    gOotSave.inventory.dungeonItems[dungeonId].compass = 1;
}

void comboAddMapOot(u16 dungeonId)
{
    gOotSave.inventory.dungeonItems[dungeonId].map = 1;
}

static void addHealth(u8 count)
{
    u16 health;

    health = (u16)count * 0x10;
    gOotSave.playerData.health += health;
    if (gOotSave.playerData.health > gOotSave.playerData.healthMax)
        gOotSave.playerData.health = gOotSave.playerData.healthMax;
}

void comboAddMagicUpgradeOot(int level)
{
    gOotSave.playerData.magicUpgrade = 1;
    if (level >= 2)
        gOotSave.playerData.magicUpgrade2 = 1;
}

static void refillMagic(int level)
{
    gOotSave.playerData.magicSize = level;
    gOotSave.playerData.magicAmount = level * 0x30;
}

static void addRupees(u16 count)
{
    u16 max;

    max = gOotMaxRupees[gOotSave.inventory.upgrades.wallet];
    gOotSave.playerData.rupees += count;
    if (gOotSave.playerData.rupees > max)
        gOotSave.playerData.rupees = max;
}

static int addSilverRupees(int id, int noEffect)
{
    if (noEffect)
        addRupees(5);
    return comboSilverRupeesIncCount(id, noEffect);
}

static void addSilverRupeePouch(int id, int noEffect)
{
    int max;

    max = gSilverRupeeData[id].count;
    for (int i = 0; i < max; ++i)
        addSilverRupees(id, noEffect);
}

static void addMagicalRupee(int noEffect)
{
    for (int i = 0; i < ARRAY_SIZE(gSilverRupeeData); ++i)
        addSilverRupeePouch(i, noEffect);
}

void comboAddCommonItemOot(int sid, int noEffect)
{
    switch (sid)
    {
    case SITEM_HEART_PIECE:
        gOotSave.inventory.quest.heartPieces++;
        if (noEffect)
        {
            if (gOotSave.inventory.quest.heartPieces >= 4)
            {
                gOotSave.inventory.quest.heartPieces -= 4;
                gOotSave.playerData.healthMax += 0x10;
            }
            addHealth(20);
        }
        break;
    case SITEM_HEART_CONTAINER:
        gOotSave.playerData.healthMax += 0x10;
        if (noEffect)
            addHealth(20);
        break;
    }
}

void comboAddItemSharedOot(s16 gi, int noEffect)
{
    if (comboConfig(CFG_SHARED_MAGIC))
    {
        switch (gi)
        {
        case GI_OOT_MAGIC_UPGRADE:
            comboAddMagicUpgradeMm(1);
            break;
        case GI_OOT_MAGIC_UPGRADE2:
            comboAddMagicUpgradeMm(2);
            break;
        }
    }

    if (comboConfig(CFG_SHARED_HEALTH))
    {
        switch (gi)
        {
        case GI_OOT_HEART_CONTAINER:
        case GI_OOT_HEART_CONTAINER2:
            comboAddCommonItemMm(SITEM_HEART_CONTAINER, noEffect);
            break;
        case GI_OOT_HEART_PIECE:
        case GI_OOT_TC_HEART_PIECE:
            comboAddCommonItemMm(SITEM_HEART_PIECE, noEffect);
            break;
        }
    }

    if (comboConfig(CFG_SHARED_SOULS_ENEMY))
    {
        switch (gi)
        {
        case GI_OOT_SOUL_ENEMY_OCTOROK:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_OCTOROK);
            break;
        case GI_OOT_SOUL_ENEMY_WALLMASTER:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_WALLMASTER);
            break;
        case GI_OOT_SOUL_ENEMY_DODONGO:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_DODONGO);
            break;
        case GI_OOT_SOUL_ENEMY_KEESE:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_KEESE);
            break;
        case GI_OOT_SOUL_ENEMY_TEKTITE:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_TEKTITE);
            break;
        case GI_OOT_SOUL_ENEMY_PEAHAT:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_PEAHAT);
            break;
        case GI_OOT_SOUL_ENEMY_LIZALFOS_DINALFOS:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_LIZALFOS_DINALFOS);
            break;
        case GI_OOT_SOUL_ENEMY_SKULLTULA:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_SKULLTULA);
            break;
        case GI_OOT_SOUL_ENEMY_ARMOS:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_ARMOS);
            break;
        case GI_OOT_SOUL_ENEMY_DEKU_BABA:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_DEKU_BABA);
            break;
        case GI_OOT_SOUL_ENEMY_DEKU_SCRUB:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_DEKU_SCRUB);
            break;
        case GI_OOT_SOUL_ENEMY_BUBBLE:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_BUBBLE);
            break;
        case GI_OOT_SOUL_ENEMY_BEAMOS:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_BEAMOS);
            break;
        case GI_OOT_SOUL_ENEMY_REDEAD_GIBDO:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_REDEAD_GIBDO);
            break;
        case GI_OOT_SOUL_ENEMY_SKULLWALLTULA:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_SKULLWALLTULA);
            break;
        case GI_OOT_SOUL_ENEMY_SHELL_BLADE:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_SHELL_BLADE);
            break;
        case GI_OOT_SOUL_ENEMY_LIKE_LIKE:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_LIKE_LIKE);
            break;
        case GI_OOT_SOUL_ENEMY_IRON_KNUCKLE:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_IRON_KNUCKLE);
            break;
        case GI_OOT_SOUL_ENEMY_FREEZARD:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_FREEZARD);
            break;
        case GI_OOT_SOUL_ENEMY_WOLFOS:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_WOLFOS);
            break;
        case GI_OOT_SOUL_ENEMY_GUAY:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_GUAY);
            break;
        case GI_OOT_SOUL_ENEMY_FLYING_POT:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_FLYING_POT);
            break;
        case GI_OOT_SOUL_ENEMY_FLOORMASTER:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_FLOORMASTER);
            break;
        case GI_OOT_SOUL_ENEMY_LEEVER:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_LEEVER);
            break;
        case GI_OOT_SOUL_ENEMY_STALCHILD:
            comboAddSoulMm(GI_MM_SOUL_ENEMY_STALCHILD);
            break;
        }
    }

    if (comboConfig(CFG_SHARED_SKELETON_KEY) && gi == GI_OOT_SKELETON_KEY)
        comboAddSkeletonKeyMm();

    if (comboConfig(CFG_SHARED_OCARINA_BUTTONS))
    {
        switch (gi)
        {
        case GI_OOT_BUTTON_A:
            gSharedCustomSave.ocarinaButtonMaskMm |= A_BUTTON;
            break;
        case GI_OOT_BUTTON_C_RIGHT:
            gSharedCustomSave.ocarinaButtonMaskMm |= R_CBUTTONS;
            break;
        case GI_OOT_BUTTON_C_LEFT:
            gSharedCustomSave.ocarinaButtonMaskMm |= L_CBUTTONS;
            break;
        case GI_OOT_BUTTON_C_UP:
            gSharedCustomSave.ocarinaButtonMaskMm |= U_CBUTTONS;
            break;
        case GI_OOT_BUTTON_C_DOWN:
            gSharedCustomSave.ocarinaButtonMaskMm |= D_CBUTTONS;
            break;
        }
    }
}

int comboAddItemOot(s16 gi, int noEffect)
{
    int count;
    u16 dungeonId;

    count = 0;
    (void)dungeonId;
    switch (gi)
    {
    case GI_OOT_MAGIC_UPGRADE:
        comboAddMagicUpgradeOot(1);
        if (noEffect)
            refillMagic(1);
        break;
    case GI_OOT_MAGIC_UPGRADE2:
        comboAddMagicUpgradeOot(2);
        if (noEffect)
            refillMagic(2);
        break;
    case GI_OOT_HEART_PIECE:
    case GI_OOT_TC_HEART_PIECE:
        comboAddCommonItemOot(SITEM_HEART_PIECE, noEffect);
        break;
    case GI_OOT_HEART_CONTAINER:
    case GI_OOT_HEART_CONTAINER2:
        comboAddCommonItemOot(SITEM_HEART_CONTAINER, noEffect);
        break;
    case GI_OOT_GS_TOKEN:
        gOotSave.inventory.quest.goldToken = 1;
        count = ++gOotSave.inventory.goldTokens;
        break;
    case GI_OOT_SMALL_KEY_FOREST:
        count = comboAddSmallKeyOot(SCE_OOT_TEMPLE_FOREST);
        break;
    case GI_OOT_SMALL_KEY_FIRE:
        count = comboAddSmallKeyOot(SCE_OOT_TEMPLE_FIRE);
        break;
    case GI_OOT_SMALL_KEY_WATER:
        count = comboAddSmallKeyOot(SCE_OOT_TEMPLE_WATER);
        break;
    case GI_OOT_SMALL_KEY_SPIRIT:
        count = comboAddSmallKeyOot(SCE_OOT_TEMPLE_SPIRIT);
        break;
    case GI_OOT_SMALL_KEY_SHADOW:
        count = comboAddSmallKeyOot(SCE_OOT_TEMPLE_SHADOW);
        break;
    case GI_OOT_SMALL_KEY_GANON:
        count = comboAddSmallKeyOot(SCE_OOT_INSIDE_GANON_CASTLE);
        break;
    case GI_OOT_SMALL_KEY_BOTW:
        count = comboAddSmallKeyOot(SCE_OOT_BOTTOM_OF_THE_WELL);
        break;
    case GI_OOT_SMALL_KEY_GF:
        count = comboAddSmallKeyOot(SCE_OOT_THIEVES_HIDEOUT);
        break;
    case GI_OOT_SMALL_KEY_GTG:
        count = comboAddSmallKeyOot(SCE_OOT_GERUDO_TRAINING_GROUND);
        break;
    case GI_OOT_SMALL_KEY_TCG:
        count = comboAddSmallKeyOot(SCE_OOT_TREASURE_SHOP);
        break;
    case GI_OOT_KEY_RING_FOREST:
        comboAddKeyRingOot(SCE_OOT_TEMPLE_FOREST);
        break;
    case GI_OOT_KEY_RING_FIRE:
        comboAddKeyRingOot(SCE_OOT_TEMPLE_FIRE);
        break;
    case GI_OOT_KEY_RING_WATER:
        comboAddKeyRingOot(SCE_OOT_TEMPLE_WATER);
        break;
    case GI_OOT_KEY_RING_SPIRIT:
        comboAddKeyRingOot(SCE_OOT_TEMPLE_SPIRIT);
        break;
    case GI_OOT_KEY_RING_SHADOW:
        comboAddKeyRingOot(SCE_OOT_TEMPLE_SHADOW);
        break;
    case GI_OOT_KEY_RING_GANON:
        comboAddKeyRingOot(SCE_OOT_INSIDE_GANON_CASTLE);
        break;
    case GI_OOT_KEY_RING_BOTW:
        comboAddKeyRingOot(SCE_OOT_BOTTOM_OF_THE_WELL);
        break;
    case GI_OOT_KEY_RING_GF:
        comboAddKeyRingOot(SCE_OOT_THIEVES_HIDEOUT);
        break;
    case GI_OOT_KEY_RING_GTG:
        comboAddKeyRingOot(SCE_OOT_GERUDO_TRAINING_GROUND);
        break;
    case GI_OOT_KEY_RING_TCG:
        comboAddKeyRingOot(SCE_OOT_TREASURE_SHOP);
        break;
    case GI_OOT_SKELETON_KEY:
        comboAddSkeletonKeyOot();
        break;
    case GI_OOT_BOSS_KEY_FOREST:
        comboAddBossKeyOot(SCE_OOT_TEMPLE_FOREST);
        break;
    case GI_OOT_BOSS_KEY_FIRE:
        comboAddBossKeyOot(SCE_OOT_TEMPLE_FIRE);
        break;
    case GI_OOT_BOSS_KEY_WATER:
        comboAddBossKeyOot(SCE_OOT_TEMPLE_WATER);
        break;
    case GI_OOT_BOSS_KEY_SPIRIT:
        comboAddBossKeyOot(SCE_OOT_TEMPLE_SPIRIT);
        break;
    case GI_OOT_BOSS_KEY_SHADOW:
        comboAddBossKeyOot(SCE_OOT_TEMPLE_SHADOW);
        break;
    case GI_OOT_BOSS_KEY_GANON:
        comboAddBossKeyOot(SCE_OOT_GANON_TOWER);
        break;
    case GI_OOT_MAP_DT:
        comboAddMapOot(SCE_OOT_INSIDE_DEKU_TREE);
        break;
    case GI_OOT_MAP_DC:
        comboAddMapOot(SCE_OOT_DODONGO_CAVERN);
        break;
    case GI_OOT_MAP_JJ:
        comboAddMapOot(SCE_OOT_INSIDE_JABU_JABU);
        break;
    case GI_OOT_MAP_FOREST:
        comboAddMapOot(SCE_OOT_TEMPLE_FOREST);
        break;
    case GI_OOT_MAP_FIRE:
        comboAddMapOot(SCE_OOT_TEMPLE_FIRE);
        break;
    case GI_OOT_MAP_WATER:
        comboAddMapOot(SCE_OOT_TEMPLE_WATER);
        break;
    case GI_OOT_MAP_SPIRIT:
        comboAddMapOot(SCE_OOT_TEMPLE_SPIRIT);
        break;
    case GI_OOT_MAP_SHADOW:
        comboAddMapOot(SCE_OOT_TEMPLE_SHADOW);
        break;
    case GI_OOT_MAP_BOTW:
        comboAddMapOot(SCE_OOT_BOTTOM_OF_THE_WELL);
        break;
    case GI_OOT_MAP_IC:
        comboAddMapOot(SCE_OOT_ICE_CAVERN);
        break;
    case GI_OOT_COMPASS_DT:
        comboAddCompassOot(SCE_OOT_INSIDE_DEKU_TREE);
        break;
    case GI_OOT_COMPASS_DC:
        comboAddCompassOot(SCE_OOT_DODONGO_CAVERN);
        break;
    case GI_OOT_COMPASS_JJ:
        comboAddCompassOot(SCE_OOT_INSIDE_JABU_JABU);
        break;
    case GI_OOT_COMPASS_FOREST:
        comboAddCompassOot(SCE_OOT_TEMPLE_FOREST);
        break;
    case GI_OOT_COMPASS_FIRE:
        comboAddCompassOot(SCE_OOT_TEMPLE_FIRE);
        break;
    case GI_OOT_COMPASS_WATER:
        comboAddCompassOot(SCE_OOT_TEMPLE_WATER);
        break;
    case GI_OOT_COMPASS_SPIRIT:
        comboAddCompassOot(SCE_OOT_TEMPLE_SPIRIT);
        break;
    case GI_OOT_COMPASS_SHADOW:
        comboAddCompassOot(SCE_OOT_TEMPLE_SHADOW);
        break;
    case GI_OOT_COMPASS_BOTW:
        comboAddCompassOot(SCE_OOT_BOTTOM_OF_THE_WELL);
        break;
    case GI_OOT_COMPASS_IC:
        comboAddCompassOot(SCE_OOT_ICE_CAVERN);
        break;
    case GI_OOT_TRIFORCE:
    case GI_OOT_TRIFORCE_POWER:
    case GI_OOT_TRIFORCE_COURAGE:
    case GI_OOT_TRIFORCE_WISDOM:
        count = ++gTriforceCount;
        break;
    case GI_OOT_COIN_RED:
        count = ++gSharedCustomSave.coins[0];
        break;
    case GI_OOT_COIN_GREEN:
        count = ++gSharedCustomSave.coins[1];
        break;
    case GI_OOT_COIN_BLUE:
        count = ++gSharedCustomSave.coins[2];
        break;
    case GI_OOT_COIN_YELLOW:
        count = ++gSharedCustomSave.coins[3];
        break;
    case GI_OOT_RUPEE_SILVER_DC:
        count = addSilverRupees(SR_DC, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_BOTW:
        count = addSilverRupees(SR_BOTW, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_SPIRIT_CHILD:
        count = addSilverRupees(SR_SPIRIT1, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_SPIRIT_SUN:
        count = addSilverRupees(SR_SPIRIT2, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_SPIRIT_BOULDERS:
        count = addSilverRupees(SR_SPIRIT3, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_SPIRIT_LOBBY:
        count = addSilverRupees(SR_SPIRIT1, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_SPIRIT_ADULT:
        count = addSilverRupees(SR_SPIRIT2, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_SHADOW_SCYTHE:
        count = addSilverRupees(SR_SHADOW1, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_SHADOW_PIT:
        count = addSilverRupees(SR_SHADOW3, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_SHADOW_SPIKES:
        count = addSilverRupees(SR_SHADOW4, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_SHADOW_BLADES:
        count = addSilverRupees(SR_SHADOW2, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_IC_SCYTHE:
        count = addSilverRupees(SR_IC1, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_IC_BLOCK:
        count = addSilverRupees(SR_IC2, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_GTG_SLOPES:
        count = addSilverRupees(SR_GTG1, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_GTG_LAVA:
        count = addSilverRupees(SR_GTG2, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_GTG_WATER:
        count = addSilverRupees(SR_GTG3, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_GANON_SPIRIT:
        count = addSilverRupees(SR_GANON1, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_GANON_LIGHT:
        count = addSilverRupees(SR_GANON2, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_GANON_FIRE:
        count = addSilverRupees(SR_GANON3, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_GANON_FOREST:
        count = addSilverRupees(SR_GANON4, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_GANON_SHADOW:
        count = addSilverRupees(SR_GANON1, noEffect);
        break;
    case GI_OOT_RUPEE_SILVER_GANON_WATER:
        count = addSilverRupees(SR_GANON2, noEffect);
        break;
    case GI_OOT_POUCH_SILVER_DC:
        addSilverRupeePouch(SR_DC, noEffect);
        break;
    case GI_OOT_POUCH_SILVER_BOTW:
        addSilverRupeePouch(SR_BOTW, noEffect);
        break;
    case GI_OOT_POUCH_SILVER_SPIRIT_CHILD:
        addSilverRupeePouch(SR_SPIRIT1, noEffect);
        break;
    case GI_OOT_POUCH_SILVER_SPIRIT_SUN:
        addSilverRupeePouch(SR_SPIRIT2, noEffect);
        break;
    case GI_OOT_POUCH_SILVER_SPIRIT_BOULDERS:
        addSilverRupeePouch(SR_SPIRIT3, noEffect);
        break;
    case GI_OOT_POUCH_SILVER_SPIRIT_LOBBY:
        addSilverRupeePouch(SR_SPIRIT1, noEffect);
        break;
    case GI_OOT_POUCH_SILVER_SPIRIT_ADULT:
        addSilverRupeePouch(SR_SPIRIT2, noEffect);
        break;
    case GI_OOT_POUCH_SILVER_SHADOW_SCYTHE:
        addSilverRupeePouch(SR_SHADOW1, noEffect);
        break;
    case GI_OOT_POUCH_SILVER_SHADOW_PIT:
        addSilverRupeePouch(SR_SHADOW3, noEffect);
        break;
    case GI_OOT_POUCH_SILVER_SHADOW_SPIKES:
        addSilverRupeePouch(SR_SHADOW4, noEffect);
        break;
    case GI_OOT_POUCH_SILVER_SHADOW_BLADES:
        addSilverRupeePouch(SR_SHADOW2, noEffect);
        break;
    case GI_OOT_POUCH_SILVER_IC_SCYTHE:
        addSilverRupeePouch(SR_IC1, noEffect);
        break;
    case GI_OOT_POUCH_SILVER_IC_BLOCK:
        addSilverRupeePouch(SR_IC2, noEffect);
        break;
    case GI_OOT_POUCH_SILVER_GTG_SLOPES:
        addSilverRupeePouch(SR_GTG1, noEffect);
        break;
    case GI_OOT_POUCH_SILVER_GTG_LAVA:
        addSilverRupeePouch(SR_GTG2, noEffect);
        break;
    case GI_OOT_POUCH_SILVER_GTG_WATER:
        addSilverRupeePouch(SR_GTG3, noEffect);
        break;
    case GI_OOT_POUCH_SILVER_GANON_SPIRIT:
        addSilverRupeePouch(SR_GANON1, noEffect);
        break;
    case GI_OOT_POUCH_SILVER_GANON_LIGHT:
        addSilverRupeePouch(SR_GANON2, noEffect);
        break;
    case GI_OOT_POUCH_SILVER_GANON_FIRE:
        addSilverRupeePouch(SR_GANON3, noEffect);
        break;
    case GI_OOT_POUCH_SILVER_GANON_FOREST:
        addSilverRupeePouch(SR_GANON4, noEffect);
        break;
    case GI_OOT_POUCH_SILVER_GANON_SHADOW:
        addSilverRupeePouch(SR_GANON1, noEffect);
        break;
    case GI_OOT_POUCH_SILVER_GANON_WATER:
        addSilverRupeePouch(SR_GANON2, noEffect);
        break;
    case GI_OOT_RUPEE_MAGICAL:
        addMagicalRupee(noEffect);
        break;
    case GI_OOT_SOUL_ENEMY_STALFOS:
    case GI_OOT_SOUL_ENEMY_OCTOROK:
    case GI_OOT_SOUL_ENEMY_WALLMASTER:
    case GI_OOT_SOUL_ENEMY_DODONGO:
    case GI_OOT_SOUL_ENEMY_KEESE:
    case GI_OOT_SOUL_ENEMY_TEKTITE:
    case GI_OOT_SOUL_ENEMY_LEEVER:
    case GI_OOT_SOUL_ENEMY_PEAHAT:
    case GI_OOT_SOUL_ENEMY_LIZALFOS_DINALFOS:
    case GI_OOT_SOUL_ENEMY_GOHMA_LARVA:
    case GI_OOT_SOUL_ENEMY_SHABOM:
    case GI_OOT_SOUL_ENEMY_BABY_DODONGO:
    case GI_OOT_SOUL_ENEMY_DARK_LINK:
    case GI_OOT_SOUL_ENEMY_BIRI_BARI:
    case GI_OOT_SOUL_ENEMY_TAILPASARN:
    case GI_OOT_SOUL_ENEMY_PARASITE:
    case GI_OOT_SOUL_ENEMY_SKULLTULA:
    case GI_OOT_SOUL_ENEMY_TORCH_SLUG:
    case GI_OOT_SOUL_ENEMY_MOBLIN:
    case GI_OOT_SOUL_ENEMY_ARMOS:
    case GI_OOT_SOUL_ENEMY_DEKU_BABA:
    case GI_OOT_SOUL_ENEMY_DEKU_SCRUB:
    case GI_OOT_SOUL_ENEMY_BUBBLE:
    case GI_OOT_SOUL_ENEMY_BEAMOS:
    case GI_OOT_SOUL_ENEMY_FLOORMASTER:
    case GI_OOT_SOUL_ENEMY_REDEAD_GIBDO:
    case GI_OOT_SOUL_ENEMY_SKULLWALLTULA:
    case GI_OOT_SOUL_ENEMY_FLARE_DANCER:
    case GI_OOT_SOUL_ENEMY_DEAD_HAND:
    case GI_OOT_SOUL_ENEMY_SHELL_BLADE:
    case GI_OOT_SOUL_ENEMY_LIKE_LIKE:
    case GI_OOT_SOUL_ENEMY_SPIKE:
    case GI_OOT_SOUL_ENEMY_ANUBIS:
    case GI_OOT_SOUL_ENEMY_IRON_KNUCKLE:
    case GI_OOT_SOUL_ENEMY_SKULL_KID:
    case GI_OOT_SOUL_ENEMY_FLYING_POT:
    case GI_OOT_SOUL_ENEMY_FREEZARD:
    case GI_OOT_SOUL_ENEMY_STINGER:
    case GI_OOT_SOUL_ENEMY_WOLFOS:
    case GI_OOT_SOUL_ENEMY_STALCHILD:
    case GI_OOT_SOUL_ENEMY_GUAY:
    case GI_OOT_SOUL_BOSS_QUEEN_GOHMA:
    case GI_OOT_SOUL_BOSS_KING_DODONGO:
    case GI_OOT_SOUL_BOSS_BARINADE:
    case GI_OOT_SOUL_BOSS_PHANTOM_GANON:
    case GI_OOT_SOUL_BOSS_VOLVAGIA:
    case GI_OOT_SOUL_BOSS_MORPHA:
    case GI_OOT_SOUL_BOSS_BONGO_BONGO:
    case GI_OOT_SOUL_BOSS_TWINROVA:
    case GI_OOT_SOUL_NPC_SARIA:
    case GI_OOT_SOUL_NPC_DARUNIA:
    case GI_OOT_SOUL_NPC_RUTO:
    case GI_OOT_SOUL_NPC_KING_ZORA:
    case GI_OOT_SOUL_NPC_ZELDA:
    case GI_OOT_SOUL_NPC_SHEIK:
    case GI_OOT_SOUL_NPC_CUCCO_LADY:
    case GI_OOT_SOUL_NPC_CARPENTERS:
    case GI_OOT_SOUL_NPC_GURU_GURU:
    case GI_OOT_SOUL_NPC_MIDO:
    case GI_OOT_SOUL_NPC_KOKIRI:
    case GI_OOT_SOUL_NPC_KOKIRI_SHOPKEEPER:
    case GI_OOT_SOUL_NPC_HYLIAN_GUARD:
    case GI_OOT_SOUL_NPC_HYLIAN_CITIZEN:
    case GI_OOT_SOUL_NPC_MALON:
    case GI_OOT_SOUL_NPC_TALON:
    case GI_OOT_SOUL_NPC_FISHING_POND_OWNER:
    case GI_OOT_SOUL_NPC_GORON:
    case GI_OOT_SOUL_NPC_MEDIGORON:
    case GI_OOT_SOUL_NPC_BIGGORON:
    case GI_OOT_SOUL_NPC_GORON_CHILD:
    case GI_OOT_SOUL_NPC_GORON_SHOPKEEPER:
    case GI_OOT_SOUL_NPC_ZORA:
    case GI_OOT_SOUL_NPC_ZORA_SHOPKEEPER:
    case GI_OOT_SOUL_NPC_BAZAAR_SHOPKEEPER:
    case GI_OOT_SOUL_NPC_BOMBCHU_SHOPKEEPER:
    case GI_OOT_SOUL_NPC_POTION_SHOPKEEPER:
    case GI_OOT_SOUL_NPC_BOMBCHU_BOWLING_LADY:
    case GI_OOT_SOUL_NPC_TRASURE_CHEST_GAME_OWNER:
    case GI_OOT_SOUL_NPC_SHOOTING_GALLERY_OWNER:
    case GI_OOT_SOUL_NPC_DAMPE:
    case GI_OOT_SOUL_NPC_GRAVEYARD_KID:
    case GI_OOT_SOUL_NPC_POE_COLLECTOR:
        comboAddSoulOot(gi);
        break;
    case GI_OOT_BUTTON_A:
        gSharedCustomSave.ocarinaButtonMaskOot |= A_BUTTON;
        break;
    case GI_OOT_BUTTON_C_RIGHT:
        gSharedCustomSave.ocarinaButtonMaskOot |= R_CBUTTONS;
        break;
    case GI_OOT_BUTTON_C_LEFT:
        gSharedCustomSave.ocarinaButtonMaskOot |= L_CBUTTONS;
        break;
    case GI_OOT_BUTTON_C_UP:
        gSharedCustomSave.ocarinaButtonMaskOot |= U_CBUTTONS;
        break;
    case GI_OOT_BUTTON_C_DOWN:
        gSharedCustomSave.ocarinaButtonMaskOot |= D_CBUTTONS;
        break;
    case GI_OOT_FISHING_POND_CHILD_FISH_2LBS:
    case GI_OOT_FISHING_POND_CHILD_FISH_3LBS:
    case GI_OOT_FISHING_POND_CHILD_FISH_4LBS:
    case GI_OOT_FISHING_POND_CHILD_FISH_5LBS:
    case GI_OOT_FISHING_POND_CHILD_FISH_6LBS:
    case GI_OOT_FISHING_POND_CHILD_FISH_7LBS:
    case GI_OOT_FISHING_POND_CHILD_FISH_8LBS:
    case GI_OOT_FISHING_POND_CHILD_FISH_9LBS:
    case GI_OOT_FISHING_POND_CHILD_FISH_10LBS:
    case GI_OOT_FISHING_POND_CHILD_FISH_11LBS:
    case GI_OOT_FISHING_POND_CHILD_FISH_12LBS:
    case GI_OOT_FISHING_POND_CHILD_FISH_13LBS:
    case GI_OOT_FISHING_POND_CHILD_FISH_14LBS:
    {
        u8 caughtListLength = gSharedCustomSave.caughtChildFishWeight[0];
        if (caughtListLength < ARRAY_SIZE(gSharedCustomSave.caughtChildFishWeight) - 1) {
            s16 pounds = gi - GI_OOT_FISHING_POND_CHILD_FISH_2LBS + 2;
            gSharedCustomSave.caughtChildFishWeight[++gSharedCustomSave.caughtChildFishWeight[0]] = pounds & 0x7F;
        }
    }
        break;
    case GI_OOT_FISHING_POND_CHILD_LOACH_14LBS:
    case GI_OOT_FISHING_POND_CHILD_LOACH_15LBS:
    case GI_OOT_FISHING_POND_CHILD_LOACH_16LBS:
    case GI_OOT_FISHING_POND_CHILD_LOACH_17LBS:
    case GI_OOT_FISHING_POND_CHILD_LOACH_18LBS:
    case GI_OOT_FISHING_POND_CHILD_LOACH_19LBS:
    {
        u8 caughtListLength = gSharedCustomSave.caughtChildFishWeight[0];
        if (caughtListLength < ARRAY_SIZE(gSharedCustomSave.caughtChildFishWeight) - 1) {
            s16 pounds = gi - GI_OOT_FISHING_POND_CHILD_LOACH_14LBS + 14;
            gSharedCustomSave.caughtChildFishWeight[++gSharedCustomSave.caughtChildFishWeight[0]] = (pounds & 0x7F) | 0x80;
        }
    }
        break;
    case GI_OOT_FISHING_POND_ADULT_FISH_4LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_5LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_6LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_7LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_8LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_9LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_10LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_11LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_12LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_13LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_14LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_15LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_16LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_17LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_18LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_19LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_20LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_21LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_22LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_23LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_24LBS:
    case GI_OOT_FISHING_POND_ADULT_FISH_25LBS:
    {
        u8 caughtListLength = gSharedCustomSave.caughtAdultFishWeight[0];
        if (caughtListLength < ARRAY_SIZE(gSharedCustomSave.caughtAdultFishWeight) - 1) {
            s16 pounds = gi - GI_OOT_FISHING_POND_ADULT_FISH_4LBS + 4;
            gSharedCustomSave.caughtAdultFishWeight[++gSharedCustomSave.caughtAdultFishWeight[0]] = pounds & 0x7F;
        }
    }
        break;
    case GI_OOT_FISHING_POND_ADULT_LOACH_29LBS:
    case GI_OOT_FISHING_POND_ADULT_LOACH_30LBS:
    case GI_OOT_FISHING_POND_ADULT_LOACH_31LBS:
    case GI_OOT_FISHING_POND_ADULT_LOACH_32LBS:
    case GI_OOT_FISHING_POND_ADULT_LOACH_33LBS:
    case GI_OOT_FISHING_POND_ADULT_LOACH_34LBS:
    case GI_OOT_FISHING_POND_ADULT_LOACH_35LBS:
    case GI_OOT_FISHING_POND_ADULT_LOACH_36LBS:
    {
        u8 caughtListLength = gSharedCustomSave.caughtAdultFishWeight[0];
        if (caughtListLength < ARRAY_SIZE(gSharedCustomSave.caughtAdultFishWeight) - 1) {
            s16 pounds = gi - GI_OOT_FISHING_POND_ADULT_LOACH_29LBS + 29;
            gSharedCustomSave.caughtAdultFishWeight[++gSharedCustomSave.caughtAdultFishWeight[0]] = (pounds & 0x7F) | 0x80;
        }
    }
        break;
    }

    return count;
}
