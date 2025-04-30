#ifndef COMBO_GLOBAL_H
#define COMBO_GLOBAL_H

#include <combo/types.h>
#include <combo/xflags.h>

typedef struct ComboItemQuery ComboItemQuery;

typedef struct
{
    void*                   customKeep;
    u32                     initialEntrance;
    u8                      inGrotto;
    u8                      isCreditWarp;
    u8                      isCredits;
    const ComboItemQuery*   itemQuery;
    const ComboItemQuery*   itemQueryBox;
    u8                      silverRupee;
    u8                      delayedSwitchFlags[4];
    u8                      delayedSwitchFlagsCount;
    u8                      roomEnemyLackSoul:1;
    u8                      spawnExtended:1;
    u8                      xflagOverride:1;
    u8                      isNextEntranceInitialSong:1;
    u8                      decoysCount;
    u8                      menuScreen;
    u8                      menuCursor;
    u8                      menuCursorMax;
    u8                      maxKeysOot[0x11];
    u8                      maxKeysMm[4];
    u8                      sceneSetupId;
    u32                     nextEntrance;
    s8                      prevRoom;
    Xflag                   xflag;
}
ComboGlobal;

extern ComboGlobal g;

#endif
