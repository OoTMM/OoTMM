#include <combo.h>
#include <combo/age.h>
#include <combo/inventory.h>
#include <combo/item.h>
#include <combo/player.h>
#include <combo/config.h>

int Age_GetStarting(void)
{
    return Config_Flag(CFG_OOT_START_ADULT) ? AGE_ADULT : AGE_CHILD;
}

static void Age_SwapFaroreOot(void)
{
    OotFaroreWind* current;
    OotFaroreWind* prev;
    OotFaroreWind tmp;

    current = Age_GetFaroreOot(gOotSave.age);
    prev = Age_GetFaroreOot(1 - gOotSave.age);

    memcpy(&tmp, current, sizeof(tmp));
    memcpy(current, prev, sizeof(tmp));
    memcpy(prev, &tmp, sizeof(tmp));
}

OotFaroreWind* Age_GetFaroreOot(int age)
{
    OotFaroreWind* current = &gOotSave.info.fw;

    if (age == gOotSave.age)
        return current;

    return current - 1;
}

static void Age_SwapEquipmentOot(void)
{
    OotItemEquips* prevAge;
    OotItemEquips* nextAge;
    u8 item;

    if (gOotSave.age == AGE_ADULT)
    {
        prevAge = &gOotSave.info.adultEquips;
        nextAge = &gOotSave.info.childEquips;
    }
    else
    {
        prevAge = &gOotSave.info.childEquips;
        nextAge = &gOotSave.info.adultEquips;
    }

    memcpy(prevAge, &gOotSave.info.equips, sizeof(*prevAge));
    if (EV_OOT_IS_SWORDLESS())
        prevAge->buttonItems[0] = ITEM_NONE;
    memcpy(&gOotSave.info.equips, nextAge, sizeof(*nextAge));

    /* Reload bottles */
    for (int i = 0; i < 3; ++i)
    {
        item = gOotSave.info.equips.buttonItems[i + 1];
        if ((item >= ITEM_OOT_BOTTLE_EMPTY && item <= ITEM_OOT_POE) || comboIsTradeBottleOot(item))
            item = gOotSave.info.inventory.items[gOotSave.info.equips.cButtonSlots[i]];
        gOotSave.info.equips.buttonItems[i + 1] = item;
    }

    /* Fix sword */
    if (gOotSave.info.equips.buttonItems[0] == ITEM_NONE)
        EV_OOT_SET_SWORDLESS();
    else
        EV_OOT_UNSET_SWORDLESS();

    /* Fix shield, if opposite age lost it */
    if (gOotSave.info.equips.equipment.shields && !(gOotSave.info.inventory.equipment.shields & (1 << (gOotSave.info.equips.equipment.shields - 1))))
        gOotSave.info.equips.equipment.shields = 0;
}


static void Age_SwapEquipmentMm(void)
{
    MmHumanAgeLoadout* curAge;
    MmHumanAgeLoadout* newAge;

    curAge = &gSharedCustomSave.mm.humanAgeLoadouts[gMmSave.linkAge];
    newAge = &gSharedCustomSave.mm.humanAgeLoadouts[1 - gMmSave.linkAge];

    /* Save current equips */
    for (int i = EQUIP_SLOT_C_LEFT; i <= EQUIP_SLOT_C_RIGHT; ++i)
    {
        curAge->buttonItems[i] = gMmSave.info.itemEquips.buttonItems[0][i];
        curAge->cButtonSlots[i] = gMmSave.info.itemEquips.cButtonSlots[0][i];
    }
    curAge->boots = gMmSave.info.itemEquips.boots;
    curAge->tunic = gMmSave.info.itemEquips.tunic;

    /* Validate new age logical equipment */
    if (newAge->sword != MM_SWORD_NONE && !MmSword_IsOwned((MmSwordId)newAge->sword))
        newAge->sword = MM_SWORD_NONE;

    if (newAge->shield != MM_SHIELD_NONE && !MmShield_IsOwned((MmShieldId)newAge->shield))
        newAge->shield = MM_SHIELD_NONE;

    /* Load new equips */
    for (int i = EQUIP_SLOT_C_LEFT; i <= EQUIP_SLOT_C_RIGHT; ++i)
    {
        gMmSave.info.itemEquips.buttonItems[0][i] = newAge->buttonItems[i];
        gMmSave.info.itemEquips.cButtonSlots[0][i] = newAge->cButtonSlots[i];
    }
    gMmSave.info.itemEquips.boots = newAge->boots;
    gMmSave.info.itemEquips.tunic = newAge->tunic;

    /* Reload bottles */
    for (int i = EQUIP_SLOT_C_LEFT; i <= EQUIP_SLOT_C_RIGHT; ++i)
    {
        u8 slot = gMmSave.info.itemEquips.cButtonSlots[0][i];
        if (slot >= ITS_MM_BOTTLE && slot <= ITS_MM_BOTTLE6)
            gMmSave.info.itemEquips.buttonItems[0][i] = gMmSave.info.inventory.items[slot];
    }
}

static void Age_OnChangeOot(void)
{
    Age_SwapFaroreOot();
    Age_SwapEquipmentOot();
#if defined(GAME_OOT)
    if (Config_Flag(CFG_MM_CROSS_AGE))
    {
        gMmSave.equippedMask = 0;
        gSharedCustomSave.mm.customMask = 0;
    }
#endif
}

#if defined(GAME_OOT)
PATCH_FUNC(0x8006f804, Age_OnChangeOot);
#endif

static void Age_OnChangeMm(void)
{
    Age_SwapEquipmentMm();
}

void Age_SetRawOot(PlayState* play, int age)
{
    if (gOotSave.age == age)
        return;

#if defined(GAME_OOT)
    /* Defer on next load if possible */
    if (play)
    {
        play->linkAgeOnLoad = age;
        return;
    }
#endif

    Age_OnChangeOot();
    gOotSave.age = age;
}

void Age_SetRawMm(PlayState* play, int age)
{
    if (gMmSave.linkAge == age)
        return;

    Age_OnChangeMm();
    gMmSave.linkAge = age;
    MmSword_RefreshNativeEquip(NULL);
    MmShield_RefreshNativeEquip(NULL);
}

void Age_SetOot(PlayState* play, int age)
{
    Age_SetRawOot(play, age);
    if (Config_Flag(CFG_MM_CROSS_AGE))
        Age_SetRawMm(play, age);
}

void Age_SwapOot(PlayState* play)
{
    Age_SetOot(play,  1 - gOotSave.age);
}

void Age_SetMm(PlayState* play, int age)
{
    Age_SetRawMm(play, age);
    if (Config_Flag(CFG_MM_CROSS_AGE))
        Age_SetRawOot(play, age);
}

void Age_SwapMm(PlayState* play)
{
    Age_SetMm(play,  1 - gMmSave.linkAge);
}

typedef struct ItemAgeReqConfig
{
    u8 item;
    u16 adultFlag;
    u16 childFlag;
} ItemAgeReqConfig;

static const ItemAgeReqConfig kMmItemAgeReqConfigs[] =
{
    { ITEM_MM_HOOKSHOT,           CFG_MM_AGE_REQ_ADULT_HOOKSHOT,           CFG_MM_AGE_REQ_CHILD_HOOKSHOT },
    { ITEM_MM_HOOKSHOT_SHORT,     CFG_MM_AGE_REQ_ADULT_HOOKSHOT_SHORT,     CFG_MM_AGE_REQ_CHILD_HOOKSHOT_SHORT },
    { ITEM_MM_BOW,                CFG_MM_AGE_REQ_ADULT_BOW,                CFG_MM_AGE_REQ_CHILD_BOW },
    { ITEM_MM_ARROW_FIRE,         CFG_MM_AGE_REQ_ADULT_ARROW_FIRE,         CFG_MM_AGE_REQ_CHILD_ARROW_FIRE },
    { ITEM_MM_ARROW_ICE,          CFG_MM_AGE_REQ_ADULT_ARROW_ICE,          CFG_MM_AGE_REQ_CHILD_ARROW_ICE },
    { ITEM_MM_ARROW_LIGHT,        CFG_MM_AGE_REQ_ADULT_ARROW_LIGHT,        CFG_MM_AGE_REQ_CHILD_ARROW_LIGHT },
    { ITEM_MM_OCARINA_OF_TIME,    CFG_MM_AGE_REQ_ADULT_OCARINA_OF_TIME,    CFG_MM_AGE_REQ_CHILD_OCARINA_OF_TIME },
    { ITEM_MM_OCARINA_FAIRY,      CFG_MM_AGE_REQ_ADULT_OCARINA_FAIRY,      CFG_MM_AGE_REQ_CHILD_OCARINA_FAIRY },
    { ITEM_MM_BOMB,               CFG_MM_AGE_REQ_ADULT_BOMB,               CFG_MM_AGE_REQ_CHILD_BOMB },
    { ITEM_MM_BOMBCHU,            CFG_MM_AGE_REQ_ADULT_BOMBCHU,            CFG_MM_AGE_REQ_CHILD_BOMBCHU },
    { ITEM_MM_STICK,              CFG_MM_AGE_REQ_ADULT_STICK,              CFG_MM_AGE_REQ_CHILD_STICK },
    { ITEM_MM_NUT,                CFG_MM_AGE_REQ_ADULT_DEKU_NUTS,          CFG_MM_AGE_REQ_CHILD_DEKU_NUTS },
    { ITEM_MM_MAGIC_BEAN,         CFG_MM_AGE_REQ_ADULT_MAGIC_BEAN,         CFG_MM_AGE_REQ_CHILD_MAGIC_BEAN },
    { ITEM_MM_POWDER_KEG,         CFG_MM_AGE_REQ_ADULT_POWDER_KEG,         CFG_MM_AGE_REQ_CHILD_POWDER_KEG },
    { ITEM_MM_PICTOGRAPH_BOX,     CFG_MM_AGE_REQ_ADULT_PICTOGRAPH_BOX,     CFG_MM_AGE_REQ_CHILD_PICTOGRAPH_BOX },
    { ITEM_MM_LENS_OF_TRUTH,      CFG_MM_AGE_REQ_ADULT_LENS_OF_TRUTH,      CFG_MM_AGE_REQ_CHILD_LENS_OF_TRUTH },
    { ITEM_MM_GREAT_FAIRY_SWORD,  CFG_MM_AGE_REQ_ADULT_GREAT_FAIRY_SWORD,  CFG_MM_AGE_REQ_CHILD_GREAT_FAIRY_SWORD },

    { ITEM_MM_SWORD_KOKIRI,       CFG_MM_AGE_REQ_ADULT_SWORD_KOKIRI,       CFG_MM_AGE_REQ_CHILD_SWORD_KOKIRI },
    { ITEM_MM_SWORD_RAZOR,        CFG_MM_AGE_REQ_ADULT_SWORD_RAZOR,        CFG_MM_AGE_REQ_CHILD_SWORD_RAZOR },
    { ITEM_MM_SWORD_GILDED,       CFG_MM_AGE_REQ_ADULT_SWORD_GILDED,       CFG_MM_AGE_REQ_CHILD_SWORD_GILDED },
    { ITEM_MM_SWORD_MASTER,       CFG_MM_AGE_REQ_ADULT_SWORD_MASTER,       CFG_MM_AGE_REQ_CHILD_SWORD_MASTER },
    { ITEM_MM_SWORD_GIANTS_KNIFE, CFG_MM_AGE_REQ_ADULT_SWORD_GORON,        CFG_MM_AGE_REQ_CHILD_SWORD_GORON },
    { ITEM_MM_SWORD_BIGGORON,     CFG_MM_AGE_REQ_ADULT_SWORD_GORON,        CFG_MM_AGE_REQ_CHILD_SWORD_GORON },

    { ITEM_MM_SHIELD_DEKU,        CFG_MM_AGE_REQ_ADULT_SHIELD_DEKU,        CFG_MM_AGE_REQ_CHILD_SHIELD_DEKU },
    { ITEM_MM_SHIELD_HERO,        CFG_MM_AGE_REQ_ADULT_SHIELD_HERO,        CFG_MM_AGE_REQ_CHILD_SHIELD_HERO },
    { ITEM_MM_SHIELD_HYLIAN,      CFG_MM_AGE_REQ_ADULT_SHIELD_HYLIAN,      CFG_MM_AGE_REQ_CHILD_SHIELD_HYLIAN },
    { ITEM_MM_SHIELD_MIRROR,      CFG_MM_AGE_REQ_ADULT_SHIELD_MIRROR,      CFG_MM_AGE_REQ_CHILD_SHIELD_MIRROR },


    { ITEM_MM_HAMMER,             CFG_MM_AGE_REQ_ADULT_HAMMER,             CFG_MM_AGE_REQ_CHILD_HAMMER },
    { ITEM_MM_BOOTS_IRON,         CFG_MM_AGE_REQ_ADULT_BOOTS_IRON,         CFG_MM_AGE_REQ_CHILD_BOOTS_IRON },
    { ITEM_MM_BOOTS_HOVER,        CFG_MM_AGE_REQ_ADULT_BOOTS_HOVER,        CFG_MM_AGE_REQ_CHILD_BOOTS_HOVER },
    { ITEM_MM_TUNIC_GORON,        CFG_MM_AGE_REQ_ADULT_TUNIC_GORON,        CFG_MM_AGE_REQ_CHILD_TUNIC_GORON },
    { ITEM_MM_TUNIC_ZORA,         CFG_MM_AGE_REQ_ADULT_TUNIC_ZORA,         CFG_MM_AGE_REQ_CHILD_TUNIC_ZORA },
    { ITEM_MM_SPELL_WIND,         CFG_MM_AGE_REQ_ADULT_SPELL_WIND,         CFG_MM_AGE_REQ_CHILD_SPELL_WIND },
    { ITEM_MM_SPELL_LOVE,         CFG_MM_AGE_REQ_ADULT_SPELL_LOVE,         CFG_MM_AGE_REQ_CHILD_SPELL_LOVE },
    { ITEM_MM_SPELL_FIRE,         CFG_MM_AGE_REQ_ADULT_SPELL_FIRE,         CFG_MM_AGE_REQ_CHILD_SPELL_FIRE },
    { ITEM_MM_BOOMERANG,          CFG_MM_AGE_REQ_ADULT_BOOMERANG,          CFG_MM_AGE_REQ_CHILD_BOOMERANG },
    { ITEM_MM_SLINGSHOT,          CFG_MM_AGE_REQ_ADULT_SLINGSHOT,          CFG_MM_AGE_REQ_CHILD_SLINGSHOT },

    { ITEM_MM_MASK_DEKU,          CFG_MM_AGE_REQ_ADULT_MASK_DEKU,          CFG_MM_AGE_REQ_CHILD_MASK_DEKU },
    { ITEM_MM_MASK_GORON,         CFG_MM_AGE_REQ_ADULT_MASK_GORON,         CFG_MM_AGE_REQ_CHILD_MASK_GORON },
    { ITEM_MM_MASK_ZORA,          CFG_MM_AGE_REQ_ADULT_MASK_ZORA,          CFG_MM_AGE_REQ_CHILD_MASK_ZORA },
    { ITEM_MM_MASK_FIERCE_DEITY,  CFG_MM_AGE_REQ_ADULT_MASK_FIERCE_DEITY,  CFG_MM_AGE_REQ_CHILD_MASK_FIERCE_DEITY },
    { ITEM_MM_MASK_TRUTH,         CFG_MM_AGE_REQ_ADULT_MASK_TRUTH,         CFG_MM_AGE_REQ_CHILD_MASK_TRUTH },
    { ITEM_MM_MASK_KAFEI,         CFG_MM_AGE_REQ_ADULT_MASK_KAFEI,         CFG_MM_AGE_REQ_CHILD_MASK_KAFEI },
    { ITEM_MM_MASK_ALL_NIGHT,     CFG_MM_AGE_REQ_ADULT_MASK_ALL_NIGHT,     CFG_MM_AGE_REQ_CHILD_MASK_ALL_NIGHT },
    { ITEM_MM_MASK_BUNNY,         CFG_MM_AGE_REQ_ADULT_MASK_BUNNY,         CFG_MM_AGE_REQ_CHILD_MASK_BUNNY },
    { ITEM_MM_MASK_KEATON,        CFG_MM_AGE_REQ_ADULT_MASK_KEATON,        CFG_MM_AGE_REQ_CHILD_MASK_KEATON },
    { ITEM_MM_MASK_GARO,          CFG_MM_AGE_REQ_ADULT_MASK_GARO,          CFG_MM_AGE_REQ_CHILD_MASK_GARO },
    { ITEM_MM_MASK_ROMANI,        CFG_MM_AGE_REQ_ADULT_MASK_ROMANI,        CFG_MM_AGE_REQ_CHILD_MASK_ROMANI },
    { ITEM_MM_MASK_TROUPE_LEADER, CFG_MM_AGE_REQ_ADULT_MASK_TROUPE_LEADER, CFG_MM_AGE_REQ_CHILD_MASK_TROUPE_LEADER },
    { ITEM_MM_MASK_POSTMAN,       CFG_MM_AGE_REQ_ADULT_MASK_POSTMAN,       CFG_MM_AGE_REQ_CHILD_MASK_POSTMAN },
    { ITEM_MM_MASK_COUPLE,        CFG_MM_AGE_REQ_ADULT_MASK_COUPLE,        CFG_MM_AGE_REQ_CHILD_MASK_COUPLE },
    { ITEM_MM_MASK_GREAT_FAIRY,   CFG_MM_AGE_REQ_ADULT_MASK_GREAT_FAIRY,   CFG_MM_AGE_REQ_CHILD_MASK_GREAT_FAIRY },
    { ITEM_MM_MASK_GIBDO,         CFG_MM_AGE_REQ_ADULT_MASK_GIBDO,         CFG_MM_AGE_REQ_CHILD_MASK_GIBDO },
    { ITEM_MM_MASK_DON_GERO,      CFG_MM_AGE_REQ_ADULT_MASK_DON_GERO,      CFG_MM_AGE_REQ_CHILD_MASK_DON_GERO },
    { ITEM_MM_MASK_KAMARO,        CFG_MM_AGE_REQ_ADULT_MASK_KAMARO,        CFG_MM_AGE_REQ_CHILD_MASK_KAMARO },
    { ITEM_MM_MASK_CAPTAIN,       CFG_MM_AGE_REQ_ADULT_MASK_CAPTAIN,       CFG_MM_AGE_REQ_CHILD_MASK_CAPTAIN },
    { ITEM_MM_MASK_STONE,         CFG_MM_AGE_REQ_ADULT_MASK_STONE,         CFG_MM_AGE_REQ_CHILD_MASK_STONE },
    { ITEM_MM_MASK_BREMEN,        CFG_MM_AGE_REQ_ADULT_MASK_BREMEN,        CFG_MM_AGE_REQ_CHILD_MASK_BREMEN },
    { ITEM_MM_MASK_BLAST,         CFG_MM_AGE_REQ_ADULT_MASK_BLAST,         CFG_MM_AGE_REQ_CHILD_MASK_BLAST },
    { ITEM_MM_MASK_SCENTS,        CFG_MM_AGE_REQ_ADULT_MASK_SCENTS,        CFG_MM_AGE_REQ_CHILD_MASK_SCENTS },
    { ITEM_MM_MASK_GIANT,         CFG_MM_AGE_REQ_ADULT_MASK_GIANT,         CFG_MM_AGE_REQ_CHILD_MASK_GIANT },

    { ITEM_MM_MASK_GERUDO,        CFG_MM_AGE_REQ_ADULT_MASK_GERUDO,        CFG_MM_AGE_REQ_CHILD_MASK_GERUDO },
    { ITEM_MM_MASK_SKULL,         CFG_MM_AGE_REQ_ADULT_MASK_SKULL,         CFG_MM_AGE_REQ_CHILD_MASK_SKULL },
    { ITEM_MM_MASK_SPOOKY,        CFG_MM_AGE_REQ_ADULT_MASK_SPOOKY,        CFG_MM_AGE_REQ_CHILD_MASK_SPOOKY },

    { ITEM_MM_MOON_TEAR,          CFG_MM_AGE_REQ_ADULT_MOON_TEAR,          CFG_MM_AGE_REQ_CHILD_MOON_TEAR },
    { ITEM_MM_DEED_LAND,          CFG_MM_AGE_REQ_ADULT_DEED_LAND,          CFG_MM_AGE_REQ_CHILD_DEED_LAND },
    { ITEM_MM_DEED_SWAMP,         CFG_MM_AGE_REQ_ADULT_DEED_SWAMP,         CFG_MM_AGE_REQ_CHILD_DEED_SWAMP },
    { ITEM_MM_DEED_MOUNTAIN,      CFG_MM_AGE_REQ_ADULT_DEED_MOUNTAIN,      CFG_MM_AGE_REQ_CHILD_DEED_MOUNTAIN },
    { ITEM_MM_DEED_OCEAN,         CFG_MM_AGE_REQ_ADULT_DEED_OCEAN,         CFG_MM_AGE_REQ_CHILD_DEED_OCEAN },
    { ITEM_MM_LETTER_TO_MAMA,     CFG_MM_AGE_REQ_ADULT_LETTER_TO_MAMA,     CFG_MM_AGE_REQ_CHILD_LETTER_TO_MAMA },
    { ITEM_MM_LETTER_TO_KAFEI,    CFG_MM_AGE_REQ_ADULT_LETTER_TO_KAFEI,    CFG_MM_AGE_REQ_CHILD_LETTER_TO_KAFEI },
    { ITEM_MM_PENDANT_OF_MEMORIES,CFG_MM_AGE_REQ_ADULT_PENDANT_OF_MEMORIES,CFG_MM_AGE_REQ_CHILD_PENDANT_OF_MEMORIES },

    { ITEM_NONE, 0, 0 },
};

static const ItemAgeReqConfig kOotItemAgeReqConfigs[] =
{
    { ITEM_NONE, 0, 0 },
};


static ComboAgeReq comboGetItemAgeReq(
    const ItemAgeReqConfig* configs,
    u8 item)
{
    const ItemAgeReqConfig* entry;

    if (item == ITEM_NONE || item >= 0xff)
        return COMBO_AGE_REQ_NONE;

    for (; configs->item != ITEM_NONE; configs++)
    {
        entry = configs;

        if (entry->item != item)
            continue;

        if (Config_Flag(entry->adultFlag))
            return COMBO_AGE_REQ_ADULT;

        if (Config_Flag(entry->childFlag))
            return COMBO_AGE_REQ_CHILD;

        return COMBO_AGE_REQ_NONE;
    }

    return COMBO_AGE_REQ_NONE;
}


int comboCheckAgeReq(ComboAgeReq req)
{
    switch (req)
    {
    case COMBO_AGE_REQ_ADULT:
        return comboIsLinkAdult();

    case COMBO_AGE_REQ_CHILD:
        return !comboIsLinkAdult();

    default:
        return 1;
    }
}


ComboAgeReq comboGetItemAgeReqMm(u8 item)
{
    return comboGetItemAgeReq(
        kMmItemAgeReqConfigs,
        item);
}


int comboCheckItemAgeReqMm(u8 item)
{
    return comboCheckAgeReq(
        comboGetItemAgeReqMm(item));
}


ComboAgeReq comboGetItemAgeReqOot(u8 item)
{
    return comboGetItemAgeReq(
        kOotItemAgeReqConfigs,
        item);
}


int comboCheckItemAgeReqOot(u8 item)
{
    return comboCheckAgeReq(
        comboGetItemAgeReqOot(item));
}