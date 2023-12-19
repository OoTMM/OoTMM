#ifndef COMBO_DRAWGI_H
#define COMBO_DRAWGI_H

#include <combo/drawgi_data.h>

#if !defined(__ASSEMBLER__)
# include <combo/game_state.h>
# include <combo/types.h>

typedef void (*DrawGiFunc)(GameState_Play* play, s16 index, u8 param);

typedef struct PACKED ALIGNED(4)
{
    DrawGiFunc  func;
    u32         lists[8];
}
DrawGi;

extern const DrawGi kDrawGi[];

/* Custom draw funcs */
void DrawGi_Xlu0(GameState_Play* play, s16 drawGiId);
void DrawGi_CustomNote(GameState_Play* play, s16 drawGiId, u8 param);
void DrawGi_CustomHeartContainer(GameState_Play* play, s16 drawGiId);
void DrawGi_CustomStick(GameState_Play* play, s16 drawGiId);
void DrawGi_CustomNut(GameState_Play* play, s16 drawGiId);
void DrawGi_BossRemains(GameState_Play* play, s16 drawGiId);
void DrawGi_SpiritualStones(GameState_Play* play, s16 drawGiId);
void DrawGi_MasterSword(GameState_Play* play, s16 drawGiId);
void DrawGi_CustomSpin(GameState_Play* play, s16 drawGiId);
void DrawGi_CustomOwl(GameState_Play* play, s16 drawGiId);
void DrawGi_CustomStrayFairy(GameState_Play* play, s16 drawGiId);
void DrawGi_CustomBottlePotion(GameState_Play* play, s16 index, u8 param);
void DrawGi_CustomGS(GameState_Play* play, s16 index);
void DrawGi_CustomPotion(GameState_Play* play, s16 index);
void DrawGi_SilverRupee(GameState_Play* play, s16 index);
void DrawGi_MagicalRupee(GameState_Play* play, s16 index);
void DrawGi_RutosLetter(GameState_Play* play, s16 index);
void DrawGi_CustomSmallKey(GameState_Play* play, s16 index);
void DrawGi_Coin(GameState_Play* play, s16 index);
void DrawGi_MagicJar(GameState_Play* play, s16 index);
void DrawGi_Triforce(GameState_Play* play, s16 index, u8 param);
void DrawGi_Wallet(GameState_Play* play, s16 index);
void DrawGi_Button(GameState_Play* play, s16 index);

#endif

#endif
