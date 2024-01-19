#ifndef COMBO_ACTORS_EN_ITEM00_H
#define COMBO_ACTORS_EN_ITEM00_H

#include <combo/xflags.h>
#include <combo/common/actor.h>

#if defined(GAME_OOT)
# define ITEM00_RUPEE_GREEN     0x00
# define ITEM00_RUPEE_BLUE      0x01
# define ITEM00_RUPEE_RED       0x02
# define ITEM00_RECOVERY_HEART  0x03
# define ITEM00_BOMB            0x04
# define ITEM00_BOMBCHU         0x05
# define ITEM00_HEART_PIECE     0x06
# define ITEM00_HEART_CONTAINER 0x07
# define ITEM00_ARROWS_5        0x08
# define ITEM00_ARROWS_10       0x09
# define ITEM00_ARROWS_30       0x0a
# define ITEM00_BOMBS_5         0x0b
# define ITEM00_NUT             0x0c
# define ITEM00_STICK           0x0d
# define ITEM00_MAGIC_LARGE     0x0e
# define ITEM00_MAGIC_SMALL     0x0f
# define ITEM00_SEEDS_5         0x10
# define ITEM00_KEY             0x11
# define ITEM00_FLEXIBLE        0x12
# define ITEM00_RUPEE_ORANGE    0x13
# define ITEM00_RUPEE_PURPLE    0x14
# define ITEM00_SHIELD_DEKU     0x15
# define ITEM00_SHIELD_HYLIAN   0x16
# define ITEM00_TUNIC_ZORA      0x17
# define ITEM00_TUNIC_GORON     0x18
# define ITEM00_BOMBS_5_ALT     0x19
#else
# define ITEM00_RUPEE_GREEN     0x00
# define ITEM00_RUPEE_BLUE      0x01
# define ITEM00_RUPEE_RED       0x02
# define ITEM00_RECOVERY_HEART  0x03
# define ITEM00_BOMB            0x04
# define ITEM00_ARROWS_10       0x05
# define ITEM00_HEART_PIECE     0x06
# define ITEM00_HEART_CONTAINER 0x07
# define ITEM00_ARROWS_20       0x08
# define ITEM00_ARROWS_30       0x09
# define ITEM00_ARROWS_30_ALT   0x0a
# define ITEM00_BOMBS_5         0x0b
# define ITEM00_NUT             0x0c
# define ITEM00_STICK           0x0d
# define ITEM00_MAGIC_LARGE     0x0e
# define ITEM00_MAGIC_SMALL     0x0f
# define ITEM00_KEY             0x11
# define ITEM00_RUPEE_ORANGE    0x13
# define ITEM00_RUPEE_PURPLE    0x14
# define ITEM00_BOMBCHU         0x19
#endif

typedef struct ALIGNED(4) Actor_EnItem00
{
    Actor   base;
    void*   handler;
    u16     collectibleFlag;
    char    unk_142[0x8];
    s16     timer;
    char    unk_14c[0x50];
#if defined(GAME_MM)
    char    unk_mm[0x4];
#endif
    Xflag   xflag;
    u8      isExtended:1;
    u8      isExtendedCollected:1;
    u8      isExtendedMajor:1;
    u8      isDecoy:1;
    s16     xflagGi;
}
Actor_EnItem00;

#if defined(GAME_OOT)
_Static_assert(sizeof(Actor_EnItem00) == 0x1a4, "OoT Actor_EnItem00 size is wrong");
#else
_Static_assert(sizeof(Actor_EnItem00) == 0x1b0, "MM Actor_EnItem00 size is wrong");
#endif

typedef struct GameState_Play GameState_Play;
void EnItem00_Init(Actor_EnItem00* this, GameState_Play* play);
void EnItem00_Update(Actor_EnItem00* this, GameState_Play* play);
void EnItem00_DrawRupee(Actor_EnItem00* this, GameState_Play* play);
void EnItem00_CollectedHandler(Actor_EnItem00* this, GameState_Play* play);
void EnItem00_SetHandler(Actor_EnItem00* this, void* handler);
s16  EnItem00_FixDrop(s16 dropType);

void            EnItem00_XflagInit(Actor_EnItem00* this, const Xflag* xflag);
void            EnItem00_XflagInitFreestanding(Actor_EnItem00* this, GameState_Play* play, u8 actorIndex, u8 slice);
Actor_EnItem00* EnItem00_DropCustom(GameState_Play* play, const Vec3f* pos, const Xflag* xflag);
Actor_EnItem00* EnItem00_SpawnDecoy(GameState_Play* play, s16 gi);

#endif
