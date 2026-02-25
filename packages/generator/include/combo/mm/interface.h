#ifndef MM_INTERFACE_H
#define MM_INTERFACE_H

#include <combo/util.h>

#define PERFECT_LETTERS_NUM_LETTERS 8

typedef struct
{
    /* 0x000 */ View view;
    /* 0x168 */ Vtx* actionVtx;
    /* 0x16C */ Vtx* beatingHeartVtx;
    /* 0x170 */ u8* parameterSegment;
    /* 0x174 */ u8* doActionSegment;
    /* 0x178 */ u8* iconItemSegment;
    /* 0x17C */ u8* mapSegment;
    /* 0x180 */ char unk_180[0x4]; // unused segment?
    /* 0x184 */ DmaRequest dmaRequest;
    /* 0x1A4 */ char unk_1a4[0x40]; // likely 2 DmaRequest
    /* 0x1E4 */ OSMesgQueue loadQueue;
    /* 0x1FC */ OSMesg loadMsg;
    /* 0x200 */ Viewport viewport;
    /* 0x210 */ s16 aButtonState;
    /* 0x212 */ u16 aButtonDoActionDelayed; // Like aButtonDoAction, but does not update until the A button animation is complete
    /* 0x214 */ u16 aButtonDoAction;
    /* 0x218 */ f32 aButtonRoll;
    /* 0x21C */ s16 bButtonPlayerDoActionActive;
    /* 0x21E */ s16 bButtonPlayerDoAction;
    /* 0x220 */ s16 tatlCalling;
    /* 0x222 */ s16 bButtonInterfaceDoActionActive;
    /* 0x224 */ s16 bButtonInterfaceDoAction;
    /* 0x226 */ s16 lifeColorChange;
    /* 0x228 */ s16 lifeColorChangeDirection;
    /* 0x22A */ s16 beatingHeartPrim[3];
    /* 0x230 */ s16 beatingHeartEnv[3];
    /* 0x236 */ s16 heartsPrimR[2];
    /* 0x23A */ s16 heartsPrimG[2];
    /* 0x23E */ s16 heartsPrimB[2];
    /* 0x242 */ s16 heartsEnvR[2];
    /* 0x246 */ s16 heartsEnvG[2];
    /* 0x24A */ s16 heartsEnvB[2];
    /* 0x24E */ s16 health;
    /* 0x250 */ s16 healthTimer;
    /* 0x252 */ s16 lifeSizeChange;
    /* 0x254 */ s16 lifeSizeChangeDirection; // 1 means shrinking, 0 growing
    /* 0x256 */ char unk_256[0x2];
    /* 0x258 */ s16 magicConsumptionTimer; // For certain magic states, 1 unit of magic is consumed every time the timer reaches 0
    /* 0x25A */ u8 numHorseBoosts;
    /* 0x25C */ u16 minigamePoints; // Points to add to the minigame score. Reset to 0 every frame.
    /* 0x25E */ u16 minigameHiddenPoints; // Points to add to the secondary set of minigame points not displayed. Reset to 0 every frame.
    /* 0x260 */ u16 minigameAmmo;
    /* 0x262 */ u16 minigameUnusedPoints; // Associated with other minigame points, unused
    /* 0x264 */ s16 screenFillAlpha;
    /* 0x266 */ s16 aAlpha;
    /* 0x268 */ s16 bAlpha;
    /* 0x26A */ s16 cLeftAlpha;
    /* 0x26C */ s16 cDownAlpha;
    /* 0x26E */ s16 cRightAlpha;
    /* 0x270 */ s16 healthAlpha;
    /* 0x272 */ s16 magicAlpha;
    /* 0x274 */ s16 minimapAlpha;
    /* 0x276 */ s16 startAlpha;
    /* 0x278 */ s16 unk_278; // Set in Map_Init, never read
    /* 0x27A */ s16 dungeonSceneIndex;
    /* 0x27C */ s16 mapRoomNum;
    /* 0x27E */ u8 fishingHUDControl; // Left over from OoT
    /* 0x27F */ char unk_27f;
    /* 0x280 */ u8 minigameState;
    /* 0x282 */ s16 minigameCountdownAlpha;
    /* 0x284 */ s16 minigameCountdownScale;
    /* 0x286 */ s16 perfectLettersOn;
    /* 0x288 */ s16 perfectLettersType;
    /* 0x28A */ s16 perfectLettersState[PERFECT_LETTERS_NUM_LETTERS];
    /* 0x29A */ u16 perfectLettersAngles[PERFECT_LETTERS_NUM_LETTERS]; // Angle that follows the projectory of an ellipse
    /* 0x2AA */ s16 perfectLettersOffsetX[PERFECT_LETTERS_NUM_LETTERS];
    /* 0x2BC */ f32 perfectLettersSemiAxisX[PERFECT_LETTERS_NUM_LETTERS];
    /* 0x2DC */ f32 perfectLettersSemiAxisY[PERFECT_LETTERS_NUM_LETTERS];
    /* 0x2FC */ s16 perfectLettersPrimColor[4];
    /* 0x304 */ s16 perfectLettersCount;
    /* 0x306 */ s16 perfectLettersUnused;
    /* 0x308 */ s16 perfectLettersColorIndex;
    /* 0x30A */ s16 perfectLettersColorTimer;
    /* 0x30C */ s16 perfectLettersTimer;
    struct PACKED {
        u8 hGauge;
        u8 bButton;
        u8 aButton;
        u8 tradeItems;
        u8 songOfTime;
        u8 songOfDoubleTime;
        u8 invSongOfTime;
        u8 songOfSoaring;
        u8 songOfStorms;
        u8 masks;
        u8 pictoBox;
        u8 all;
    } restrictions;
    /* 0x31A */ u8 storyState;
    /* 0x31B */ u8 storyType;
    /* 0x31C */ u8 storyDmaStatus;
    /* 0x320 */ OSMesgQueue storyMsgQueue;
    /* 0x338 */ OSMesg storyMsgBuf;
    /* 0x33C */ u8* storySegment;
    /* 0x340 */ uintptr_t storyAddr;
    /* 0x344 */ size_t storySize;
}
InterfaceContext;

ASSERT_OFFSET(InterfaceContext, view, 0x000);
ASSERT_OFFSET(InterfaceContext, actionVtx, 0x168);
ASSERT_OFFSET(InterfaceContext, beatingHeartVtx, 0x16C);
ASSERT_OFFSET(InterfaceContext, parameterSegment, 0x170);
ASSERT_OFFSET(InterfaceContext, doActionSegment, 0x174);
ASSERT_OFFSET(InterfaceContext, iconItemSegment, 0x178);
ASSERT_OFFSET(InterfaceContext, mapSegment, 0x17C);
ASSERT_OFFSET(InterfaceContext, unk_180, 0x180);
ASSERT_OFFSET(InterfaceContext, dmaRequest, 0x184);
ASSERT_OFFSET(InterfaceContext, unk_1a4, 0x1A4);
ASSERT_OFFSET(InterfaceContext, loadQueue, 0x1E4);
ASSERT_OFFSET(InterfaceContext, loadMsg, 0x1FC);
ASSERT_OFFSET(InterfaceContext, viewport, 0x200);
ASSERT_OFFSET(InterfaceContext, aButtonState, 0x210);
ASSERT_OFFSET(InterfaceContext, aButtonDoActionDelayed, 0x212);
ASSERT_OFFSET(InterfaceContext, aButtonDoAction, 0x214);
ASSERT_OFFSET(InterfaceContext, aButtonRoll, 0x218);
ASSERT_OFFSET(InterfaceContext, bButtonPlayerDoActionActive, 0x21C);
ASSERT_OFFSET(InterfaceContext, bButtonPlayerDoAction, 0x21E);
ASSERT_OFFSET(InterfaceContext, tatlCalling, 0x220);
ASSERT_OFFSET(InterfaceContext, bButtonInterfaceDoActionActive, 0x222);
ASSERT_OFFSET(InterfaceContext, bButtonInterfaceDoAction, 0x224);
ASSERT_OFFSET(InterfaceContext, lifeColorChange, 0x226);
ASSERT_OFFSET(InterfaceContext, lifeColorChangeDirection, 0x228);
ASSERT_OFFSET(InterfaceContext, beatingHeartPrim, 0x22A);
ASSERT_OFFSET(InterfaceContext, beatingHeartEnv, 0x230);
ASSERT_OFFSET(InterfaceContext, heartsPrimR, 0x236);
ASSERT_OFFSET(InterfaceContext, heartsPrimG, 0x23A);
ASSERT_OFFSET(InterfaceContext, heartsPrimB, 0x23E);
ASSERT_OFFSET(InterfaceContext, heartsEnvR, 0x242);
ASSERT_OFFSET(InterfaceContext, heartsEnvG, 0x246);
ASSERT_OFFSET(InterfaceContext, heartsEnvB, 0x24A);
ASSERT_OFFSET(InterfaceContext, health, 0x24E);
ASSERT_OFFSET(InterfaceContext, healthTimer, 0x250);
ASSERT_OFFSET(InterfaceContext, lifeSizeChange, 0x252);
ASSERT_OFFSET(InterfaceContext, lifeSizeChangeDirection, 0x254);
ASSERT_OFFSET(InterfaceContext, unk_256, 0x256);
ASSERT_OFFSET(InterfaceContext, magicConsumptionTimer, 0x258);
ASSERT_OFFSET(InterfaceContext, numHorseBoosts, 0x25A);
ASSERT_OFFSET(InterfaceContext, minigamePoints, 0x25C);
ASSERT_OFFSET(InterfaceContext, minigameHiddenPoints, 0x25E);
ASSERT_OFFSET(InterfaceContext, minigameAmmo, 0x260);
ASSERT_OFFSET(InterfaceContext, minigameUnusedPoints, 0x262);
ASSERT_OFFSET(InterfaceContext, screenFillAlpha, 0x264);
ASSERT_OFFSET(InterfaceContext, aAlpha, 0x266);
ASSERT_OFFSET(InterfaceContext, bAlpha, 0x268);
ASSERT_OFFSET(InterfaceContext, cLeftAlpha, 0x26A);
ASSERT_OFFSET(InterfaceContext, cDownAlpha, 0x26C);
ASSERT_OFFSET(InterfaceContext, cRightAlpha, 0x26E);
ASSERT_OFFSET(InterfaceContext, healthAlpha, 0x270);
ASSERT_OFFSET(InterfaceContext, magicAlpha, 0x272);
ASSERT_OFFSET(InterfaceContext, minimapAlpha, 0x274);
ASSERT_OFFSET(InterfaceContext, startAlpha, 0x276);
ASSERT_OFFSET(InterfaceContext, unk_278, 0x278);
ASSERT_OFFSET(InterfaceContext, dungeonSceneIndex, 0x27A);
ASSERT_OFFSET(InterfaceContext, mapRoomNum, 0x27C);
ASSERT_OFFSET(InterfaceContext, fishingHUDControl, 0x27E);
ASSERT_OFFSET(InterfaceContext, unk_27f, 0x27F);
ASSERT_OFFSET(InterfaceContext, minigameState, 0x280);
ASSERT_OFFSET(InterfaceContext, minigameCountdownAlpha, 0x282);
ASSERT_OFFSET(InterfaceContext, minigameCountdownScale, 0x284);
ASSERT_OFFSET(InterfaceContext, perfectLettersOn, 0x286);
ASSERT_OFFSET(InterfaceContext, perfectLettersType, 0x288);
ASSERT_OFFSET(InterfaceContext, perfectLettersState, 0x28A);
ASSERT_OFFSET(InterfaceContext, perfectLettersAngles, 0x29A);
ASSERT_OFFSET(InterfaceContext, perfectLettersOffsetX, 0x2AA);
ASSERT_OFFSET(InterfaceContext, perfectLettersSemiAxisX, 0x2BC);
ASSERT_OFFSET(InterfaceContext, perfectLettersSemiAxisY, 0x2DC);
ASSERT_OFFSET(InterfaceContext, perfectLettersPrimColor, 0x2FC);
ASSERT_OFFSET(InterfaceContext, perfectLettersCount, 0x304);
ASSERT_OFFSET(InterfaceContext, perfectLettersUnused, 0x306);
ASSERT_OFFSET(InterfaceContext, perfectLettersColorIndex, 0x308);
ASSERT_OFFSET(InterfaceContext, perfectLettersColorTimer, 0x30A);
ASSERT_OFFSET(InterfaceContext, perfectLettersTimer, 0x30C);
ASSERT_OFFSET(InterfaceContext, storyState, 0x31A);
ASSERT_OFFSET(InterfaceContext, storyType, 0x31B);
ASSERT_OFFSET(InterfaceContext, storyDmaStatus, 0x31C);
ASSERT_OFFSET(InterfaceContext, storyMsgQueue, 0x320);
ASSERT_OFFSET(InterfaceContext, storyMsgBuf, 0x338);
ASSERT_OFFSET(InterfaceContext, storySegment, 0x33C);
ASSERT_OFFSET(InterfaceContext, storyAddr, 0x340);
ASSERT_OFFSET(InterfaceContext, storySize, 0x344);

#endif
