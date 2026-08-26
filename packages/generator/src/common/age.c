#include <combo.h>
#include <combo/age.h>
#include <combo/inventory.h>

static void Age_SwapFaroreOot(void)
{
    OotFaroreWind* current;
    OotFaroreWind* prev;
    OotFaroreWind tmp;

    current = &gOotSave.info.fw;
    prev = current - 1;

    memcpy(&tmp, current, sizeof(tmp));
    memcpy(current, prev, sizeof(tmp));
    memcpy(prev, &tmp, sizeof(tmp));
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
