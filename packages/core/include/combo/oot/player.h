#ifndef COMBO_OOT_PLAYER_H
#define COMBO_OOT_PLAYER_H

#include <ultra64.h>
#include <combo/common/actor.h>

typedef struct PACKED ALIGNED(4) Actor_Player
{
    Actor           base;
    s8              currentTunic; // current tunic from `PlayerTunic`
    s8              currentSwordItemId;
    s8              currentShield; // current shield from `PlayerShield`
    s8              currentBoots; // current boots from `PlayerBoots`
    u8              heldItemButton;
    u8              itemAction;
    u8              heldItemId;
    u8              boots;
    u8              heldItemAction;
    char            unk_155[0x003];
    u8              modelGroup;
    u8              nextModelGroup;
    s8              itemChangeType;
    u8              modelAnimType;
    u8              leftHandType;
    u8              rightHandType;
    u8              sheathType;
    u8              mask;
    char            unk_150[0x34];
    OSMesgQueue     objMsgQueue;
    OSMesg          objMsg;
    void*           objBuffer;
    char            unk_1a4[0x280];
    s16             gi; /* Extended to s16 */
    char            unk_426[0x246];
    u32             state;
    u32             state2;
    Actor*          unk_674;
    Actor*          boomerangActor;
    Actor*          naviActor;
    s16             naviTextId;
    u8              state3;
    s8              exchangeItemId;
    char            unk_684[0x1ce];
    u8              drawGiId;
    char            unk_853[0x219];

}
Actor_Player;

_Static_assert(sizeof(Actor_Player) == 0xa6c, "Actor_Player size is wrong");

ASSERT_OFFSET(Actor_Player, currentTunic,   0x13c);
ASSERT_OFFSET(Actor_Player, heldItemButton, 0x140);
ASSERT_OFFSET(Actor_Player, itemAction,     0x141);
ASSERT_OFFSET(Actor_Player, heldItemId,     0x142);
ASSERT_OFFSET(Actor_Player, boots,          0x143);
ASSERT_OFFSET(Actor_Player, heldItemAction, 0x144);
ASSERT_OFFSET(Actor_Player, mask,           0x14f);
ASSERT_OFFSET(Actor_Player, unk_150,        0x150);
ASSERT_OFFSET(Actor_Player, unk_1a4,        0x1a4);
ASSERT_OFFSET(Actor_Player, gi,             0x424);
ASSERT_OFFSET(Actor_Player, unk_426,        0x426);
ASSERT_OFFSET(Actor_Player, unk_674,        0x674);
ASSERT_OFFSET(Actor_Player, boomerangActor, 0x678);
ASSERT_OFFSET(Actor_Player, naviActor,      0x67c);
ASSERT_OFFSET(Actor_Player, naviTextId,     0x680);
ASSERT_OFFSET(Actor_Player, state3,         0x682);
ASSERT_OFFSET(Actor_Player, exchangeItemId, 0x683);
ASSERT_OFFSET(Actor_Player, unk_684,        0x684);
ASSERT_OFFSET(Actor_Player, drawGiId,       0x852);
ASSERT_OFFSET(Actor_Player, unk_853,        0x853);

#endif
