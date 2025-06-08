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
#define PLAYER_ACTOR_STATE_TALKING              0x00000040
#define PLAYER_ACTOR_STATE_DEATH                0x00000080
#define PLAYER_ACTOR_STATE_FROZEN               0x00000200
#define PLAYER_ACTOR_STATE_GET_ITEM             0x00000400
#define PLAYER_ACTOR_STATE_HOLD_ITEM            0x00000800
#define PLAYER_ACTOR_STATE_JUMPING              0x00040000
#define PLAYER_ACTOR_STATE_CLIMB2               0x00200000
#define PLAYER_ACTOR_STATE_EPONA                0x00800000
#define PLAYER_ACTOR_STATE_WATER                0x08000000
#define PLAYER_ACTOR_STATE_USE_ITEM             0x10000000
#define PLAYER_ACTOR_STATE_CUTSCENE_FROZEN      0x20000000

void Player_Update(Player* this, PlayState* play);
void Player_SetModels(Player* link, s32 modelGroup);
int Player_UsingItem(Player* link);
int Player_GetEnvironmentalHazard(PlayState* play);
int Player_InCsMode(PlayState* play);
void Player_Freeze(PlayState* play);
void Player_Unfreeze(PlayState* play);
void Player_UseItem(PlayState* play, Player* link, s16 itemId);
s32 Player_ActionToModelGroup(Player* link, s32 itemAction);
void Player_PlaySfx(Player* player, u16 sfxId);
float Player_GetHeight(Player* player);
int Player_OverrideLimbDrawGameplayDefault(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Player* player);
int Player_OverrideLimbDrawPause(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Player* player);
int Player_OverrideLimbDrawGameplayFirstPerson(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void*);
void Player_SetBootData(PlayState* play, Player* player);
int Player_GetStrength(void);
s32 Player_SetCsAction(PlayState* play, Actor* csActor, u8 csAction);

#if defined(GAME_MM)
void Player_DrawHookshotReticle(PlayState* play, Player* player, f32 distance);
void Player_UpdateForm(Player* this, PlayState* play);
void Player_TryUpdateForm(Player* this, PlayState* play);
#endif

/* Custom stuff */
void Player_HandleBronzeScale(Player* this, PlayState* play);

#endif
