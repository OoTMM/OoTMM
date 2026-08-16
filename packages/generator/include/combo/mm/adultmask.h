#ifndef COMBO_MM_ADULT_MASK_H
#define COMBO_MM_ADULT_MASK_H

#include <combo/types.h>
#include "play.h"
#include "player.h"

s32 AdultMask_TryUse(Player* player, PlayState* play, s32 itemAction);
s32 AdultMask_IsCsItem(Player* player);
void AdultMask_StartCsItem(Player* player, PlayState* play);
void AdultMask_AfterStart(Player* player);
s32 AdultMask_IsPuttingOn(void);
s32 AdultMask_IsTakeOffTransform(void);
s32 AdultMask_IsTakeOffChild(void);
s32 AdultMask_ShouldDrawTransformFace(void);
s32 AdultMask_GetTimer(void);
void AdultMask_DrawTransformationMaskOnFace(PlayState* play, Player* player);
void AdultMask_DrawMaskOnFaceNativeLike(PlayState* play, Player* player);
void AdultMask_DrawMaskInHand(PlayState* play, Player* player);
void AdultMask_DrawTransformRing(PlayState* play, Player* player);

typedef void (*PlayerActionFunc)(Player*, PlayState*);
typedef void (*Player_SetAction_PreserveItemActionFunc)(PlayState*, Player*, PlayerActionFunc, s32);
typedef void (*Player_BeforeCsActionFunc)(PlayState*, Player*);
typedef void (*Player_ClearAttentionAndCameraModeFunc)(Player*);
typedef void (*Player_UpdateUpperBodyOrHeldItemFunc)(Player*);
typedef void (*Player_DecelerateToZeroFunc)(Player*);
typedef void (*Player_ReturnToDefaultActionFunc)(Player*, PlayState*);
typedef void (*Player_StopCutsceneFunc)(Player*);
typedef void (*Player_Anim_PlayOnceAdjustedFunc)(PlayState*, Player*, PlayerAnimationHeader*);
typedef void (*Player_Anim_PlayOnceMorphAdjustedFunc)(PlayState*, Player*, PlayerAnimationHeader*);
typedef void (*Player_StartMaskTransformCsFunc)(Player*, s16);
typedef PlayerAnimationHeader* (*Player_GetIdleAnimFunc)(Player*);

typedef struct struct_8085D910 { u8 unk_0, unk_1, unk_2, unk_3; } struct_8085D910;
typedef void (*Player_UpdateMaskTransformEffectsFunc)(PlayState*, Player*, struct_8085D910**);
typedef void (*Player_DrawMaskTransformEffectsFunc)(PlayState*, Player*, f32, f32, s32);

#endif