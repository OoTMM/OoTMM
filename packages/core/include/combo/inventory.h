#ifndef COMBO_INVENTORY_H
#define COMBO_INVENTORY_H

#include <combo/types.h>

s32 comboGetSlotExtras(u32 slot, u8** outItemPtr, u32* outFlags, const u8** outTable, u32* outTableSize);
u8 comboGetNextTrade(u8 currentItem, u32 flags, const u8* table, u32 tableSize);
void comboToggleSlot(u32 slot);
void comboToggleTrade(u8* slot, u32 flags, const u8* table, u32 tableSize);

#if defined(GAME_OOT)
void comboRemoveTradeItemAdult(u16 xitemId);
void comboRemoveTradeItemChild(u16 xitemId);
#endif

#if defined(GAME_MM)
void comboRemoveTradeItem1(u16 xitemId);
void comboRemoveTradeItem2(u16 xitemId);
void comboRemoveTradeItem3(u16 xitemId);
#endif

#endif
