#include <combo.h>
#include <combo/item.h>
#include <combo/souls.h>

static const u16 kSwords[] = {
    ITEM_NONE,
    ITEM_MM_SWORD_KOKIRI,
    ITEM_MM_SWORD_RAZOR,
    ITEM_MM_SWORD_GILDED,
};

int comboAddSmallKeyMm(u16 dungeonId)
{
    s8 keyCount;

    /* Max keys */
    if (gMmSave.inventory.dungeonItems[dungeonId].maxKeys >= g.maxKeysMm[dungeonId])
        return 0;

    keyCount = gMmSave.inventory.dungeonKeys[dungeonId];
    if (keyCount < 0)
        keyCount = 1;
    else
        keyCount++;
    gMmSave.inventory.dungeonKeys[dungeonId] = keyCount;
    gMmSave.inventory.dungeonItems[dungeonId].maxKeys++;

    return gMmSave.inventory.dungeonItems[dungeonId].maxKeys;
}

void comboAddKeyRingMm(u16 dungeonId)
{
    for (int i = 0; i < g.maxKeysMm[dungeonId]; i++)
        comboAddSmallKeyMm(dungeonId);
}

void comboAddSkeletonKeyMm(void)
{
    for (int i = 0; i < ARRAY_SIZE(g.maxKeysMm); ++i)
        comboAddKeyRingMm(i);
}

void comboAddBossKeyMm(u16 dungeonId)
{
    gMmSave.inventory.dungeonItems[dungeonId].bossKey = 1;
}

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

void comboAddMapMm(u16 dungeonId)
{
    gMmSave.inventory.dungeonItems[dungeonId].map = 1;
}

void comboAddCompassMm(u16 dungeonId)
{
    gMmSave.inventory.dungeonItems[dungeonId].compass = 1;
}

static void addHealth(u8 count)
{
    u16 health;

    health = (u16)count * 0x10;
    gMmSave.playerData.health += health;
    if (gMmSave.playerData.health > gMmSave.playerData.healthMax)
        gMmSave.playerData.health = gMmSave.playerData.healthMax;
}

static void addSword(int index)
{
    gMmSave.itemEquips.buttonItems[0][0] = kSwords[index];
    gMmSave.itemEquips.sword = index;
}

static void addShield(int index)
{
    if (index > gMmSave.itemEquips.shield)
        gMmSave.itemEquips.shield = index;
}

void comboAddBombBagMm(int index)
{
    gMmSave.inventory.items[ITS_MM_BOMBS] = ITEM_MM_BOMB;
    gMmSave.inventory.upgrades.bombBag = index;
    gMmSave.inventory.ammo[ITS_MM_BOMBS] = kMaxBombs[index];
}

void comboAddBombsMm(int count)
{
    u16 max;

    if (gMmSave.inventory.upgrades.bombBag == 0)
        return;
    max = kMaxBombs[gMmSave.inventory.upgrades.bombBag];
    gMmSave.inventory.items[ITS_MM_BOMBS] = ITEM_MM_BOMB;
    gMmSave.inventory.ammo[ITS_MM_BOMBS] += count;
    if (gMmSave.inventory.ammo[ITS_MM_BOMBS] > max)
        gMmSave.inventory.ammo[ITS_MM_BOMBS] = max;
}

static void addNewBottle(u16 itemId)
{
    int i;

    for (i = 0; i < 6; i++)
    {
        if (gMmSave.inventory.items[ITS_MM_BOTTLE + i] == ITEM_NONE)
        {
            gMmSave.inventory.items[ITS_MM_BOTTLE + i] = itemId;
            break;
        }
    }
}

static void fillBottle(u16 itemId)
{
    int slot;

    slot = -1;
    for (int i = 0; i < 6; ++i)
    {
        if (gMmSave.inventory.items[ITS_MM_BOTTLE + i] == ITEM_MM_BOTTLE_EMPTY)
        {
            slot = i;
            break;
        }
    }
    if (slot == -1)
        return;
    gMmSave.inventory.items[ITS_MM_BOTTLE + slot] = itemId;
    for (int i = 1; i <= 3; ++i)
    {
        if (gMmSave.itemEquips.cButtonSlots[0][i] == ITS_MM_BOTTLE + slot)
        {
            gMmSave.itemEquips.buttonItems[0][i] = itemId;
        }
    }
}

static void addOcarina(int level)
{
    u8 itemId;

    if (level >= 2)
        itemId = ITEM_MM_OCARINA_OF_TIME;
    else
        itemId = ITEM_MM_OCARINA_FAIRY;
    gMmSave.inventory.items[ITS_MM_OCARINA] = itemId;
    gMmExtraItems.ocarina |= (1 << (level - 1));
    //reloadSlotMm(ITS_MM_OCARINA);
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

void comboAddCommonItemMm(int sid, int noEffect)
{
    switch (sid)
    {
    case SITEM_SONG_EPONA:
        gMmSave.inventory.quest.songEpona = 1;
        break;
    case SITEM_SONG_TIME:
        gMmSave.inventory.quest.songTime = 1;
        break;
    case SITEM_SONG_STORMS:
        gMmSave.inventory.quest.songStorms = 1;
        break;
    case SITEM_OCARINA_FAIRY:
        addOcarina(1);
        break;
    case SITEM_OCARINA_TIME:
        addOcarina(2);
        break;
    case SITEM_HEART_PIECE:
        gMmSave.inventory.quest.heartPieces += 1;
        if (gMmSave.inventory.quest.heartPieces == 4)
        {
            gMmSave.inventory.quest.heartPieces = 0;
            gMmSave.playerData.healthMax += 0x10;
        }
        if (noEffect)
            addHealth(20);
        break;
    case SITEM_HEART_CONTAINER:
        gMmSave.playerData.healthMax += 0x10;
        if (noEffect)
            addHealth(20);
        break;
    case SITEM_RECOVERY_HEART:
        if (noEffect)
            addHealth(1);
        break;
    case SITEM_DEFENSE_UPGRADE:
        gMmSave.playerData.doubleDefense = 1;
        gMmSave.inventory.defenseHearts = 20;
        if (noEffect)
            addHealth(20);
        break;
    case SITEM_SONG_SUN:
        gMmSave.inventory.quest.songSun = 1;
        break;
    }
}

void comboAddItemSharedMm(s16 gi, int noEffect)
{
    if (comboConfig(CFG_SHARED_BOMB_BAGS))
    {
        switch (gi)
        {
        case GI_MM_BOMB_BAG:
            comboAddBombBagOot(1);
            break;
        case GI_MM_BOMB_BAG2:
            comboAddBombBagOot(2);
            break;
        case GI_MM_BOMB_BAG3:
            comboAddBombBagOot(3);
            break;
        }
    }

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

    if (comboConfig(CFG_SHARED_SONG_EPONA) && gi == GI_MM_SONG_EPONA)
    {
        comboAddCommonItemOot(SITEM_SONG_EPONA, noEffect);
    }

    if (comboConfig(CFG_SHARED_SONG_STORMS) && gi == GI_MM_SONG_STORMS)
    {
        comboAddCommonItemOot(SITEM_SONG_STORMS, noEffect);
    }

    if (comboConfig(CFG_SHARED_SONG_TIME) && gi == GI_MM_SONG_TIME)
    {
        comboAddCommonItemOot(SITEM_SONG_TIME, noEffect);
    }

    if (comboConfig(CFG_SHARED_SONG_SUN) && gi == GI_MM_SONG_SUN)
    {
        comboAddCommonItemOot(SITEM_SONG_SUN, noEffect);
    }

    if (comboConfig(CFG_SHARED_OCARINA))
    {
        switch (gi)
        {
        case GI_MM_OCARINA_FAIRY:
            comboAddCommonItemOot(SITEM_OCARINA_FAIRY, noEffect);
            break;
        case GI_MM_OCARINA_OF_TIME:
            comboAddCommonItemOot(SITEM_OCARINA_TIME, noEffect);
            break;
        }
    }

    if (comboConfig(CFG_SHARED_HEALTH))
    {
        switch (gi)
        {
        case GI_MM_RECOVERY_HEART:
            comboAddCommonItemOot(SITEM_RECOVERY_HEART, noEffect);
            break;
        case GI_MM_HEART_CONTAINER:
            comboAddCommonItemOot(SITEM_HEART_CONTAINER, noEffect);
            break;
        case GI_MM_HEART_PIECE:
            comboAddCommonItemOot(SITEM_HEART_PIECE, noEffect);
            break;
        case GI_MM_DEFENSE_UPGRADE:
            comboAddCommonItemOot(SITEM_DEFENSE_UPGRADE, noEffect);
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

    if (comboConfig(CFG_SHARED_SKELETON_KEY) && gi == GI_MM_SKELETON_KEY)
        comboAddSkeletonKeyOot();

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
    case GI_MM_OCARINA_FAIRY:
        comboAddCommonItemMm(SITEM_OCARINA_FAIRY, noEffect);
        break;
    case GI_MM_OCARINA_OF_TIME:
        comboAddCommonItemMm(SITEM_OCARINA_TIME, noEffect);
        break;
    case GI_MM_MAGIC_BEAN:
        gMmSave.inventory.items[ITS_MM_BEANS] = ITEM_MM_MAGIC_BEAN;
        gMmSave.inventory.ammo[ITS_MM_BEANS]++;
        break;
    case GI_MM_POWDER_KEG:
        gMmSave.inventory.items[ITS_MM_KEG] = ITEM_MM_POWDER_KEG;
        gMmSave.inventory.ammo[ITS_MM_KEG] = 1;
        break;
    case GI_MM_BOTTLE_EMPTY:
        addNewBottle(ITEM_MM_BOTTLE_EMPTY);
        break;
    case GI_MM_BOTTLE_MILK:
        addNewBottle(ITEM_MM_MILK);
        break;
    case GI_MM_BOTTLED_GOLD_DUST:
        addNewBottle(ITEM_MM_BOTTLED_GOLD_DUST);
        gMmExtraItems.goldDust = 1;
        break;
    case GI_MM_BOTTLE_CHATEAU:
        addNewBottle(ITEM_MM_BOTTLE_CHATEAU);
        break;
    case GI_MM_BOTTLE_POTION_RED:
        addNewBottle(ITEM_MM_POTION_RED);
        break;
    case GI_MM_BOTTLE_POTION_GREEN:
        addNewBottle(ITEM_MM_POTION_GREEN);
        break;
    case GI_MM_BOTTLE_POTION_BLUE:
        addNewBottle(ITEM_MM_POTION_BLUE);
        break;
    case GI_MM_POTION_RED:
        fillBottle(ITEM_MM_POTION_RED);
        break;
    case GI_MM_POTION_BLUE:
        fillBottle(ITEM_MM_POTION_BLUE);
        break;
    case GI_MM_POTION_GREEN:
        fillBottle(ITEM_MM_POTION_GREEN);
        break;
    case GI_MM_SEAHORSE:
    case GI_MM_SEAHORSE2:
        fillBottle(ITEM_MM_BOTTLED_SEAHORSE);
        break;
    case GI_MM_MILK:
        fillBottle(ITEM_MM_MILK);
        break;
    case GI_MM_FAIRY:
        fillBottle(ITEM_MM_FAIRY);
        break;
    case GI_MM_CHATEAU:
        fillBottle(ITEM_MM_BOTTLE_CHATEAU);
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
    case GI_MM_SWORD_KOKIRI:
        addSword(1);
        break;
    case GI_MM_SWORD_RAZOR:
        addSword(2);
        gMmSave.playerData.swordHealth = 100;
        break;
    case GI_MM_SWORD_GILDED:
        addSword(3);
        break;
    case GI_MM_PROGRESSIVE_SHIELD_HERO:
        gMmExtraFlags2.progressiveShield = 1;
        /* Fallthrough */
    case GI_MM_SHIELD_HERO:
        addShield(1);
        break;
    case GI_MM_SHIELD_MIRROR:
        addShield(2);
        break;
    case GI_MM_BOMB_BAG:
        comboAddBombBagMm(1);
        break;
    case GI_MM_BOMB_BAG2:
        comboAddBombBagMm(2);
        break;
    case GI_MM_BOMB_BAG3:
        comboAddBombBagMm(3);
        break;
    case GI_MM_GS_TOKEN_SWAMP:
        count = ++gMmSave.skullCountSwamp;
        break;
    case GI_MM_GS_TOKEN_OCEAN:
        count = ++gMmSave.skullCountOcean;
        break;
    case GI_MM_SONG_AWAKENING:
        gMmSave.inventory.quest.songAwakening = 1;
        break;
    case GI_MM_SONG_GORON:
        gMmSave.inventory.quest.songLullaby = 1;
        break;
    case GI_MM_SONG_ZORA:
        gMmSave.inventory.quest.songNewWave = 1;
        break;
    case GI_MM_SONG_EMPTINESS:
        gMmSave.inventory.quest.songEmpty = 1;
        break;
    case GI_MM_SONG_ORDER:
        gMmSave.inventory.quest.songOrder = 1;
        break;
    case GI_MM_SONG_TIME:
        comboAddCommonItemMm(SITEM_SONG_TIME, noEffect);
        break;
    case GI_MM_SONG_HEALING:
        gMmSave.inventory.quest.songHealing = 1;
        break;
    case GI_MM_SONG_EPONA:
        comboAddCommonItemMm(SITEM_SONG_EPONA, noEffect);
        break;
    case GI_MM_SONG_SOARING:
        gMmSave.inventory.quest.songSoaring = 1;
        break;
    case GI_MM_SONG_STORMS:
        comboAddCommonItemMm(SITEM_SONG_STORMS, noEffect);
        break;
    case GI_MM_SONG_GORON_HALF:
        gMmSave.inventory.quest.songLullabyIntro = 1;
        break;
    case GI_MM_SONG_SUN:
        comboAddCommonItemMm(SITEM_SONG_SUN, noEffect);
        break;
    case GI_MM_BOMBER_NOTEBOOK:
        gMmSave.inventory.quest.notebook = 1;
        break;
    case GI_MM_REMAINS_ODOLWA:
        gMmSave.inventory.quest.remainsOdolwa = 1;
        break;
    case GI_MM_REMAINS_GOHT:
        gMmSave.inventory.quest.remainsGoht = 1;
        break;
    case GI_MM_REMAINS_GYORG:
        gMmSave.inventory.quest.remainsGyorg = 1;
        break;
    case GI_MM_REMAINS_TWINMOLD:
        gMmSave.inventory.quest.remainsTwinmold = 1;
        break;
    case GI_MM_HEART_PIECE:
        comboAddCommonItemMm(SITEM_HEART_PIECE, noEffect);
        break;
    case GI_MM_HEART_CONTAINER:
        comboAddCommonItemMm(SITEM_HEART_CONTAINER, noEffect);
        break;
    case GI_MM_RECOVERY_HEART:
        comboAddCommonItemMm(SITEM_RECOVERY_HEART, noEffect);
        break;
    case GI_MM_SMALL_KEY_WF:
        count = comboAddSmallKeyMm(0);
        break;
    case GI_MM_SMALL_KEY_SH:
        count = comboAddSmallKeyMm(1);
        break;
    case GI_MM_SMALL_KEY_GB:
        count = comboAddSmallKeyMm(2);
        break;
    case GI_MM_SMALL_KEY_ST:
        count = comboAddSmallKeyMm(3);
        break;
    case GI_MM_KEY_RING_WF:
        comboAddKeyRingMm(0);
        break;
    case GI_MM_KEY_RING_SH:
        comboAddKeyRingMm(1);
        break;
    case GI_MM_KEY_RING_GB:
        comboAddKeyRingMm(2);
        break;
    case GI_MM_KEY_RING_ST:
        comboAddKeyRingMm(3);
        break;
    case GI_MM_SKELETON_KEY:
        comboAddSkeletonKeyMm();
        break;
    case GI_MM_BOSS_KEY_WF:
        comboAddBossKeyMm(0);
        break;
    case GI_MM_BOSS_KEY_SH:
        comboAddBossKeyMm(1);
        break;
    case GI_MM_BOSS_KEY_GB:
        comboAddBossKeyMm(2);
        break;
    case GI_MM_BOSS_KEY_ST:
        comboAddBossKeyMm(3);
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
    case GI_MM_DEFENSE_UPGRADE:
        comboAddCommonItemMm(SITEM_DEFENSE_UPGRADE, noEffect);
        break;
    case GI_MM_SPIN_UPGRADE:
        MM_SET_EVENT_WEEK(EV_MM_WEEK_SPIN_UPGRADE);
        break;
    case GI_MM_MAP_WF:
        comboAddMapMm(0);
        break;
    case GI_MM_MAP_SH:
        comboAddMapMm(1);
        break;
    case GI_MM_MAP_GB:
        comboAddMapMm(2);
        break;
    case GI_MM_MAP_ST:
        comboAddMapMm(3);
        break;
    case GI_MM_COMPASS_WF:
        comboAddCompassMm(0);
        break;
    case GI_MM_COMPASS_SH:
        comboAddCompassMm(1);
        break;
    case GI_MM_COMPASS_GB:
        comboAddCompassMm(2);
        break;
    case GI_MM_COMPASS_ST:
        comboAddCompassMm(3);
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
