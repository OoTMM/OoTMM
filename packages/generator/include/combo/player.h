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
#define PLAYER_ACTOR_STATE_GROTTO               0x80000000

typedef enum PlayerCsAction {
    /*   -1 */ PLAYER_CSACTION_NEG1 = -1, // Specific to Kafei, any negative number works
    /* 0x00 */ PLAYER_CSACTION_NONE,
    /* 0x01 */ PLAYER_CSACTION_1,
    /* 0x02 */ PLAYER_CSACTION_2,
    /* 0x03 */ PLAYER_CSACTION_3,
    /* 0x04 */ PLAYER_CSACTION_4,
    /* 0x05 */ PLAYER_CSACTION_5,
    /* 0x06 */ PLAYER_CSACTION_6,
    /* 0x07 */ PLAYER_CSACTION_7,
    /* 0x08 */ PLAYER_CSACTION_8,
    /* 0x09 */ PLAYER_CSACTION_9,
    /* 0x0A */ PLAYER_CSACTION_10,
    /* 0x0B */ PLAYER_CSACTION_11,
    /* 0x0C */ PLAYER_CSACTION_12,
    /* 0x0D */ PLAYER_CSACTION_13,
    /* 0x0E */ PLAYER_CSACTION_14,
    /* 0x0F */ PLAYER_CSACTION_15,
    /* 0x10 */ PLAYER_CSACTION_16,
    /* 0x11 */ PLAYER_CSACTION_17,
    /* 0x12 */ PLAYER_CSACTION_18, // Strangled by Wallmaster
    /* 0x13 */ PLAYER_CSACTION_19,
    /* 0x14 */ PLAYER_CSACTION_20,
    /* 0x15 */ PLAYER_CSACTION_21,
    /* 0x16 */ PLAYER_CSACTION_22,
    /* 0x17 */ PLAYER_CSACTION_23,
    /* 0x18 */ PLAYER_CSACTION_24,
    /* 0x19 */ PLAYER_CSACTION_25,
    /* 0x1A */ PLAYER_CSACTION_26, // Halt!
    /* 0x1B */ PLAYER_CSACTION_27,
    /* 0x1C */ PLAYER_CSACTION_28,
    /* 0x1D */ PLAYER_CSACTION_29,
    /* 0x1E */ PLAYER_CSACTION_30,
    /* 0x1F */ PLAYER_CSACTION_31,
    /* 0x20 */ PLAYER_CSACTION_32,
    /* 0x21 */ PLAYER_CSACTION_33,
    /* 0x22 */ PLAYER_CSACTION_34,
    /* 0x23 */ PLAYER_CSACTION_35,
    /* 0x24 */ PLAYER_CSACTION_36,
    /* 0x25 */ PLAYER_CSACTION_37,
    /* 0x26 */ PLAYER_CSACTION_38,
    /* 0x27 */ PLAYER_CSACTION_39,
    /* 0x28 */ PLAYER_CSACTION_40,
    /* 0x29 */ PLAYER_CSACTION_41,
    /* 0x2A */ PLAYER_CSACTION_42,
    /* 0x2B */ PLAYER_CSACTION_43,
    /* 0x2C */ PLAYER_CSACTION_44,
    /* 0x2D */ PLAYER_CSACTION_45,
    /* 0x2E */ PLAYER_CSACTION_46,
    /* 0x2F */ PLAYER_CSACTION_47,
    /* 0x30 */ PLAYER_CSACTION_48,
    /* 0x31 */ PLAYER_CSACTION_49,
    /* 0x32 */ PLAYER_CSACTION_50,
    /* 0x33 */ PLAYER_CSACTION_51,
    /* 0x34 */ PLAYER_CSACTION_52,
    /* 0x35 */ PLAYER_CSACTION_53,
    /* 0x36 */ PLAYER_CSACTION_54,
    /* 0x37 */ PLAYER_CSACTION_55,
    /* 0x38 */ PLAYER_CSACTION_56,
    /* 0x39 */ PLAYER_CSACTION_57,
    /* 0x3A */ PLAYER_CSACTION_58,
    /* 0x3B */ PLAYER_CSACTION_59,
    /* 0x3C */ PLAYER_CSACTION_60,
    /* 0x3D */ PLAYER_CSACTION_61,
    /* 0x3E */ PLAYER_CSACTION_62,
    /* 0x3F */ PLAYER_CSACTION_63,
    /* 0x40 */ PLAYER_CSACTION_64,
    /* 0x41 */ PLAYER_CSACTION_65,
    /* 0x42 */ PLAYER_CSACTION_66, // Look side-to-side with chin down
    /* 0x43 */ PLAYER_CSACTION_67,
    /* 0x44 */ PLAYER_CSACTION_68,
    /* 0x45 */ PLAYER_CSACTION_69,
    /* 0x46 */ PLAYER_CSACTION_70,
    /* 0x47 */ PLAYER_CSACTION_71,
    /* 0x48 */ PLAYER_CSACTION_72,
    /* 0x49 */ PLAYER_CSACTION_73,
    /* 0x4A */ PLAYER_CSACTION_74, // Give a big nod of approval
    /* 0x4B */ PLAYER_CSACTION_75,
    /* 0x4C */ PLAYER_CSACTION_76,
    /* 0x4D */ PLAYER_CSACTION_77,
    /* 0x4E */ PLAYER_CSACTION_78,
    /* 0x4F */ PLAYER_CSACTION_79,
    /* 0x50 */ PLAYER_CSACTION_80,
    /* 0x51 */ PLAYER_CSACTION_81, // Look side-to-side with chin up
    /* 0x52 */ PLAYER_CSACTION_82, // Close eyes and sway body in circles
    /* 0x53 */ PLAYER_CSACTION_83,
    /* 0x54 */ PLAYER_CSACTION_84, // Sucked by the moon
    /* 0x55 */ PLAYER_CSACTION_85,
    /* 0x56 */ PLAYER_CSACTION_86,
    /* 0x57 */ PLAYER_CSACTION_87,
    /* 0x58 */ PLAYER_CSACTION_88,
    /* 0x59 */ PLAYER_CSACTION_89,
    /* 0x5A */ PLAYER_CSACTION_90,
    /* 0x5B */ PLAYER_CSACTION_91,
    /* 0x5C */ PLAYER_CSACTION_92,
    /* 0x5D */ PLAYER_CSACTION_93,
    /* 0x5E */ PLAYER_CSACTION_94,
    /* 0x5F */ PLAYER_CSACTION_95,
    /* 0x60 */ PLAYER_CSACTION_96,
    /* 0x61 */ PLAYER_CSACTION_97,
    /* 0x62 */ PLAYER_CSACTION_98,
    /* 0x63 */ PLAYER_CSACTION_99,
    /* 0x64 */ PLAYER_CSACTION_100,
    /* 0x65 */ PLAYER_CSACTION_101,
    /* 0x66 */ PLAYER_CSACTION_102,
    /* 0x67 */ PLAYER_CSACTION_103,
    /* 0x68 */ PLAYER_CSACTION_104,
    /* 0x69 */ PLAYER_CSACTION_105,
    /* 0x6A */ PLAYER_CSACTION_106,
    /* 0x6B */ PLAYER_CSACTION_107,
    /* 0x6C */ PLAYER_CSACTION_108,
    /* 0x6D */ PLAYER_CSACTION_109,
    /* 0x6E */ PLAYER_CSACTION_110,
    /* 0x6F */ PLAYER_CSACTION_111,
    /* 0x70 */ PLAYER_CSACTION_112,
    /* 0x71 */ PLAYER_CSACTION_113,
    /* 0x72 */ PLAYER_CSACTION_114,
    /* 0x73 */ PLAYER_CSACTION_115,
    /* 0x74 */ PLAYER_CSACTION_116,
    /* 0x75 */ PLAYER_CSACTION_117,
    /* 0x76 */ PLAYER_CSACTION_118,
    /* 0x77 */ PLAYER_CSACTION_119,
    /* 0x78 */ PLAYER_CSACTION_120,
    /* 0x79 */ PLAYER_CSACTION_121,
    /* 0x7A */ PLAYER_CSACTION_122,
    /* 0x7B */ PLAYER_CSACTION_123,
    /* 0x7C */ PLAYER_CSACTION_124,
    /* 0x7D */ PLAYER_CSACTION_125,
    /* 0x7E */ PLAYER_CSACTION_126,
    /* 0x7F */ PLAYER_CSACTION_127,
    /* 0x80 */ PLAYER_CSACTION_128,
    /* 0x81 */ PLAYER_CSACTION_129,
    /* 0x82 */ PLAYER_CSACTION_130,
    /* 0x83 */ PLAYER_CSACTION_131,
    /* 0x84 */ PLAYER_CSACTION_132,
    /* 0x85 */ PLAYER_CSACTION_133,
    /* 0x86 */ PLAYER_CSACTION_134,
    /* 0x87 */ PLAYER_CSACTION_135,
    /* 0x88 */ PLAYER_CSACTION_136,
    /* 0x89 */ PLAYER_CSACTION_137,
    /* 0x8A */ PLAYER_CSACTION_138,
    /* 0x8B */ PLAYER_CSACTION_139,
    /* 0x8C */ PLAYER_CSACTION_MAX
} PlayerCsAction;

#if defined (GAME_MM)

#define PLAYER_CSACTION_END    PLAYER_CSACTION_6
#define PLAYER_CSACTION_WAIT   PLAYER_CSACTION_7

#else

#define PLAYER_CSACTION_END    PLAYER_CSACTION_7

#endif

void Play_ReturnToMainCam(PlayState* this, s16 camId, s16 duration);
void Player_Update(Player* this, PlayState* play);
void Player_SetModels(Player* link, s32 modelGroup);
int Player_GetMask(PlayState* play);
int Player_UsingItem(Player* link);
int Player_ActionToBottle(Player* link, s32 itemAction);
int Player_GetEnvironmentalHazard(PlayState* play);
int Player_InCsMode(PlayState* play);
void Player_Freeze(PlayState* play);
void Player_Unfreeze(PlayState* play);
void Player_UseItem(PlayState* play, Player* link, s16 itemId);
s32 Player_CheckHostileLockOn(Player* link);
s32 Player_IsChildWithHylianShield(Player* link);
void Player_PlaySfx(Player* player, u16 sfxId);
float Player_GetHeight(Player* player);
int Player_OverrideLimbDrawGameplayDefault(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Player* player);
int Player_OverrideLimbDrawPause(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Player* player);
int Player_OverrideLimbDrawGameplayFirstPerson(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, void*);
void Player_SetBootData(PlayState* play, Player* player);
int Player_GetStrength(void);
s32 Player_SetCsAction(PlayState* play, Actor* csActor, u8 csAction);
int Player_IsBurningStickInRange(PlayState* play, Vec3f* pos, f32 xzRange, f32 yRange);
s32 func_80123590(PlayState* play, Actor* actor);

#if defined(GAME_MM)
void Player_DrawHookshotReticle(PlayState* play, Player* player, f32 distance);
void Player_UpdateForm(Player* this, PlayState* play);
s32 func_80123810(PlayState* play);
void Player_TryUpdateForm(Player* this, PlayState* play);
#endif

/* Custom stuff */
void Player_HandleBronzeScale(Player* this, PlayState* play);

#endif
