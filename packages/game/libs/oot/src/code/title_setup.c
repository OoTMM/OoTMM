#include <combo.h>
#include "global.h"

void Setup_InitImpl(SetupState* this) {
    PRINTF(T("ゼルダ共通データ初期化\n", "Zelda common data initialization\n"));
    SaveContext_Init();
    this->state.running = false;

    if (gGameStarted)
    {
        /* Prepare save loading */
        Sram_OnLoad();
        gOotSave.entranceIndex = 0x1d1; /* Market from mask shop */

        /* Enable audio */
        R_AUDIOMGR_DEBUG_LEVEL = 0;

        SET_NEXT_GAMESTATE(&this->state, Play_Init, PlayState);
    }
    else
    {
        SET_NEXT_GAMESTATE(&this->state, ConsoleLogo_Init, ConsoleLogoState);
    }
}

void Setup_Destroy(GameState* thisx) {
}

void Setup_Init(GameState* thisx) {
    SetupState* this = (SetupState*)thisx;

    this->state.destroy = Setup_Destroy;
    Setup_InitImpl(this);
}
