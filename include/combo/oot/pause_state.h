#ifndef COMBO_PAUSE_STATE_H
#define COMBO_PAUSE_STATE_H

#include <combo/oot/pause_state_defs.h>

// Sets warp points for owl statues
typedef enum {
    /* 0x0 */ OWL_WARP_GREAT_BAY_COAST,
    /* 0x1 */ OWL_WARP_ZORA_CAPE,
    /* 0x2 */ OWL_WARP_SNOWHEAD,
    /* 0x3 */ OWL_WARP_MOUNTAIN_VILLAGE,
    /* 0x4 */ OWL_WARP_CLOCK_TOWN,
    /* 0x5 */ OWL_WARP_MILK_ROAD,
    /* 0x6 */ OWL_WARP_WOODFALL,
    /* 0x7 */ OWL_WARP_SOUTHERN_SWAMP,
    /* 0x8 */ OWL_WARP_IKANA_CANYON,
    /* 0x9 */ OWL_WARP_STONE_TOWER,
    /* 0xA */ OWL_WARP_ENTRANCE, // Special index for warping to the entrance of a scene
    /* 0xB */ OWL_WARP_MAX
} OwlWarpId;

// Sets cursor point options on the world map
typedef enum {
    /* 0x0 */ REGION_GREAT_BAY,
    /* 0x1 */ REGION_ZORA_HALL,
    /* 0x2 */ REGION_ROMANI_RANCH,
    /* 0x3 */ REGION_DEKU_PALACE,
    /* 0x4 */ REGION_WOODFALL,
    /* 0x5 */ REGION_CLOCK_TOWN,
    /* 0x6 */ REGION_SNOWHEAD,
    /* 0x7 */ REGION_IKANA_GRAVEYARD,
    /* 0x8 */ REGION_IKANA_CANYON,
    /* 0x9 */ REGION_GORON_VILLAGE,
    /* 0xA */ REGION_STONE_TOWER,
    /* 0xB */ REGION_MAX
} RegionId;

typedef enum {
    /* 0x00 */ PAUSE_MAIN_STATE_IDLE, // Await input for the next action
    /* 0x01 */ PAUSE_MAIN_STATE_SWITCHING_PAGE,
    /* 0x02 */ PAUSE_MAIN_STATE_SONG_PLAYBACK,
    /* 0x03 */ PAUSE_MAIN_STATE_EQUIP_ITEM,
    /* 0x04 */ PAUSE_MAIN_STATE_SONG_PROMPT_INIT,
    /* 0x05 */ PAUSE_MAIN_STATE_SONG_PROMPT,
    /* 0x06 */ PAUSE_MAIN_STATE_SONG_PROMPT_DONE,
    /* 0x07 */ PAUSE_MAIN_STATE_SONG_PROMPT_UNUSED,
    /* 0x08 */ PAUSE_MAIN_STATE_IDLE_CURSOR_ON_SONG, // Await input but the cursor is on a song
    /* 0x09 */ PAUSE_MAIN_STATE_SONG_PLAYBACK_INIT,
    /* 0x0F */ PAUSE_MAIN_STATE_EQUIP_MASK = 0xF,
    /* 0x10 */ PAUSE_MAIN_STATE_BOMBERS_NOTEBOOK_OPEN,
    /* 0x11 */ PAUSE_MAIN_STATE_UNK
} PauseMainState;

typedef enum {
    // 0..59 are the 15 background textures
    /*  0 */ QUAD_MAP_PAGE_WORLD_BG_FIRST,
    /* 14 */ QUAD_MAP_PAGE_WORLD_BG_LAST = PAGE_BG_QUADS - 1,
    // Clouds covering the world map
    /* 15 */ QUAD_MAP_PAGE_WORLD_CLOUDS_FIRST,
    /* 29 */ QUAD_MAP_PAGE_WORLD_CLOUDS_LAST = QUAD_MAP_PAGE_WORLD_CLOUDS_FIRST + WORLD_MAP_NUM_CLOUDS - 1,
    // Markers for regions
    /* 30 */ QUAD_MAP_PAGE_WORLD_REGION_FIRST,
    /* 40 */ QUAD_MAP_PAGE_WORLD_REGION_LAST = QUAD_MAP_PAGE_WORLD_REGION_FIRST + REGION_MAX - 1,
    // Markers for owl warps
    /* 41 */ QUAD_MAP_PAGE_WORLD_WARP_FIRST,
    /* 50 */ QUAD_MAP_PAGE_WORLD_WARP_LAST = QUAD_MAP_PAGE_WORLD_WARP_FIRST + OWL_WARP_ENTRANCE - 1,
    // Fragments of the world map
    /* 51 */ QUAD_MAP_PAGE_WORLD_IMAGE_FIRST,
    /* 65 */ QUAD_MAP_PAGE_WORLD_IMAGE_LAST = QUAD_MAP_PAGE_WORLD_IMAGE_FIRST + WORLD_MAP_IMAGE_FRAG_NUM - 1,
    /* 66 */ QUAD_MAP_PAGE_WORLD_MAX
} WorldMapPageQuad;


typedef enum {
    /* 0 */ VTX_PAGE_MAP_WORLD,
    /* 1 */ VTX_PAGE_MAX
} VtxPage;

typedef enum {
    /* 0 */ PAUSE_QUAD_CURSOR_0,
    /* 1 */ PAUSE_QUAD_CURSOR_1,
    /* 2 */ PAUSE_QUAD_CURSOR_2,
    /* 3 */ PAUSE_QUAD_CURSOR_3,
    /* 4 */ PAUSE_QUAD_CURSOR_4,
    /* 5 */ PAUSE_QUAD_CURSOR_MAX
} PauseCursorQuad;

#endif
