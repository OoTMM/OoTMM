#include "z_title_setup.h"

#include "overlays/gamestates/ovl_title/z_title.h"

void Setup_InitRegs(void) {
    XREG(2) = 0;
    XREG(10) = 26;
    XREG(11) = 20;
    XREG(12) = 14;
    XREG(13) = 0;
    R_A_BTN_Y_OFFSET = 0;
    R_MAGIC_CONSUME_TIMER_GIANTS_MASK = 80;

    R_THREE_DAY_CLOCK_Y_POS = 64596;
    R_THREE_DAY_CLOCK_SUN_MOON_CUTOFF = 215;
    R_THREE_DAY_CLOCK_HOUR_DIGIT_CUTOFF = 218;

    XREG(68) = 0x61;
    XREG(69) = 0x93;
    XREG(70) = 0x28;
    XREG(73) = 0x1E;
    XREG(74) = 0x42;
    XREG(75) = 0x1E;
    XREG(76) = 0x1C;
    XREG(77) = 0x3C;
    XREG(78) = 0x2F;
    XREG(79) = 0x62;
    R_PAUSE_OWL_WARP_ALPHA = 0;
    XREG(88) = 0x56;
    XREG(89) = 0x258;
    XREG(90) = 0x1C2;

    R_STORY_FILL_SCREEN_ALPHA = 0;
    R_PLAYER_FLOOR_REVERSE_INDEX = 0;
    R_MINIMAP_DISABLED = false;

    R_PICTO_FOCUS_BORDER_TOPLEFT_X = 80;
    R_PICTO_FOCUS_BORDER_TOPLEFT_Y = 60;
    R_PICTO_FOCUS_BORDER_TOPRIGHT_X = 220;
    R_PICTO_FOCUS_BORDER_TOPRIGHT_Y = 60;
    R_PICTO_FOCUS_BORDER_BOTTOMLEFT_X = 80;
    R_PICTO_FOCUS_BORDER_BOTTOMLEFT_Y = 160;
    R_PICTO_FOCUS_BORDER_BOTTOMRIGHT_X = 220;
    R_PICTO_FOCUS_BORDER_BOTTOMRIGHT_Y = 160;
    R_PICTO_FOCUS_ICON_X = 142;
    R_PICTO_FOCUS_ICON_Y = 108;
    R_PICTO_FOCUS_TEXT_X = 204;
    R_PICTO_FOCUS_TEXT_Y = 177;
}

static void StartGame(void)
{
    u32 i;

    /* Taken from Sram_OpenSave */
    gMmSave.saveInfo.playerData.magicLevel = 0;
    /* Owl checks */
    if ((gMmSave.entrance == ENTRANCE(SOUTHERN_SWAMP_POISONED, 10)) &&
        CHECK_WEEKEVENTREG(WEEKEVENTREG_CLEARED_WOODFALL_TEMPLE)) {
        gMmSave.entrance = ENTRANCE(SOUTHERN_SWAMP_CLEARED, 10);
    } else if ((gMmSave.entrance == ENTRANCE(MOUNTAIN_VILLAGE_WINTER, 8)) &&
                CHECK_WEEKEVENTREG(WEEKEVENTREG_CLEARED_SNOWHEAD_TEMPLE)) {
        gMmSave.entrance = ENTRANCE(MOUNTAIN_VILLAGE_SPRING, 8);
    }

    for (i = 0; i < ARRAY_COUNT(gSaveContext.cycleSceneFlags); i++) {
        gSaveContext.cycleSceneFlags[i].chest = gMmSave.saveInfo.permanentSceneFlags[i].chest;
        gSaveContext.cycleSceneFlags[i].switch0 = gMmSave.saveInfo.permanentSceneFlags[i].switch0;
        gSaveContext.cycleSceneFlags[i].switch1 = gMmSave.saveInfo.permanentSceneFlags[i].switch1;
        gSaveContext.cycleSceneFlags[i].clearedRoom = gMmSave.saveInfo.permanentSceneFlags[i].clearedRoom;
        gSaveContext.cycleSceneFlags[i].collectible = gMmSave.saveInfo.permanentSceneFlags[i].collectible;
    }

    if (gMmSave.saveInfo.scarecrowSpawnSongSet) {
        Lib_MemCpy(gScarecrowSpawnSongPtr, gMmSave.saveInfo.scarecrowSpawnSong, sizeof(gMmSave.saveInfo.scarecrowSpawnSong));
    }

    /* Taken from File Select */
    gSaveContext.gameMode = GAMEMODE_NORMAL;
    gSaveContext.respawnFlag = 0;
    gSaveContext.respawn[RESPAWN_MODE_DOWN].entrance = ENTR_LOAD_OPENING;
    gSaveContext.seqId = (u8)NA_BGM_DISABLED;
    gSaveContext.ambienceId = AMBIENCE_ID_DISABLED;
    gSaveContext.showTitleCard = true;
    gSaveContext.dogParams = 0;

    for (i = 0; i < TIMER_ID_MAX; i++) {
        gSaveContext.timerStates[i] = TIMER_STATE_OFF;
    }

    gSaveContext.prevHudVisibility = HUD_VISIBILITY_ALL;
    gSaveContext.nayrusLoveTimer = 0;
    gSaveContext.healthAccumulator = 0;
    gSaveContext.magicFlag = 0;
    gSaveContext.forcedSeqId = 0;
    gSaveContext.skyboxTime = CLOCK_TIME(0, 0);
    gSaveContext.nextTransitionType = TRANS_NEXT_TYPE_DEFAULT;
    gSaveContext.cutsceneTrigger = 0;
    gSaveContext.chamberCutsceneNum = 0;
    gSaveContext.nextDayTime = NEXT_TIME_NONE;
    gSaveContext.retainWeatherMode = false;

    gSaveContext.buttonStatus[EQUIP_SLOT_B] = BTN_ENABLED;
    gSaveContext.buttonStatus[EQUIP_SLOT_C_LEFT] = BTN_ENABLED;
    gSaveContext.buttonStatus[EQUIP_SLOT_C_DOWN] = BTN_ENABLED;
    gSaveContext.buttonStatus[EQUIP_SLOT_C_RIGHT] = BTN_ENABLED;
    gSaveContext.buttonStatus[EQUIP_SLOT_A] = BTN_ENABLED;

    gSaveContext.hudVisibilityForceButtonAlphasByStatus = false;
    gSaveContext.nextHudVisibility = HUD_VISIBILITY_IDLE;
    gSaveContext.hudVisibility = HUD_VISIBILITY_IDLE;
    gSaveContext.hudVisibilityTimer = 0;

    gMmSave.saveInfo.playerData.tatlTimer = 0;

    /* Custom changes */
    gSaveContext.nextCutsceneIndex = 0;
    gMmSave.cutsceneIndex = 0;
}

void Setup_InitImpl(SetupState* this) {
    SaveContext_Init();
    Setup_InitRegs();
    StartGame();
    gMmSave.entrance = 0xd800; /* Entrance to Clock Town */

    STOP_GAMESTATE(&this->state);
    SET_NEXT_GAMESTATE(&this->state, Play_Init, sizeof(PlayState));
}

void Setup_Destroy(GameState* thisx) {
}

void Setup_Init(GameState* thisx) {
    SetupState* this = (SetupState*)thisx;

    this->state.destroy = Setup_Destroy;
    Setup_InitImpl(this);
}
