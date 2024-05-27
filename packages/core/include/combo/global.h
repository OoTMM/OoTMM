#ifndef COMBO_GLOBAL_H
#define COMBO_GLOBAL_H

#include <combo/types.h>

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
    u8                      delayedSwitchFlag;
    u8                      roomEnemyLackSoul:1;
    u8                      spawnExtended:1;
    u8                      decoysCount;
    u8                      menuScreen;
    u8                      menuCursor;
    u8                      menuCursorMax;
    u8                      maxKeysOot[0x11];
    u8                      maxKeysMm[4];
    u8                      actorIndex;
    u8                      sceneSetupId;
    s8                      keatonGrassMax;
    u32                     nextEntrance;
}
ComboGlobal;

extern ComboGlobal g;

#endif
