#ifndef COMBO_H
#define COMBO_H

#include <PR/ultratypes.h>
#include <save.h>
#include <combo/alloc.h>
#include <combo/file.h>

#define GAME_NONE 0
#define GAME_OOT  1
#define GAME_MM   2

typedef struct
{
    void*   ram;
    u32     rom;
    u32     size;
    void (*entrypoint)(void);
}
GameConfig;

extern const GameConfig gLoaderGameConfigs[];

extern char gGameSwitch;
extern char gGameId;
extern char gNextGameId;
extern char gGameStarted;

void Game_Switch(int gameId);

/* Init */
void Combo_Init(void);

#endif
