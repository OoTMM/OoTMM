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

/* Vanilla draw funcs */
void DrawGi_Opa0_Xlu1(GameState_Play*, s16);
void DrawGi_Opa0_Xlu12(GameState_Play*, s16);
void DrawGi_Opa0(GameState_Play*, s16);
void DrawGi_Opa01(GameState_Play*, s16);
void DrawGi_Opa1023(GameState_Play*, s16);
void DrawGi_Opa10_Xlu2(GameState_Play*, s16);
void DrawGi_Opa10_Xlu234(GameState_Play*, s16);
void DrawGi_Opa10_Xlu32(GameState_Play*, s16);
void DrawGi_Opa10234567(GameState_Play*, s16);
void DrawGi_Xlu01(GameState_Play*, s16);
void DrawGi_BlueFire(GameState_Play*, s16);
void DrawGi_BombchuMask(GameState_Play*, s16);
void DrawGi_Compass(GameState_Play*, s16);
void DrawGi_DekuNut(GameState_Play*, s16);
void DrawGi_Fairy(GameState_Play*, s16);
void DrawGi_Fish(GameState_Play*, s16);
void DrawGi_GiantKnife(GameState_Play*, s16);
void DrawGi_GS(GameState_Play*, s16);
void DrawGi_Heart(GameState_Play*, s16);
void DrawGi_Medallion(GameState_Play*, s16);
void DrawGi_MirrorShield(GameState_Play*, s16);
void DrawGi_Poe(GameState_Play*, s16);
void DrawGi_Potion(GameState_Play*, s16);
void DrawGi_Scale(GameState_Play*, s16);
void DrawGi_SoldOut(GameState_Play*, s16);
void DrawGi_Spell(GameState_Play*, s16);
void DrawGi_MoonTear(GameState_Play*, s16);
void DrawGi_BottleFairy(GameState_Play*, s16);
void DrawGi_BottleBlueFire(GameState_Play*, s16);
void DrawGi_VanillaRupee(GameState_Play*, s16);

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
void DrawGi_TriforceFull(GameState_Play* play, s16 index, u8 param);
void DrawGi_MaskMajora(GameState_Play* play, s16 index, u8 param);
void DrawGi_Wallet(GameState_Play* play, s16 index);
void DrawGi_Button(GameState_Play* play, s16 index);
void DrawGi_CustomBottleFairy(GameState_Play* play, s16 index, u8 param);
void DrawGi_BombchuBag(GameState_Play* play, s16 index, u8 param);
void DrawGi_BigFairy(GameState_Play* play, s16 index);
void DrawGi_Clock(GameState_Play*, s16);
void DrawGi_Rupee(GameState_Play*, s16 index, u8 param);
void DrawGi_PondFish(GameState_Play*, s16 index, u8 param);

#endif

#endif
