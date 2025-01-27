#ifndef COMBO_DRAWGI_H
#define COMBO_DRAWGI_H

#include <combo/drawgi_data.h>

#if !defined(__ASSEMBLER__)
# include <combo/game_state.h>
# include <combo/types.h>

typedef void (*DrawGiFunc)(PlayState* play, s16 index, u8 param);

typedef struct PACKED ALIGNED(4)
{
    DrawGiFunc  func;
    u32         lists[8];
}
DrawGi;

extern const DrawGi kDrawGi[];

/* Vanilla draw funcs */
void DrawGi_Opa0_Xlu1(PlayState*, s16);
void DrawGi_Opa0_Xlu12(PlayState*, s16);
void DrawGi_Opa0(PlayState*, s16);
void DrawGi_Opa01(PlayState*, s16);
void DrawGi_Opa1023(PlayState*, s16);
void DrawGi_Opa10_Xlu2(PlayState*, s16);
void DrawGi_Opa10_Xlu234(PlayState*, s16);
void DrawGi_Opa10_Xlu32(PlayState*, s16);
void DrawGi_Opa10234567(PlayState*, s16);
void DrawGi_Xlu01(PlayState*, s16);
void DrawGi_BlueFire(PlayState*, s16);
void DrawGi_BombchuMask(PlayState*, s16);
void DrawGi_Compass(PlayState*, s16);
void DrawGi_DekuNut(PlayState*, s16);
void DrawGi_Fairy(PlayState*, s16);
void DrawGi_Fish(PlayState*, s16);
void DrawGi_GiantKnife(PlayState*, s16);
void DrawGi_GS(PlayState*, s16);
void DrawGi_Heart(PlayState*, s16);
void DrawGi_Medallion(PlayState*, s16);
void DrawGi_MirrorShield(PlayState*, s16);
void DrawGi_Poe(PlayState*, s16);
void DrawGi_Potion(PlayState*, s16);
void DrawGi_Scale(PlayState*, s16);
void DrawGi_SoldOut(PlayState*, s16);
void DrawGi_Spell(PlayState*, s16);
void DrawGi_MoonTear(PlayState*, s16);
void DrawGi_BottleFairy(PlayState*, s16);
void DrawGi_BottleBlueFire(PlayState*, s16);
void DrawGi_VanillaRupee(PlayState*, s16);

/* Custom draw funcs */
void DrawGi_Xlu0(PlayState* play, s16 drawGiId);
void DrawGi_CustomNote(PlayState* play, s16 drawGiId, u8 param);
void DrawGi_CustomHeartContainer(PlayState* play, s16 drawGiId);
void DrawGi_CustomStick(PlayState* play, s16 drawGiId);
void DrawGi_CustomNut(PlayState* play, s16 drawGiId);
void DrawGi_BossRemains(PlayState* play, s16 drawGiId);
void DrawGi_SpiritualStones(PlayState* play, s16 drawGiId);
void DrawGi_MasterSword(PlayState* play, s16 drawGiId);
void DrawGi_CustomSpin(PlayState* play, s16 drawGiId, u8 param);
void DrawGi_CustomOwl(PlayState* play, s16 drawGiId);
void DrawGi_CustomStrayFairy(PlayState* play, s16 drawGiId);
void DrawGi_CustomBottlePotion(PlayState* play, s16 index, u8 param);
void DrawGi_CustomGS(PlayState* play, s16 index);
void DrawGi_CustomPotion(PlayState* play, s16 index);
void DrawGi_SilverRupee(PlayState* play, s16 index);
void DrawGi_MagicalRupee(PlayState* play, s16 index);
void DrawGi_RutosLetter(PlayState* play, s16 index);
void DrawGi_CustomSmallKey(PlayState* play, s16 index);
void DrawGi_Coin(PlayState* play, s16 index);
void DrawGi_MagicJar(PlayState* play, s16 index);
void DrawGi_Triforce(PlayState* play, s16 index, u8 param);
void DrawGi_TriforceFull(PlayState* play, s16 index, u8 param);
void DrawGi_MaskMajora(PlayState* play, s16 index, u8 param);
void DrawGi_Wallet(PlayState* play, s16 index);
void DrawGi_Button(PlayState* play, s16 index);
void DrawGi_CustomBottleFairy(PlayState* play, s16 index, u8 param);
void DrawGi_BombchuBag(PlayState* play, s16 index, u8 param);
void DrawGi_BigFairy(PlayState* play, s16 index);
void DrawGi_Clock(PlayState*, s16);
void DrawGi_Rupee(PlayState*, s16 index, u8 param);
void DrawGi_PondFish(PlayState*, s16 index, u8 param);
void DrawGi_CustomShield(PlayState* play, s16 drawGiId, u8 param);
void DrawGi_TrapIce(PlayState* play, s16 drawGiId, u8 param);

#endif

#endif
