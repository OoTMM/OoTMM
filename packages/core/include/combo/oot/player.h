#ifndef COMBO_OOT_PLAYER_H
#define COMBO_OOT_PLAYER_H

#include <ultra64.h>
#include <combo/common/actor.h>

typedef struct PACKED ALIGNED(4) Actor_Player
{
    Actor           base;
    char            unk_13c[0x04];
    u8              heldItemButton;
    u8              itemAction;
    u8              heldItemId;
    u8              boots;
    u8              heldItemAction;
    char            unk_145[0x3f];
    OSMesgQueue     objMsgQueue;
    OSMesg          objMsg;
    void*           objBuffer;
    char            unk_1a4[0x4c8];
    u32             state;
    u32             state2;
    /* 0x674 */ Actor*          unk_674;
    /* 0x678 */ Actor*          boomerangActor;
    /* 0x67C */ Actor*          naviActor;
    /* 0x680 */ s16             naviTextId;
    /* 0x682 */ u8              state3;
    /* 0x683 */ s8              exchangeItemId;
    char            unk_684[0x3e8];
}
Actor_Player;

_Static_assert(sizeof(Actor_Player) == 0xa6c, "Actor_Player size is wrong");

ASSERT_OFFSET(Actor_Player, unk_13c,        0x13c);
ASSERT_OFFSET(Actor_Player, heldItemButton, 0x140);
ASSERT_OFFSET(Actor_Player, itemAction,     0x141);
ASSERT_OFFSET(Actor_Player, heldItemId,     0x142);
ASSERT_OFFSET(Actor_Player, boots,          0x143);
ASSERT_OFFSET(Actor_Player, heldItemAction, 0x144);
ASSERT_OFFSET(Actor_Player, unk_145,        0x145);

#endif
