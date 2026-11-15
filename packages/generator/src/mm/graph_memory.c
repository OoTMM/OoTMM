#include <combo.h>
#include <combo/game_state.h>

#define MM_SCREEN_WIDTH                  320
#define MM_SCREEN_HEIGHT                 240
#define MM_LORES_BUFFER_SIZE             (MM_SCREEN_WIDTH * MM_SCREEN_HEIGHT * sizeof(u16))
#define MM_GFX_OUTPUT_BUFFER_SIZE        0x18000

#define MM_HI_SCRATCH_ADDR               0x80784600u
#define MM_HI_FRAMEBUFFER_ADDR           0x807DA800u
#define MM_WORK_BUFFER_ADDR              (MM_HI_SCRATCH_ADDR + MM_LORES_BUFFER_SIZE)

#define MM_G_ZBUFFER_LORES               (*(void**)0x801FBBA4u)
#define MM_G_WORKBUFFER_LORES            (*(void**)0x801FBBA8u)
#define MM_G_GFX_OUTPUT_LORES            (*(void**)0x801FBBACu)
#define MM_G_GFX_OUTPUT_END_LORES        (*(void**)0x801FBBB0u)
#define MM_G_GFX_OUTPUT_HIRES            (*(void**)0x801FBBC4u)
#define MM_G_GFX_OUTPUT_END_HIRES        (*(void**)0x801FBBC8u)
#define MM_G_WORKBUFFER                  (*(void**)0x801FBB90u)
#define MM_GAMESTATE_OVERLAY_TABLE       ((GameStateOverlay*)0x801BD910u)
#define MM_SYSTEM_MALLOC                 ((void* (*)(size_t))0x80086DD0u)
#define MM_SYSTEM_FREE                   ((void (*)(void*))0x80086E50u)
#define MM_OVERLAY_LOAD_GAMESTATE        ((void (*)(GameStateOverlay*))0x800B3880u)
#define MM_OVERLAY_FREE_GAMESTATE        ((void (*)(GameStateOverlay*))0x800B39A4u)
#define MM_GAMESTATE_INIT                ((void (*)(GameState*, void*, GraphicsContext*))0x80173950u)
#define MM_GAMESTATE_DESTROY             ((void (*)(GameState*))0x80173A50u)
#define MM_GRAPH_GET_NEXT_GAMESTATE      ((GameStateOverlay* (*)(GameState*))0x80173F98u)
#define MM_GRAPH_INIT                    ((void (*)(GraphicsContext*))0x801740D0u)
#define MM_GRAPH_DESTROY                 ((void (*)(GraphicsContext*))0x80174174u)
#define MM_GRAPH_UPDATE                  ((void (*)(GraphicsContext*, GameState*))0x80174868u)
#define MM_SYS_CFB_INIT                  ((void (*)(void))0x80178978u)
#define MM_FAULT_SET_FRAMEBUFFER         ((void (*)(void*, u16, u16))0x80083B70u)

_Static_assert(MM_LORES_BUFFER_SIZE == 0x25800, "unexpected MM low-res buffer size");
_Static_assert(
    MM_WORK_BUFFER_ADDR + MM_LORES_BUFFER_SIZE <= MM_HI_FRAMEBUFFER_ADDR,
    "relocated MM work buffer overlaps fixed framebuffer");

static void Graph_ThreadEntryMemoryReclaim(void* arg)
{
    GraphicsContext gfxCtx;
    GameStateOverlay* nextOvl = &MM_GAMESTATE_OVERLAY_TABLE[0];
    GameStateOverlay* ovl;
    GameState* gameState;
    size_t size;
    uintptr_t zBufferAlloc;

    (void)arg;

    zBufferAlloc = (uintptr_t)MM_SYSTEM_MALLOC(MM_LORES_BUFFER_SIZE + 64 - 1);
    MM_G_ZBUFFER_LORES = (void*)((zBufferAlloc + 63) & ~(uintptr_t)63);
    MM_G_WORKBUFFER_LORES = (void*)MM_WORK_BUFFER_ADDR;
    MM_G_GFX_OUTPUT_LORES = MM_SYSTEM_MALLOC(MM_GFX_OUTPUT_BUFFER_SIZE);
    MM_G_GFX_OUTPUT_HIRES = MM_G_GFX_OUTPUT_LORES;
    MM_G_GFX_OUTPUT_END_LORES = (u8*)MM_G_GFX_OUTPUT_LORES + MM_GFX_OUTPUT_BUFFER_SIZE;
    MM_G_GFX_OUTPUT_END_HIRES = (u8*)MM_G_GFX_OUTPUT_HIRES + MM_GFX_OUTPUT_BUFFER_SIZE;

    MM_SYS_CFB_INIT();
    MM_FAULT_SET_FRAMEBUFFER(MM_G_WORKBUFFER, MM_SCREEN_WIDTH, MM_SCREEN_HEIGHT);
    MM_GRAPH_INIT(&gfxCtx);

    while (nextOvl)
    {
        ovl = nextOvl;
        MM_OVERLAY_LOAD_GAMESTATE(ovl);

        size = ovl->size;

        gameState = MM_SYSTEM_MALLOC(size);
        bzero(gameState, size);
        MM_GAMESTATE_INIT(gameState, ovl->ctor, &gfxCtx);

        while (gameState->running)
            MM_GRAPH_UPDATE(&gfxCtx, gameState);

        nextOvl = MM_GRAPH_GET_NEXT_GAMESTATE(gameState);

        MM_GAMESTATE_DESTROY(gameState);
        MM_SYSTEM_FREE(gameState);
        MM_OVERLAY_FREE_GAMESTATE(ovl);
    }

    MM_GRAPH_DESTROY(&gfxCtx);
}

PATCH_FUNC(0x801748A0, Graph_ThreadEntryMemoryReclaim);
