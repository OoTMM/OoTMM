#ifndef COMBO_INVENTORY_H
#define COMBO_INVENTORY_H

#include <combo/types.h>

s32 comboGetSlotExtras(u32 slot, u8** outItemPtr, u32* outFlags, const u8** outTable, u32* outTableSize);
u8 comboGetNextTrade(u8 currentItem, u32 flags, const u8* table, u32 tableSize);
void comboToggleSlot(u32 slot);
void comboToggleTrade(u8* slot, u32 flags, const u8* table, u32 tableSize);
s32 comboIsTradeBottleOot(u8 itemId);

int comboHasFreeBottleOot(void);
int comboHasFreeBottleMm(void);

#if defined(GAME_OOT)
void comboRemoveTradeItemAdult(u16 xitemId);
void comboRemoveTradeItemChild(u16 xitemId);
#endif

#if defined(GAME_MM)
void comboRemoveTradeItem1(u16 xitemId);
void comboRemoveTradeItem2(u16 xitemId);
void comboRemoveTradeItem3(u16 xitemId);
void Inventory_SaveLotteryCodeGuess(PlayState* play);
#endif

    void Inventory_ReobtainProgressiveShields(void);

typedef enum
{
    MM_SWORD_NONE = 0,
    MM_SWORD_KOKIRI,
    MM_SWORD_RAZOR,
    MM_SWORD_GILDED,
    MM_SWORD_MASTER,
    MM_SWORD_GIANTS_KNIFE,
    MM_SWORD_BIGGORON,
    MM_SWORD_MAX,
}
MmSwordId;

s32 MmSword_IsOwned(MmSwordId sword);
void MmSword_EnsureState(void);

MmSwordId MmSword_GetSelected(void);
MmSwordId MmSword_GetEquipped(void);
MmSwordId MmSword_GetNextOwned(MmSwordId sword);

void MmSword_SetSelected(MmSwordId sword);
void MmSword_Equip(PlayState* play, MmSwordId sword);

u16 MmSword_GetGiantsKnifeHealth(void);
void MmSword_SetGiantsKnifeHealth(u16 health);

void MmSword_RefreshNativeEquip(PlayState* play);
void MmSword_RefreshHudIcon(PlayState* play);

typedef enum
{
    MM_SHIELD_NONE = 0,
    MM_SHIELD_DEKU,
    MM_SHIELD_HERO,
    MM_SHIELD_HYLIAN,
    MM_SHIELD_MIRROR,
    MM_SHIELD_MAX,
}
MmShieldId;

s32 MmShield_IsOwned(MmShieldId shield);
void MmShield_EnsureState(void);

MmShieldId MmShield_GetSelected(void);
MmShieldId MmShield_GetEquipped(void);
MmShieldId MmShield_GetLost(void);
MmShieldId MmShield_GetNextOwned(MmShieldId shield);

void MmShield_SetSelected(MmShieldId shield);
void MmShield_Equip(PlayState* play, MmShieldId shield);
void MmShield_Lose(PlayState* play, MmShieldId shield);
void MmShield_Unequip(PlayState* play);
void MmShield_RefreshNativeEquip(PlayState* play);

s32 Inventory_GetBtnBItem(struct PlayState* play);

typedef enum
{
    OOT_CHILD_SHIELD_DEKU = 0,
    OOT_CHILD_SHIELD_HERO,
    OOT_CHILD_SHIELD_MAX,
}
OotChildShieldId;

#if defined(GAME_OOT)

extern u8 gOotChildSwordVariant;
extern u8 gOotChildSwordEquippedVariant;

u8 OotChildSword_GetVariant(void);
u8 OotChildSword_GetEquippedVariant(void);

extern u8 gOotChildShieldVariant;
extern u8 gOotChildShieldEquippedVariant;

u8 OotChildShield_GetVariant(void);
u8 OotChildShield_GetEquippedVariant(void);
u8 OotChildShield_GetNextOwned(u8 current);
#endif

#endif
