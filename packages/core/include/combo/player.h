#ifndef COMBO_PLAYER_H
#define COMBO_PLAYER_H

#include <combo/types.h>

#if defined(GAME_OOT)
# include <combo/oot/player.h>
#endif

#if defined(GAME_MM)
# include <combo/mm/player.h>
#endif

#define PLAYER_ACTOR_STATE_TRANSITION           0x00000001
#define PLAYER_ACTOR_STATE_TRANSFORM            0x00000002
#define PLAYER_ACTOR_STATE_CLIMB                0x00000004
#define PLAYER_ACTOR_STATE_DEATH                0x00000080
#define PLAYER_ACTOR_STATE_FROZEN               0x00000200
#define PLAYER_ACTOR_STATE_GET_ITEM             0x00000400
#define PLAYER_ACTOR_STATE_HOLD_ITEM            0x00000800
#define PLAYER_ACTOR_STATE_CLIMB2               0x00200000
#define PLAYER_ACTOR_STATE_EPONA                0x00800000
#define PLAYER_ACTOR_STATE_WATER                0x08000000
#define PLAYER_ACTOR_STATE_USE_ITEM             0x10000000
#define PLAYER_ACTOR_STATE_CUTSCENE_FROZEN      0x20000000

void Player_Update(Actor_Player* this, GameState_Play* play);
void Player_SetModels(Actor_Player* link, s32 modelGroup);
int Player_UsingItem(Actor_Player* link);
int Player_GetEnvironmentalHazard(GameState_Play* play);
int Player_InCsMode(GameState_Play* play);
void Player_Freeze(GameState_Play* play);
void Player_Unfreeze(GameState_Play* play);
void Player_UseItem(GameState_Play* play, Actor_Player* link, s16 itemId);
s32 Player_ActionToModelGroup(Actor_Player* link, s32 itemAction);
void Player_PlaySfx(Actor_Player* player, u16 sfxId);
float Player_GetHeight(Actor_Player* player);
int Player_OverrideLimbDrawGameplayDefault(GameState_Play* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor_Player* player);
int Player_OverrideLimbDrawPause(GameState_Play* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor_Player* player);
int Player_OverrideLimbDrawGameplayFirstPerson(GameState_Play* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void*);
void Player_SetBootData(GameState_Play* play, Actor_Player* player);
int Player_GetStrength(void);

#if defined(GAME_MM)
void Player_DrawHookshotReticle(GameState_Play* play, Actor_Player* player, f32 distance);
void Player_UpdateForm(Actor_Player* this, GameState_Play* play);
void Player_TryUpdateForm(Actor_Player* this, GameState_Play* play);
#endif

#endif
