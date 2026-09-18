#include <combo.h>
#include <combo/config.h>
#include <combo/inventory.h>

static MmSwordId sMmSwordSelected = MM_SWORD_NONE;
static MmShieldId sMmShieldSelected = MM_SHIELD_NONE;
static MmShieldId sMmShieldLost = MM_SHIELD_NONE;
s32 MmSword_IsOwned(MmSwordId sword)
{
    switch (sword)
    {
        case MM_SWORD_KOKIRI:
            return gSharedCustomSave.mm.swords.sword >= 1;

        case MM_SWORD_RAZOR:
            return gSharedCustomSave.mm.swords.sword >= 2;

        case MM_SWORD_GILDED:
            return gSharedCustomSave.mm.swords.sword >= 3;

        case MM_SWORD_MASTER:
            return gSharedCustomSave.mm.swords.masterSword;

        case MM_SWORD_GIANTS_KNIFE:
            return gSharedCustomSave.mm.swords.giantSword == 1;

        case MM_SWORD_BIGGORON:
            return gSharedCustomSave.mm.swords.giantSword >= 2;

        default:
            return 0;
    }
}

void MmSword_EnsureState(void)
{
    MmSwordId first = MM_SWORD_NONE;
    MmSwordId equipped;
    s32 age;

    switch (gSharedCustomSave.mm.swords.sword)
    {
        case 3:
            first = MM_SWORD_GILDED;
            break;

        case 2:
            first = MM_SWORD_RAZOR;
            break;

        case 1:
            first = MM_SWORD_KOKIRI;
            break;

        default:
            break;
    }

    if (first == MM_SWORD_NONE &&
        gSharedCustomSave.mm.swords.masterSword)
    {
        first = MM_SWORD_MASTER;
    }

    if (first == MM_SWORD_NONE)
    {
        switch (gSharedCustomSave.mm.swords.giantSword)
        {
            case 2:
                first = MM_SWORD_BIGGORON;
                break;

            case 1:
                first = MM_SWORD_GIANTS_KNIFE;
                break;

            default:
                break;
        }
    }

    for (age = 0; age < 2; age++)
    {
        MmSwordId sword =
            (MmSwordId)gSharedCustomSave.mm.humanAgeLoadouts[age].sword;

        if (!MmSword_IsOwned(sword))
            gSharedCustomSave.mm.humanAgeLoadouts[age].sword =
                MM_SWORD_NONE;
    }

    if (first == MM_SWORD_NONE)
    {
        sMmSwordSelected = MM_SWORD_NONE;
        return;
    }

    if (!MmSword_IsOwned(sMmSwordSelected))
    {
        equipped =
            (MmSwordId)gSharedCustomSave.mm
                .humanAgeLoadouts[gMmSave.linkAge].sword;

        sMmSwordSelected =
            MmSword_IsOwned(equipped) ? equipped : first;
    }
}

MmSwordId MmSword_GetSelected(void)
{
    MmSword_EnsureState();
    return sMmSwordSelected;
}

MmSwordId MmSword_GetEquipped(void)
{
    MmSword_EnsureState();
    return (MmSwordId)gSharedCustomSave.mm.humanAgeLoadouts[gMmSave.linkAge].sword;
}

void MmSword_SetSelected(MmSwordId sword)
{
    if (MmSword_IsOwned(sword))
        sMmSwordSelected = sword;
}

MmSwordId MmSword_GetNextOwned(MmSwordId current)
{
    MmSwordId sword = current;

    for (s32 i = 0; i < MM_SWORD_MAX; i++)
    {
        sword++;

        if (sword >= MM_SWORD_MAX)
            sword = MM_SWORD_KOKIRI;

        if (MmSword_IsOwned(sword))
            return sword;
    }

    return current;
}

u16 MmSword_GetGiantsKnifeHealth(void)
{
    if (Config_Flag(CFG_SHARED_GORON_SWORDS))
        return gOotSave.info.playerData.swordHealth;

    return gSharedCustomSave.mm.mmGiantsKnifeHealth;
}

static void SyncOotGiantsKnifeEquip(OotItemEquips* equips, u16 health)
{
    if (health == 0)
    {
        if (equips->buttonItems[0] == ITEM_OOT_SWORD_KNIFE_BIGGORON)
            equips->buttonItems[0] = ITEM_OOT_SWORD_KNIFE_BROKEN;
    }
    else
    {
        if (equips->buttonItems[0] == ITEM_OOT_SWORD_KNIFE_BROKEN)
            equips->buttonItems[0] = ITEM_OOT_SWORD_KNIFE_BIGGORON;
    }
}

void MmSword_SetGiantsKnifeHealth(u16 health)
{
    if (Config_Flag(CFG_SHARED_GORON_SWORDS))
    {
        gOotSave.info.playerData.swordHealth = health;

        if (health == 0)
        {
            gOotSave.info.inventory.equipment.swords |=
                EQ_OOT_SWORD_KNIFE_BROKEN;
        }
        else
        {
            gOotSave.info.inventory.equipment.swords &=
                ~EQ_OOT_SWORD_KNIFE_BROKEN;
        }
        SyncOotGiantsKnifeEquip(&gOotSave.info.equips, health);
        SyncOotGiantsKnifeEquip(&gOotSave.info.adultEquips, health);
        SyncOotGiantsKnifeEquip(&gOotSave.info.childEquips, health);

        return;
    }

    gSharedCustomSave.mm.mmGiantsKnifeHealth = health;
}

void MmSword_Equip(PlayState* play, MmSwordId sword)
{
    if (!MmSword_IsOwned(sword))
        return;

    sMmSwordSelected = sword;
    gSharedCustomSave.mm.humanAgeLoadouts[gMmSave.linkAge].sword = (u8)sword;
    MmSword_RefreshNativeEquip(play);
}

s32 MmShield_IsOwned(MmShieldId shield)
{
    if (shield <= MM_SHIELD_NONE || shield >= MM_SHIELD_MAX)
        return 0;

    return !!(gSharedCustomSave.mm.shieldsOwned & (1u << (shield - 1)));
}

void MmShield_EnsureState(void)
{
    MmShieldId first = MM_SHIELD_NONE;
    MmShieldId equipped;
    s32 age;

    for (MmShieldId shield = MM_SHIELD_DEKU; shield < MM_SHIELD_MAX; shield++)
    {
        if (MmShield_IsOwned(shield))
        {
            first = shield;
            break;
        }
    }

    for (age = 0; age < 2; age++)
    {
        MmShieldId shield = (MmShieldId)gSharedCustomSave.mm.humanAgeLoadouts[age].shield;

        if (!MmShield_IsOwned(shield))
            gSharedCustomSave.mm.humanAgeLoadouts[age].shield = MM_SHIELD_NONE;
    }

    if (first == MM_SHIELD_NONE)
    {
        sMmShieldSelected = MM_SHIELD_NONE;
        return;
    }

    if (!MmShield_IsOwned(sMmShieldSelected))
    {
        equipped = (MmShieldId)gSharedCustomSave.mm.humanAgeLoadouts[gMmSave.linkAge].shield;
        sMmShieldSelected = MmShield_IsOwned(equipped) ? equipped : first;
    }
}

MmShieldId MmShield_GetSelected(void)
{
    MmShield_EnsureState();
    return sMmShieldSelected;
}

MmShieldId MmShield_GetEquipped(void)
{
    MmShield_EnsureState();
    return (MmShieldId)gSharedCustomSave.mm.humanAgeLoadouts[gMmSave.linkAge].shield;
}

MmShieldId MmShield_GetLost(void)
{
    if ((gSharedCustomSave.mmProgressiveShields & 1) &&
        !MmShield_IsOwned(MM_SHIELD_DEKU))
    {
        return MM_SHIELD_DEKU;
    }

    if ((gSharedCustomSave.mmProgressiveShields & 2) &&
        !MmShield_IsOwned(MM_SHIELD_HERO))
    {
        return MM_SHIELD_HERO;
    }

    if ((gSharedCustomSave.mmProgressiveShields & 2) &&
        Config_Flag(CFG_MM_HYLIAN_SHIELD) &&
        !MmShield_IsOwned(MM_SHIELD_HYLIAN))
    {
        return MM_SHIELD_HYLIAN;
    }

    return MM_SHIELD_NONE;
}

void MmShield_SetSelected(MmShieldId shield)
{
    if (MmShield_IsOwned(shield))
        sMmShieldSelected = shield;
}

MmShieldId MmShield_GetNextOwned(MmShieldId current)
{
    MmShieldId shield = current;

    for (s32 i = 0; i < MM_SHIELD_MAX; i++)
    {
        shield++;

        if (shield >= MM_SHIELD_MAX)
            shield = MM_SHIELD_DEKU;

        if (MmShield_IsOwned(shield))
            return shield;
    }

    return current;
}

void MmShield_RefreshNativeEquip(PlayState* play)
{
    MmShieldId shield = MmShield_GetEquipped();

    switch (shield)
    {
    case MM_SHIELD_DEKU:
    case MM_SHIELD_HERO:
    case MM_SHIELD_HYLIAN:
        gMmSave.info.itemEquips.shield = 1;
        break;

    case MM_SHIELD_MIRROR:
        gMmSave.info.itemEquips.shield = 2;
        break;

    default:
        gMmSave.info.itemEquips.shield = 0;
        break;
    }

#if defined(GAME_MM)
    if (play)
    {
        Player* player = GET_PLAYER(play);

        if (player)
            UpdateEquipment(play, player);
    }
#endif
}

void MmSword_RefreshNativeEquip(PlayState* play)
{
    MmSwordId sword = MmSword_GetEquipped();
    u8 itemId;

    switch (sword)
    {
    case MM_SWORD_KOKIRI:
        gMmSave.info.itemEquips.sword = 1;
        itemId = ITEM_MM_SWORD_KOKIRI;
        break;

    case MM_SWORD_RAZOR:
        gMmSave.info.itemEquips.sword = 2;
        itemId = ITEM_MM_SWORD_RAZOR;
        break;

    case MM_SWORD_GILDED:
        gMmSave.info.itemEquips.sword = 3;
        itemId = ITEM_MM_SWORD_GILDED;
        break;

    case MM_SWORD_MASTER:
        gMmSave.info.itemEquips.sword = 0;
        itemId = ITEM_MM_SWORD_MASTER;
        break;

    case MM_SWORD_GIANTS_KNIFE:
        gMmSave.info.itemEquips.sword = 0;
        itemId = ITEM_MM_SWORD_GIANTS_KNIFE;
        break;

    case MM_SWORD_BIGGORON:
        gMmSave.info.itemEquips.sword = 0;
        itemId = ITEM_MM_SWORD_BIGGORON;
        break;

    default:
        gMmSave.info.itemEquips.sword = 0;
        itemId = ITEM_NONE;
        break;
    }

    gMmSave.info.itemEquips.buttonItems[0][EQUIP_SLOT_B] = itemId;

#if defined(GAME_MM)
    if (play)
    {
        Player* player = GET_PLAYER(play);

        if (player)
        {
            UpdateEquipment(play, player);
            Interface_LoadItemIconImpl(play, EQUIP_SLOT_B);
        }
    }
#endif
}

void MmShield_Equip(PlayState* play, MmShieldId shield)
{
    if (!MmShield_IsOwned(shield))
        return;

    sMmShieldSelected = shield;
    gSharedCustomSave.mm.humanAgeLoadouts[gMmSave.linkAge].shield = (u8)shield;
    MmShield_RefreshNativeEquip(play);
}

void MmShield_Lose(PlayState* play, MmShieldId shield)
{
    s32 age;

    if (!MmShield_IsOwned(shield))
        return;

    sMmShieldLost = shield;

    gSharedCustomSave.mm.shieldsOwned &= ~(1u << (shield - 1));

    for (age = 0; age < 2; age++)
    {
        if (gSharedCustomSave.mm.humanAgeLoadouts[age].shield == (u8)shield)
            gSharedCustomSave.mm.humanAgeLoadouts[age].shield = MM_SHIELD_NONE;
    }

    if (sMmShieldSelected == shield)
        sMmShieldSelected = MM_SHIELD_NONE;

    if (shield == MM_SHIELD_DEKU &&
        Config_Flag(CFG_MM_PROGRESSIVE_SHIELDS) &&
        (gSharedCustomSave.mmProgressiveShields & 1))
    {
        gSharedCustomSave.mm.shieldsOwned |= 1u << (MM_SHIELD_DEKU - 1);
    }

    MmShield_EnsureState();
    MmShield_RefreshNativeEquip(play);
}

void Inventory_ReobtainProgressiveShields(void)
{
    s32 ootProgressive = Config_Flag(CFG_OOT_PROGRESSIVE_SHIELDS);
    s32 mmProgressive = Config_Flag(CFG_MM_PROGRESSIVE_SHIELDS);

    if (!ootProgressive && !mmProgressive)
        return;

    if (ootProgressive)
    {
        if (gOotExtraItems.shield & EQ_OOT_SHIELD_DEKU)
            gOotSave.info.inventory.equipment.shields |= EQ_OOT_SHIELD_DEKU;

        if (gOotExtraItems.shield & EQ_OOT_SHIELD_HYLIAN)
        {
            gOotSave.info.inventory.equipment.shields |= EQ_OOT_SHIELD_HYLIAN;

            if (Config_Flag(CFG_OOT_HERO_SHIELD))
            {
                gSharedCustomSave.ootChildShields |= 1u << OOT_CHILD_SHIELD_HERO;
                gOotSave.info.inventory.equipment.shields |= EQ_OOT_SHIELD_DEKU;
            }
        }
    }

    if (mmProgressive)
    {
        if (gSharedCustomSave.mmProgressiveShields & 1)
            gSharedCustomSave.mm.shieldsOwned |= 1u << (MM_SHIELD_DEKU - 1);

        if (gSharedCustomSave.mmProgressiveShields & 2)
        {
            gSharedCustomSave.mm.shieldsOwned |= 1u << (MM_SHIELD_HERO - 1);

            if (Config_Flag(CFG_MM_HYLIAN_SHIELD))
                gSharedCustomSave.mm.shieldsOwned |= 1u << (MM_SHIELD_HYLIAN - 1);
        }
    }

    MmShield_EnsureState();
    MmShield_RefreshNativeEquip(NULL);
}

int comboHasFreeBottleOot(void)
{
    for (int i = 0; i < 4; ++i)
    {
        if (gOotSave.info.inventory.items[ITS_OOT_BOTTLE + i] == ITEM_OOT_BOTTLE_EMPTY)
            return 1;
    }
    if ((gOotExtraTrade.adult & (1 << XITEM_OOT_ADULT_BOTTLE)) && gOotExtraItems.bottleAdultSlot == ITEM_OOT_BOTTLE_EMPTY)
    {
        return 1;
    }
    if ((gOotExtraTrade.child & (1 << XITEM_OOT_CHILD_BOTTLE)) && gOotExtraItems.bottleChildSlot == ITEM_OOT_BOTTLE_EMPTY)
    {
        return 1;
    }
    return 0;
}

#if defined(GAME_OOT)
PATCH_FUNC(0x80071A94, comboHasFreeBottleOot);
#endif

int comboHasFreeBottleMm(void)
{
    for (int i = 0; i < 6; ++i)
    {
        if (gMmSave.info.inventory.items[ITS_MM_BOTTLE + i] == ITEM_MM_BOTTLE_EMPTY)
            return 1;
    }
    return 0;
}

u8 gOotChildShieldVariant = 0xff;

u8 OotChildShield_GetVariant(void)
{
    u8 owned;

    owned = gSharedCustomSave.ootChildShields;
    if (owned == 0 &&
        (gOotSave.info.inventory.equipment.shields &
         EQ_OOT_SHIELD_DEKU))
    {
        owned = 1 << OOT_CHILD_SHIELD_DEKU;
        gSharedCustomSave.ootChildShields = owned;
    }

    if (gOotChildShieldVariant >= OOT_CHILD_SHIELD_MAX ||
        !(owned & (1u << gOotChildShieldVariant)))
    {
        if (owned & (1u << OOT_CHILD_SHIELD_HERO))
            gOotChildShieldVariant = OOT_CHILD_SHIELD_HERO;
        else
            gOotChildShieldVariant = OOT_CHILD_SHIELD_DEKU;
    }

    return gOotChildShieldVariant;
}

u8 OotChildShield_GetNextOwned(u8 current)
{
    u8 owned;
    u8 next;
    s32 i;

    owned = gSharedCustomSave.ootChildShields;
    next = current;

    for (i = 0; i < OOT_CHILD_SHIELD_MAX; i++)
    {
        next++;

        if (next >= OOT_CHILD_SHIELD_MAX)
            next = 0;

        if (owned & (1u << next))
            return next;
    }

    return current;
}