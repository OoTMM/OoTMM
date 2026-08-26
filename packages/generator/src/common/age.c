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

static void Age_OnChangeOot(void)
{
    Age_SwapFaroreOot();
    Age_SwapEquipmentOot();
}

#if defined(GAME_OOT)
PATCH_FUNC(0x8006f804, Age_OnChangeOot);
#endif

void Age_SetOot(PlayState* play, int age)
{
    if (gOotSave.age == age)
        return;

    gSharedCustomSave.hasBeenChildAndAdult = 1;
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

void Age_SwapOot(PlayState* play)
{
    Age_SetOot(play,  1 - gOotSave.age);
}
