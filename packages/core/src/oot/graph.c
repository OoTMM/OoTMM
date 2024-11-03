#include <combo.h>

void hookPlay_Init(GameState*);

GameStateOverlay* Graph_GetNextGameState(GameState* gameState)
{
    int i;
    void* newCtor;
    GameStateOverlay* ovl;

    newCtor = gameState->nextGameStateInit;
    if (newCtor == Play_Init)
        newCtor = hookPlay_Init;

    for (i = 0; i < 6; ++i)
    {
        ovl = gGameStateOverlayTable + i;
        if (ovl->ctor == newCtor)
            return ovl;
    }

    Fault_AddHungupAndCrashImpl("Bad GameState", "");

    return NULL;
}

PATCH_FUNC(0x800a1248, Graph_GetNextGameState);
