#ifndef COMBO_MM_PLAYER_H
#define COMBO_MM_PLAYER_H

#include <ultra64.h>
#include <combo/common/actor.h>

typedef struct PACKED ALIGNED(4)
{
    Actor       base;
    /* 0x144 */ s8 currentShield;
    /* 0x145 */ s8 currentBoots;
    /* 0x146 */ s8 heldItemButton;
    /* 0x147 */ s8 heldItemAction; // PlayerItemAction enum
    /* 0x148 */ u8 heldItemId; // ItemId enum
    /* 0x149 */ s8 prevBoots;
    /* 0x14A */ s8 itemAction; // PlayerItemAction enum
    /* 0x14B */ u8 transformation; // PlayerTransformation enum
    /* 0x14C */ u8 modelGroup; // PlayerModelGroup enum
    /* 0x14D */ u8 nextModelGroup;
    /* 0x14E */ s8 unk_14E;
    /* 0x14F */ u8 modelAnimType; // PlayerAnimType enum
    /* 0x150 */ u8 leftHandType;
    /* 0x151 */ u8 rightHandType;
    /* 0x152 */ u8 sheathType;
    /* 0x153 */ u8 currentMask; // PlayerMask enum
    /* 0x154 */ s8 unk_154;
    /* 0x155 */ u8 prevMask;
    /* 0x156 */ s8 pad_156; // unused padding
    /* 0x157 */ s8 pad_157; // unused padding
    char        unk_158[0x84];
    OSMesgQueue objMsgQueue;
    OSMesg      objMsg;
    void*       objBuffer;
    char        unk_1fc[0x188];
    s16         gi;
    char        unk_386[0x00e];
    u8          csMode;
    char        unk_395[0x6D7];
    u32         state;
    u32         state2;
    u32         state3;
    char        unk_a78[0x0b2];
    u8          drawGiId;
    char        unk_b2b[0x24d];
}
Actor_Player;

_Static_assert(sizeof(Actor_Player) == 0xd78, "MM Actor_Player size is wrong");

ASSERT_OFFSET(Actor_Player, unk_1fc,        0x1fc);
ASSERT_OFFSET(Actor_Player, gi,             0x384);
ASSERT_OFFSET(Actor_Player, unk_386,        0x386);
ASSERT_OFFSET(Actor_Player, csMode,         0x394);
ASSERT_OFFSET(Actor_Player, unk_395,        0x395);
ASSERT_OFFSET(Actor_Player, unk_a78,        0xa78);
ASSERT_OFFSET(Actor_Player, drawGiId,       0xb2a);
ASSERT_OFFSET(Actor_Player, unk_b2b,        0xb2b);

#endif
