#ifndef COMBO_MM_PLAYER_H
#define COMBO_MM_PLAYER_H

#include <ultra64.h>
#include <combo/actor.h>
#include <combo/actor.h>
#include <combo/dma.h>

struct Actor_Player;

int Player_GetMask(GameState_Play* play);

typedef struct {
    /* 0x00 */ s32 active;
    /* 0x04 */ Vec3f tip;
    /* 0x10 */ Vec3f base;
} WeaponInfo; /* size = 0x1C */

typedef struct {
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ u8 alpha;
    /* 0x04 */ MtxF mf;
} struct_80122D44_arg1_unk_04; /* size = 0x44 */

typedef struct {
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ s8 unk_01;
    /* 0x02 */ char unk_02[2]; /* probably alignment padding */
    /* 0x04 */ struct_80122D44_arg1_unk_04 unk_04[4];
} struct_80122D44_arg1; /* size >= 0x114 */

typedef enum PlayerLimb {
    /* 0x00 */ PLAYER_LIMB_NONE,
    /* 0x01 */ PLAYER_LIMB_ROOT,
    /* 0x02 */ PLAYER_LIMB_WAIST,
    /* 0x03 */ PLAYER_LIMB_LOWER_ROOT,
    /* 0x04 */ PLAYER_LIMB_RIGHT_THIGH,
    /* 0x05 */ PLAYER_LIMB_RIGHT_SHIN,
    /* 0x06 */ PLAYER_LIMB_RIGHT_FOOT,
    /* 0x07 */ PLAYER_LIMB_LEFT_THIGH,
    /* 0x08 */ PLAYER_LIMB_LEFT_SHIN,
    /* 0x09 */ PLAYER_LIMB_LEFT_FOOT,
    /* 0x0A */ PLAYER_LIMB_UPPER_ROOT,
    /* 0x0B */ PLAYER_LIMB_HEAD,
    /* 0x0C */ PLAYER_LIMB_HAT,
    /* 0x0D */ PLAYER_LIMB_COLLAR,
    /* 0x0E */ PLAYER_LIMB_LEFT_SHOULDER,
    /* 0x0F */ PLAYER_LIMB_LEFT_FOREARM,
    /* 0x10 */ PLAYER_LIMB_LEFT_HAND,
    /* 0x11 */ PLAYER_LIMB_RIGHT_SHOULDER,
    /* 0x12 */ PLAYER_LIMB_RIGHT_FOREARM,
    /* 0x13 */ PLAYER_LIMB_RIGHT_HAND,
    /* 0x14 */ PLAYER_LIMB_SHEATH,
    /* 0x15 */ PLAYER_LIMB_TORSO,
    /* 0x16 */ PLAYER_LIMB_MAX
} PlayerLimb;

typedef enum PlayerBodyPart {
    /* 0x00 */ PLAYER_BODYPART_WAIST,          /* PLAYER_LIMB_WAIST */
    /* 0x01 */ PLAYER_BODYPART_RIGHT_THIGH,    /* PLAYER_LIMB_RIGHT_THIGH */
    /* 0x02 */ PLAYER_BODYPART_RIGHT_SHIN,     /* PLAYER_LIMB_RIGHT_SHIN */
    /* 0x03 */ PLAYER_BODYPART_RIGHT_FOOT,     /* PLAYER_LIMB_RIGHT_FOOT */
    /* 0x04 */ PLAYER_BODYPART_LEFT_THIGH,     /* PLAYER_LIMB_LEFT_THIGH */
    /* 0x05 */ PLAYER_BODYPART_LEFT_SHIN,      /* PLAYER_LIMB_LEFT_SHIN */
    /* 0x06 */ PLAYER_BODYPART_LEFT_FOOT,      /* PLAYER_LIMB_LEFT_FOOT */
    /* 0x07 */ PLAYER_BODYPART_HEAD,           /* PLAYER_LIMB_HEAD */
    /* 0x08 */ PLAYER_BODYPART_HAT,            /* PLAYER_LIMB_HAT */
    /* 0x09 */ PLAYER_BODYPART_COLLAR,         /* PLAYER_LIMB_COLLAR */
    /* 0x0A */ PLAYER_BODYPART_LEFT_SHOULDER,  /* PLAYER_LIMB_LEFT_SHOULDER */
    /* 0x0B */ PLAYER_BODYPART_LEFT_FOREARM,   /* PLAYER_LIMB_LEFT_FOREARM */
    /* 0x0C */ PLAYER_BODYPART_LEFT_HAND,      /* PLAYER_LIMB_LEFT_HAND */
    /* 0x0D */ PLAYER_BODYPART_RIGHT_SHOULDER, /* PLAYER_LIMB_R_SHOULDER */
    /* 0x0E */ PLAYER_BODYPART_RIGHT_FOREARM,  /* PLAYER_LIMB_R_FOREARM */
    /* 0x0F */ PLAYER_BODYPART_RIGHT_HAND,     /* PLAYER_LIMB_R_HAND */
    /* 0x10 */ PLAYER_BODYPART_SHEATH,         /* PLAYER_LIMB_SHEATH */
    /* 0x11 */ PLAYER_BODYPART_TORSO,          /* PLAYER_LIMB_TORSO */
    /* 0x12 */ PLAYER_BODYPART_MAX
} PlayerBodyPart;

typedef enum PlayerItemAction {
    /*   -1 */ PLAYER_IA_MINUS1 = -1, // TODO: determine usages with more player docs, possibly split into seperate values (see known usages above)
    /* 0x00 */ PLAYER_IA_NONE,
    /* 0x01 */ PLAYER_IA_LAST_USED,
    /* 0x02 */ PLAYER_IA_FISHING_ROD,
    /* 0x03 */ PLAYER_IA_SWORD_MIN,
    /* 0x03 */ PLAYER_IA_SWORD_KOKIRI = PLAYER_IA_SWORD_MIN,
    /* 0x04 */ PLAYER_IA_SWORD_RAZOR,
    /* 0x05 */ PLAYER_IA_SWORD_GILDED,
    /* 0x06 */ PLAYER_IA_SWORD_TWO_HANDED,
    /* 0x07 */ PLAYER_IA_DEKU_STICK,
    /* 0x08 */ PLAYER_IA_ZORA_FINS,
    /* 0x09 */ PLAYER_IA_BOW,
    /* 0x0A */ PLAYER_IA_BOW_FIRE,
    /* 0x0B */ PLAYER_IA_BOW_ICE,
    /* 0x0C */ PLAYER_IA_BOW_LIGHT,
    /* 0x0D */ PLAYER_IA_HOOKSHOT,
    /* 0x0E */ PLAYER_IA_EXPLOSIVE_MIN,
    /* 0x0E */ PLAYER_IA_BOMB = PLAYER_IA_EXPLOSIVE_MIN,
    /* 0x0F */ PLAYER_IA_POWDER_KEG,
    /* 0x10 */ PLAYER_IA_BOMBCHU,
    /* 0x11 */ PLAYER_IA_11,
    /* 0x12 */ PLAYER_IA_DEKU_NUT,
    /* 0x13 */ PLAYER_IA_PICTOGRAPH_BOX,
    /* 0x14 */ PLAYER_IA_OCARINA,
    /* 0x15 */ PLAYER_IA_BOTTLE_MIN,
    /* 0x15 */ PLAYER_IA_BOTTLE_EMPTY = PLAYER_IA_BOTTLE_MIN,
    /* 0x16 */ PLAYER_IA_BOTTLE_FISH,
    /* 0x17 */ PLAYER_IA_BOTTLE_SPRING_WATER,
    /* 0x18 */ PLAYER_IA_BOTTLE_HOT_SPRING_WATER,
    /* 0x19 */ PLAYER_IA_BOTTLE_ZORA_EGG,
    /* 0x1A */ PLAYER_IA_BOTTLE_DEKU_PRINCESS,
    /* 0x1B */ PLAYER_IA_BOTTLE_GOLD_DUST,
    /* 0x1C */ PLAYER_IA_BOTTLE_1C,
    /* 0x1D */ PLAYER_IA_BOTTLE_SEAHORSE,
    /* 0x1E */ PLAYER_IA_BOTTLE_MUSHROOM,
    /* 0x1F */ PLAYER_IA_BOTTLE_HYLIAN_LOACH,
    /* 0x20 */ PLAYER_IA_BOTTLE_BUG,
    /* 0x21 */ PLAYER_IA_BOTTLE_POE,
    /* 0x22 */ PLAYER_IA_BOTTLE_BIG_POE,
    /* 0x23 */ PLAYER_IA_BOTTLE_POTION_RED,
    /* 0x24 */ PLAYER_IA_BOTTLE_POTION_BLUE,
    /* 0x25 */ PLAYER_IA_BOTTLE_POTION_GREEN,
    /* 0x26 */ PLAYER_IA_BOTTLE_MILK,
    /* 0x27 */ PLAYER_IA_BOTTLE_MILK_HALF,
    /* 0x28 */ PLAYER_IA_BOTTLE_CHATEAU,
    /* 0x29 */ PLAYER_IA_BOTTLE_FAIRY,
    /* 0x2A */ PLAYER_IA_MOONS_TEAR,
    /* 0x2B */ PLAYER_IA_DEED_LAND,
    /* 0x2C */ PLAYER_IA_ROOM_KEY,
    /* 0x2D */ PLAYER_IA_LETTER_TO_KAFEI,
    /* 0x2E */ PLAYER_IA_MAGIC_BEANS,
    /* 0x2F */ PLAYER_IA_DEED_SWAMP,
    /* 0x30 */ PLAYER_IA_DEED_MOUNTAIN,
    /* 0x31 */ PLAYER_IA_DEED_OCEAN,
    /* 0x32 */ PLAYER_IA_32,
    /* 0x33 */ PLAYER_IA_LETTER_MAMA,
    /* 0x34 */ PLAYER_IA_34,
    /* 0x35 */ PLAYER_IA_35,
    /* 0x36 */ PLAYER_IA_PENDANT_OF_MEMORIES,
    /* 0x37 */ PLAYER_IA_37,
    /* 0x38 */ PLAYER_IA_38,
    /* 0x39 */ PLAYER_IA_39,
    /* 0x3A */ PLAYER_IA_MASK_MIN,
    /* 0x3A */ PLAYER_IA_MASK_TRUTH = PLAYER_IA_MASK_MIN,
    /* 0x3B */ PLAYER_IA_MASK_KAFEIS_MASK,
    /* 0x3C */ PLAYER_IA_MASK_ALL_NIGHT,
    /* 0x3D */ PLAYER_IA_MASK_BUNNY,
    /* 0x3E */ PLAYER_IA_MASK_KEATON,
    /* 0x3F */ PLAYER_IA_MASK_GARO,
    /* 0x40 */ PLAYER_IA_MASK_ROMANI,
    /* 0x41 */ PLAYER_IA_MASK_CIRCUS_LEADER,
    /* 0x42 */ PLAYER_IA_MASK_POSTMAN,
    /* 0x43 */ PLAYER_IA_MASK_COUPLE,
    /* 0x44 */ PLAYER_IA_MASK_GREAT_FAIRY,
    /* 0x45 */ PLAYER_IA_MASK_GIBDO,
    /* 0x46 */ PLAYER_IA_MASK_DON_GERO,
    /* 0x47 */ PLAYER_IA_MASK_KAMARO,
    /* 0x48 */ PLAYER_IA_MASK_CAPTAIN,
    /* 0x49 */ PLAYER_IA_MASK_STONE,
    /* 0x4A */ PLAYER_IA_MASK_BREMEN,
    /* 0x4B */ PLAYER_IA_MASK_BLAST,
    /* 0x4C */ PLAYER_IA_MASK_SCENTS,
    /* 0x4D */ PLAYER_IA_MASK_GIANT,
    /* 0x4E */ PLAYER_IA_MASK_TRANSFORMATION_MIN,
    /* 0x4E */ PLAYER_IA_MASK_FIERCE_DEITY = PLAYER_IA_MASK_TRANSFORMATION_MIN,
    /* 0x4F */ PLAYER_IA_MASK_GORON,
    /* 0x50 */ PLAYER_IA_MASK_ZORA,
    /* 0x51 */ PLAYER_IA_MASK_DEKU,
    /* 0x51 */ PLAYER_IA_MASK_MAX = PLAYER_IA_MASK_DEKU,
    /* 0x52 */ PLAYER_IA_LENS_OF_TRUTH,
    /* 0x53 */ PLAYER_IA_MAX
} PlayerItemAction;

#define GET_MASK_FROM_IA(itemAction) ((itemAction) - (PLAYER_IA_MASK_MIN - 1))

#define PLAYER_MASK_NONE            0x00
#define PLAYER_MASK_TRUTH           0x01
#define PLAYER_MASK_KAFEIS_MASK     0x02
#define PLAYER_MASK_ALL_NIGHT       0x03
#define PLAYER_MASK_BUNNY           0x04
#define PLAYER_MASK_KEATON          0x05
#define PLAYER_MASK_GARO            0x06
#define PLAYER_MASK_ROMANI          0x07
#define PLAYER_MASK_CIRCUS_LEADER   0x08
#define PLAYER_MASK_POSTMAN         0x09
#define PLAYER_MASK_COUPLE          0x0a
#define PLAYER_MASK_GREAT_FAIRY     0x0b
#define PLAYER_MASK_GIBDO           0x0c
#define PLAYER_MASK_DON_GERO        0x0d
#define PLAYER_MASK_KAMARO          0x0e
#define PLAYER_MASK_CAPTAIN         0x0f
#define PLAYER_MASK_STONE           0x10
#define PLAYER_MASK_BREMEN          0x11
#define PLAYER_MASK_BLAST           0x12
#define PLAYER_MASK_SCENTS          0x13
#define PLAYER_MASK_GIANT           0x14
#define PLAYER_MASK_FIERCE_DEITY    0x15
#define PLAYER_MASK_GORON           0x16
#define PLAYER_MASK_ZORA            0x17
#define PLAYER_MASK_DEKU            0x18
#define PLAYER_MASK_MAX             0x19

typedef enum PlayerCsAction {
    /*   -1 */ PLAYER_CSACTION_NEG1 = -1, // Specific to Kafei, any negative number works
    /* 0x00 */ PLAYER_CSACTION_NONE,
    /* 0x01 */ PLAYER_CSACTION_1,
    /* 0x02 */ PLAYER_CSACTION_2,
    /* 0x03 */ PLAYER_CSACTION_3,
    /* 0x04 */ PLAYER_CSACTION_4,
    /* 0x05 */ PLAYER_CSACTION_5,
    /* 0x06 */ PLAYER_CSACTION_END,
    /* 0x07 */ PLAYER_CSACTION_WAIT,
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

typedef enum {
    /* 0 */ PLAYER_ENV_HAZARD_NONE,
    /* 1 */ PLAYER_ENV_HAZARD_HOTROOM,
    /* 2 */ PLAYER_ENV_HAZARD_UNDERWATER_FLOOR,
    /* 3 */ PLAYER_ENV_HAZARD_SWIMMING,
    /* 4 */ PLAYER_ENV_HAZARD_UNDERWATER_FREE
} PlayerEnvHazard;

typedef struct PlayerAnimationFrame {
    /* 0x000 */ Vec3s frameTable[PLAYER_LIMB_MAX];
    /* 0x108 */ s16 appearanceInfo; /* bitpack containing the face and hands info */
} PlayerAnimationFrame; /* size = 0x10A */

#define PLAYER_PARAMS(startBgCamIndex, initMode) ((startBgCamIndex & 0xFF) | ((initMode & 0xF) << 8))

#define PLAYER_INITMODE_0 0x0
#define PLAYER_INITMODE_1 0x1
#define PLAYER_INITMODE_2 0x2
#define PLAYER_INITMODE_3 0x3
#define PLAYER_INITMODE_4 0x4
#define PLAYER_INITMODE_5 0x5
#define PLAYER_INITMODE_6 0x6
#define PLAYER_INITMODE_7 0x7
#define PLAYER_INITMODE_8 0x8
#define PLAYER_INITMODE_9 0x9
#define PLAYER_INITMODE_A 0xA
#define PLAYER_INITMODE_B 0xB
#define PLAYER_INITMODE_TELESCOPE 0xC
#define PLAYER_INITMODE_D 0xD
#define PLAYER_INITMODE_E 0xE
#define PLAYER_INITMODE_F 0xF
#define PLAYER_INITMODE_MAX 0x10

typedef struct PlayerAgeProperties {
    /* 0x00 */ f32 ceilingCheckHeight;
    /* 0x04 */ f32 shadowScale;
    /* 0x08 */ f32 unk_08;
    /* 0x0C */ f32 unk_0C;
    /* 0x10 */ f32 unk_10;
    /* 0x14 */ f32 unk_14; /* compared to yDistToLedge */
    /* 0x18 */ f32 unk_18; /* compared to yDistToLedge */
    /* 0x1C */ f32 unk_1C; /* compared to yDistToLedge */
    /* 0x20 */ f32 unk_20; /* unused? */
    /* 0x24 */ f32 unk_24; /* water stuff // depthInWater */
    /* 0x28 */ f32 unk_28; /* water stuff // depthInWater */
    /* 0x2C */ f32 unk_2C; /* water stuff // depthInWater */
    /* 0x30 */ f32 unk_30; /* water stuff // depthInWater */
    /* 0x34 */ f32 unk_34; /* height? */
    /* 0x38 */ f32 wallCheckRadius;
    /* 0x3C */ f32 unk_3C;
    /* 0x40 */ f32 unk_40;
    /* 0x44 */ Vec3s unk_44;
    /* 0x4A */ Vec3s unk_4A[4];
    /* 0x62 */ Vec3s unk_62[4];
    /* 0x7A */ Vec3s unk_7A[4];
    /* 0x92 */ u16 voiceSfxIdOffset;
    /* 0x94 */ u16 surfaceSfxIdOffset;
    /* 0x98 */ f32 unk_98;
    /* 0x9C */ f32 unk_9C;
    /* 0xA0 */ PlayerAnimationHeader* openChestAnim;
    /* 0xA4 */ PlayerAnimationHeader* unk_A4; /* OoT leftovers to interact with the Master Sword */
    /* 0xA8 */ PlayerAnimationHeader* unk_A8; /* OoT leftovers to interact with the Master Sword */
    /* 0xAC */ PlayerAnimationHeader* unk_AC;
    /* 0xB0 */ PlayerAnimationHeader* unk_B0;
    /* 0xB4 */ PlayerAnimationHeader* unk_B4[4];
    /* 0xC4 */ PlayerAnimationHeader* unk_C4[2];
    /* 0xCC */ PlayerAnimationHeader* unk_CC[2];
    /* 0xD4 */ PlayerAnimationHeader* unk_D4[2];
} PlayerAgeProperties; /* size = 0xDC */

#define PLAYER_LIMB_BUF_SIZE (ALIGN16(sizeof(PlayerAnimationFrame)) + 0xF)

typedef void (*PlayerActionFunc)(struct Actor_Player* this, struct GameState_Play* play);
typedef s32 (*PlayerUpperActionFunc)(struct Actor_Player* this, struct GameState_Play* play);
typedef void (*PlayerFuncD58)(struct GameState_Play* play, struct Actor_Player* this);

typedef struct Actor_Player
{
    /* 0x000 */ Actor actor;
    /* 0x144 */ s8 currentShield;
    /* 0x145 */ s8 currentBoots;
    /* 0x146 */ s8 heldItemButton;
    /* 0x147 */ s8 heldItemAction; /* PlayerItemAction enum */
    /* 0x148 */ u8 heldItemId; /* ItemId enum */
    /* 0x149 */ s8 prevBoots;
    /* 0x14A */ s8 itemAction; /* PlayerItemAction enum */
    /* 0x14B */ u8 transformation; /* PlayerTransformation enum */
    /* 0x14C */ u8 modelGroup; /* PlayerModelGroup enum */
    /* 0x14D */ u8 nextModelGroup;
    /* 0x14E */ s8 unk_14E;
    /* 0x14F */ u8 modelAnimType; /* PlayerAnimType enum */
    /* 0x150 */ u8 leftHandType;
    /* 0x151 */ u8 rightHandType;
    /* 0x152 */ u8 sheathType;
    /* 0x153 */ u8 currentMask; /* PlayerMask enum */
    /* 0x154 */ s8 unk_154;
    /* 0x155 */ u8 prevMask;
    /* 0x156 */ s8 pad_156; /* unused padding */
    /* 0x157 */ s8 pad_157; /* unused padding */
    /* 0x158 */ Gfx** rightHandDLists;
    /* 0x15C */ Gfx** leftHandDLists;
    /* 0x160 */ Gfx** sheathDLists;
    /* 0x164 */ Gfx** waistDLists;
    /* 0x168 */ u8 unk_168[0x4C];
    /* 0x1B4 */ s16 unk_1B4;
    /* 0x1B6 */ char unk_1B6[2];
    /* 0x1B8 */ u8 giObjectLoading;
    /* 0x1BC */ DmaRequest giObjectDmaRequest;
    /* 0x1DC */ OSMesgQueue objMsgQueue;
    /* 0x1F4 */ OSMesg objMsg;
    /* 0x1F8 */ void* objBuffer;
    /* 0x1FC */ u8 maskObjectLoadState;
    /* 0x1FD */ s8 maskId;
    /* 0x200 */ DmaRequest maskDmaRequest;
    /* 0x220 */ OSMesgQueue maskObjectLoadQueue;
    /* 0x238 */ OSMesg maskObjectLoadMsg;
    /* 0x23C */ void* maskObjectSegment;
    /* 0x240 */ SkelAnime skelAnime;
    /* 0x284 */ SkelAnime skelAnimeUpper;
    /* 0x2C8 */ SkelAnime unk_2C8;
    /* 0x30C */ Vec3s jointTable[5];
    /* 0x32A */ Vec3s morphTable[5];
    /* 0x348 */ BlinkInfo blinkInfo;
    /* 0x34C */ Actor* heldActor;
    /* 0x350 */ PosRot leftHandWorld;
    /* 0x364 */ Actor* rightHandActor;
    /* 0x368 */ PosRot rightHandWorld;
    /* 0x37C */ s8 doorType; /* PlayerDoorType enum */
    /* 0x37D */ s8 doorDirection;
    /* 0x37E */ s8 doorTimer;
    /* 0x37F */ s8 doorNext; /* used with spiral staircase */
    /* 0x380 */ Actor* doorActor;
    /* 0x384 */ s16 gi;
    /* 0x386 */ u16 getItemDirection;
    /* 0x388 */ Actor* interactRangeActor;
    /* 0x38C */ s8 mountSide;
    /* 0x390 */ Actor* rideActor;
    /* 0x394 */ u8 csMode;
    /* char        unk_395[0x6D7]; */
    /* 0x395 */ u8 prevCsAction; /* PlayerCsAction enum */
    /* 0x396 */ u8 cueId; /* PlayerCueId enum */
    /* 0x397 */ u8 unk_397; /* PlayerDoorType enum */
    /* 0x398 */ Actor* csActor;
    /* 0x39C */ u8 unk_39C[0x4];
    /* 0x3A0 */ Vec3f unk_3A0;
    /* 0x3AC */ Vec3f unk_3AC;
    /* 0x3B8 */ u16 unk_3B8;
    /* 0x3BA */ union {
                    s16 haltActorsDuringCsAction; /* If true, halt actors belonging to certain categories during a `csAction` */
                    s16 doorBgCamIndex; /* `BgCamIndex` used during a sliding door and spiral staircase cutscenes */
                } cv; /* "Cutscene Variable": context dependent variable that has different meanings depending on what function is called */
    /* 0x3BC */ s16 subCamId;
    /* 0x3BE */ char unk_3BE[2];
    /* 0x3C0 */ Vec3f unk_3C0;
    /* 0x3CC */ s16 unk_3CC;
    /* 0x3CE */ s8 unk_3CE;
    /* 0x3CF */ u8 unk_3CF;
    /* 0x3D0 */ struct_80122D44_arg1 unk_3D0;
    /* 0x4E4 */ u8 unk_4E4[0x20];
    /* 0x504 */ LightNode* lightNode;
    /* 0x508 */ LightInfo lightInfo;
    /* 0x518 */ ColliderCylinder cylinder;
    /* 0x564 */ ColliderQuad meleeWeaponQuads[2];
    /* 0x664 */ ColliderQuad shieldQuad;
    /* 0x6E4 */ ColliderCylinder shieldCylinder;
    /* 0x730 */ Actor* lockOnActor; /* Z/L-Targeted actor */
    /* 0x734 */ union {
                    char unk_734[4]; /* unused? */
                    struct {
                        u8 hoverBootsTimer;
                        u8 pad_735;
                        u8 pad_736;
                        u8 pad_737;
                    };
                };
    /* 0x738 */ s32 unk_738;
    /* 0x73C */ s32 meleeWeaponEffectIndex[3];
    /* 0x748 */ PlayerActionFunc actionFunc;
    /* 0x74C */ u8 jointTableBuffer[PLAYER_LIMB_BUF_SIZE];
    /* 0x7EB */ u8 morphTableBuffer[PLAYER_LIMB_BUF_SIZE];
    /* 0x88A */ u8 blendTableBuffer[PLAYER_LIMB_BUF_SIZE];
    /* 0x929 */ u8 jointTableUpperBuffer[PLAYER_LIMB_BUF_SIZE];
    /* 0x9C8 */ u8 morphTableUpperBuffer[PLAYER_LIMB_BUF_SIZE];
    /* 0xA68 */ PlayerAgeProperties* ageProperties;
    /* 0xA6C */ u32 state;
    /* 0xA70 */ u32 state2;
    /* 0xA74 */ u32 state3;
    /* 0xA78 */ Actor* unk_a78;
    /* 0xA7C */ Actor* boomerangActor;
    /* 0xA80 */ Actor* tatlActor;
    /* 0xA84 */ s16 tatlTextId;
    /* 0xA86 */ s8 csId;
    /* 0xA87 */ s8 exchangeItemAction; /* PlayerItemAction enum */
    /* 0xA88 */ Actor* talkActor;
    /* 0xA8C */ f32 talkActorDistance;
    /* 0xA90 */ Actor* unk_A90;
    /* 0xA94 */ f32 unk_A94;
    /* 0xA98 */ Actor* unk_A98;
    /* 0xA9C */ f32 secretRumbleCharge; /* builds per frame until discharges with a rumble request */
    /* 0xAA0 */ f32 closestSecretDistSq; /* Used to augment `secretRumbleCharge`. Cleared every frame */
    /* 0xAA4 */ s8 unk_AA4;
    /* 0xAA5 */ u8 unk_AA5; /* PlayerUnkAA5 enum */
    /* 0xAA6 */ u16 unk_AA6; /* flags of some kind */
    /* 0xAA8 */ s16 unk_AA8;
    /* 0xAAA */ s16 unk_AAA;
    /* 0xAAC */ Vec3s headLimbRot;
    /* 0xAB2 */ Vec3s upperLimbRot;
    /* 0xAB8 */ f32 unk_AB8;
    /* 0xABC */ f32 unk_ABC;
    /* 0xAC0 */ f32 unk_AC0;
    /* 0xAC4 */ PlayerUpperActionFunc upperActionFunc; /* Upper body/item action functions */
    /* 0xAC8 */ f32 skelAnimeUpperBlendWeight;
    /* 0xACC */ s16 unk_ACC;
    /* 0xACE */ s8 unk_ACE;
    /* 0xACF */ u8 putAwayCountdown; /* Frames to wait before showing "Put Away" on A */
    /* 0xAD0 */ f32 linearVelocity;
    /* 0xAD4 */ s16 currentYaw;
    /* 0xAD6 */ s16 targetYaw;
    /* 0xAD8 */ u16 underwaterTimer;
    /* 0xADA */ s8 meleeWeaponAnimation;
    /* 0xADB */ s8 meleeWeaponState;
    /* 0xADC */ s8 unk_ADC;
    /* 0xADD */ s8 unk_ADD; /* Some sort of combo counter */
    /* 0xADE */ u8 unk_ADE;
    /* 0xADF */ s8 unk_ADF[4]; /* Circular buffer used for testing for triggering a quickspin */
    /* 0xAE3 */ s8 unk_AE3[4]; /* Circular buffer used for ? */
    /* 0xAE7 */ union {
        s8 actionVar1;
    } av1; /* "Action Variable 1": context dependent variable that has different meanings depending on what action is currently running */
    /* 0xAE8 */ union {
        s16 actionVar2;
    } av2; /* "Action Variable 2": context dependent variable that has different meanings depending on what action is currently running */
    /* 0xAEC */ f32 unk_AEC;
    /* 0xAF0 */ union {
                    Vec3f unk_AF0[2];
                    f32 arr_AF0[6];
                };
    /* 0xB08 */ f32 unk_B08;
    /* 0xB0C */ f32 unk_B0C;
    /* 0xB10 */ f32 unk_B10[6];
    /* 0xB28 */ s16 unk_B28; /* Burning stick timer? */
    /* 0xB2A */ u8 drawGiId;
    /* 0xB2B */ s8 unk_b2b;
    /* 0xB2C */ f32 windSpeed;
    /* 0xB30 */ s16 windAngleX;
    /* 0xB32 */ s16 windAngleY;
    /* 0xB34 */ f32 unk_B34;
    /* 0xB38 */ f32 unk_B38;
    /* 0xB3C */ f32 unk_B3C;
    /* 0xB40 */ f32 unk_B40;
    /* 0xB44 */ f32 unk_B44;
    /* 0xB48 */ f32 unk_B48;
    /* 0xB4C */ s16 unk_B4C;
    /* 0xB4E */ s16 unk_B4E;
    /* 0xB50 */ f32 unk_B50;
    /* 0xB54 */ f32 yDistToLedge; /* y distance to ground above an interact wall. LEDGE_DIST_MAX if no ground if found */
    /* 0xB58 */ f32 distToInteractWall; /* xyz distance to the interact wall */
    /* 0xB5C */ u8 ledgeClimbType; /* see PlayerLedgeClimbType enum */
    /* 0xB5D */ u8 ledgeClimbDelayTimer;
    /* 0xB5E */ u8 unk_B5E;
    /* 0xB5F */ u8 unk_B5F;
    /* 0xB60 */ u16 blastMaskTimer;
    /* 0xB62 */ s16 unk_B62;
    /* 0xB64 */ u8 unk_B64;
    /* 0xB65 */ u8 shockTimer;
    /* 0xB66 */ u8 unk_B66;
    /* 0xB67 */ u8 remainingHopsCounter; /* Deku hopping on water */
    /* 0xB68 */ s16 fallStartHeight; /* last truncated Y position before falling */
    /* 0xB6A */ s16 fallDistance; /* truncated Y distance the player has fallen so far (positive is down) */
    /* 0xB6C */ s16 floorPitch; /* angle of the floor slope in the direction of current world yaw (positive for ascending slope) */
    /* 0xB6E */ s16 floorPitchAlt; /* the calculation for this value is bugged and doesn't represent anything meaningful */
    /* 0xB70 */ s16 unk_B70;
    /* 0xB72 */ u16 floorSfxOffset;
    /* 0xB74 */ u8 unk_B74;
    /* 0xB75 */ u8 unk_B75;
    /* 0xB76 */ s16 unk_B76;
    /* 0xB78 */ f32 unk_B78;
    /* 0xB7C */ f32 unk_B7C;
    /* 0xB80 */ f32 pushedSpeed; /* Pushing player, examples include water currents, floor conveyors, climbing sloped surfaces */
    /* 0xB84 */ s16 pushedYaw; /* Yaw of direction in which player is being pushed */
    /* 0xB86 */ s16 unk_B86[2]; /* unknown length */
    /* 0xB8A */ s16 unk_B8A;
    /* 0xB8C */ s16 unk_B8C;
    /* 0xB8E */ s16 unk_B8E;
    /* 0xB90 */ s16 unk_B90;
    /* 0xB92 */ s16 unk_B92;
    /* 0xB94 */ s16 unk_B94;
    /* 0xB96 */ s16 unk_B96;
    /* 0xB98 */ WeaponInfo meleeWeaponInfo[3];
    /* 0xBEC */ Vec3f bodyPartsPos[PLAYER_BODYPART_MAX];
    /* 0xCC4 */ MtxF leftHandMf;
    /* 0xD04 */ MtxF shieldMf;
    /* 0xD44 */ u8 isBurning;
    /* 0xD45 */ u8 flameTimers[PLAYER_BODYPART_MAX]; /* one flame per body part */
    /* 0xD57 */ u8 unk_D57;
    /* 0xD58 */ PlayerFuncD58 unk_D58;
    /* 0xD5C */ s8 invincibilityTimer; /* prevents damage when nonzero (positive = visible, counts towards zero each frame) */
    /* 0xD5D */ u8 floorTypeTimer; /* Unused remnant of OoT */
    /* 0xD5E */ u8 floorProperty; /* FloorProperty enum */
    /* 0xD5F */ u8 prevFloorType; /* Unused remnant of OoT */
    /* 0xD60 */ f32 prevControlStickMagnitude;
    /* 0xD64 */ s16 prevControlStickAngle;
    /* 0xD66 */ u16 prevFloorSfxOffset;
    /* 0xD68 */ s16 unk_D68;
    /* 0xD6A */ s8 unk_D6A;
    /* 0xD6B */ u8 unk_D6B;
    /* 0xD6C */ Vec3f unk_D6C; /* previous body part 0 position */
}
Actor_Player;

_Static_assert(sizeof(Actor_Player) == 0xd78, "MM Actor_Player size is wrong");

ASSERT_OFFSET(Actor_Player, objMsgQueue,         0x1dc);
ASSERT_OFFSET(Actor_Player, maskObjectLoadState, 0x1fc);
ASSERT_OFFSET(Actor_Player, gi,                  0x384);
ASSERT_OFFSET(Actor_Player, getItemDirection,    0x386);
ASSERT_OFFSET(Actor_Player, csMode,              0x394);
ASSERT_OFFSET(Actor_Player, prevCsAction,        0x395);
ASSERT_OFFSET(Actor_Player, unk_a78,             0xa78);
ASSERT_OFFSET(Actor_Player, drawGiId,            0xb2a);
ASSERT_OFFSET(Actor_Player, unk_b2b,             0xb2b);

typedef struct {
    /* 0x00 */ u32 maskDListEntry[24];
} PlayerMaskDList; /* size = 0x60 */

#define GET_PLAYER_CUSTOM_BOOTS(player) (player->actor.id == AC_PLAYER && player->transformation == MM_PLAYER_FORM_HUMAN ? (player->currentBoots == 6 ? PLAYER_BOOTS_IRON : (player->currentBoots == 0 ? PLAYER_BOOTS_HOVER : -1)) : -1)

#endif
