#include <combo.h>

#include <combo/config.h>
#include <combo/mm/cutscene.h>

#include <combo/mm/actors/En_Death.h>
#include <combo/mm/actors/Obj_Syokudai.h>

// static Actor *csActor;

static s32 CutsceneManager_ShouldCutscenePlay(s16 csId, Actor *actor)
{
    s32 shouldCsPlay = true;
    if (Config_Flag(CFG_MM_SKIP_CUTSCENES) && csId != CS_ID_NONE && actor != NULL)
    {
        switch (actor->id)
        {
        case ACTOR_OBJ_COMB:
        {
            if (gPlay->sceneId != SCE_MM_PIRATE_FORTRESS_INTERIOR)
            {
                shouldCsPlay = false;
            }
            break;
        }
        case ACTOR_EN_BOX:
        {
            if (gPlay->sceneId != SCE_MM_TREASURE_SHOP)
            {
                shouldCsPlay = false;
            }
            break;
        }
        case ACTOR_OBJ_SYOKUDAI:
        {
            ObjSyokudai *syokudai = (ObjSyokudai *)actor;
            if (syokudai->pendingAction >= OBJ_SYOKUDAI_PENDING_ACTION_CUTSCENE_AND_SWITCH)
                Flags_SetSwitch(gPlay, OBJ_SYOKUDAI_GET_SWITCH_FLAG(&syokudai->actor));

            syokudai->pendingAction = OBJ_SYOKUDAI_PENDING_ACTION_NONE;
            syokudai->snuffTimer = OBJ_SYOKUDAI_SNUFF_NEVER;
            shouldCsPlay = false;
            break;
        }
        case ACTOR_OBJ_BEAN:
        {
            if (((actor->params >> 0xe) & 3) == 0) // OBJBEAN_GET_C000(actor) == ENOBJBEAN_GET_C000_0
            {
                shouldCsPlay = false;
            }
            break;
        }
        case ACTOR_BG_ASTR_BOMBWALL:    // Astral Observatory - Bombable Wall
        case ACTOR_BG_DBLUE_BALANCE:    // Great Bay Temple - Seesaw and Waterwheel w/ Platforms
        case ACTOR_BG_DBLUE_MOVEBG:     // Moveable block in Great Bay Temple
        case ACTOR_BG_DBLUE_WATERFALL:  // Great Bay Temple - Freezable Waterfall
        case ACTOR_BG_HAKA_BOMBWALL:    // Beneath the Grave - Bombable Wall
        case ACTOR_BG_HAKUGIN_BOMBWALL: // Snowhead Temple - Bombable Wall
        case ACTOR_BG_HAKUGIN_ELVPOLE:  // Raisable pillar in Snowhead Temple Map room
        case ACTOR_BG_HAKUGIN_POST:     // Snowhead Temple Central Pillar
        case ACTOR_BG_HAKUGIN_SWITCH:   // Goron Link Switch
        case ACTOR_BG_IKANA_BLOCK:      // Stone Tower Temple - Rotating Room Pushblock
        case ACTOR_BG_IKANA_BOMBWALL:   // Stone Tower Temple - Bombable Tan Floor Tile
        case ACTOR_BG_IKANA_DHARMA:     // Stone Tower Temple - Punchable Pillar Segments
        case ACTOR_BG_IKNIN_SUSCEIL:    // Ikana Castle - Hot Checkered Ceiling
        case ACTOR_BG_KIN2_BOMBWALL:    // Ocean Spider House - Bombable Wall
        case ACTOR_BG_KIN2_PICTURE:     // Ocean Spider House - Skullkid Painting
        case ACTOR_BG_LADDER:           // Wooden Ladder
        case ACTOR_BG_NUMA_HANA:        // Big wooden flower in Woodfall Temple
        case ACTOR_BG_SPDWEB:           // Spider Web
        case ACTOR_BG_TOBIRA01:         // Gate to Goron Shrine
        case ACTOR_DOOR_SHUTTER:        // Studded Lifting Door / Ikana Castle Rolling Door
        case ACTOR_EN_BAL:              // Tingle
        case ACTOR_EN_CHA:              // Bell in Laundry Pool
        case ACTOR_EN_DRAGON:           // Deep Python
        case ACTOR_EN_MM:               // Rock Sirloin
        case ACTOR_EN_SW:               // Skullwalltula
        case ACTOR_OBJ_BIGICICLE:       // Large icicles
        case ACTOR_OBJ_CHAN:            // Goron Shrine chandelier
        case ACTOR_OBJ_FIRESHIELD:      // Ring of fire
        case ACTOR_OBJ_HAKAISI:         // Gravestone
        case ACTOR_OBJ_HUGEBOMBIWA:     // Boulder Blocking Goron Racetrack/Milk Road
        case ACTOR_OBJ_HUNSUI:          // Geyser in Great Bay Temple
        case ACTOR_OBJ_ICE_POLY:        // Large Block of Meltable Ice
        case ACTOR_OBJ_ICEBLOCK:        // Ice Block That Appears After Freezing Enemy
        case ACTOR_OBJ_LIGHTBLOCK:      // Sun Block
        case ACTOR_OBJ_LIGHTSWITCH:     // Sun Switch / STT Flip switch
        case ACTOR_OBJ_RAILLIFT:        // Moving Platform
        case ACTOR_OBJ_SPIDERTENT:      // Tent-Shaped Spider Web
        case ACTOR_OBJ_SWITCH:          // Floor and Eye Switches
        case ACTOR_OBJ_Y2SHUTTER:       // Pirates' Fortress sliding grated/barred shutters
        {
            shouldCsPlay = false;
            break;
        }
        }
    }
    return shouldCsPlay;
}

s16 CutsceneManager_StartHook(s16 csId, Actor *actor)
{
    if (!CutsceneManager_ShouldCutscenePlay(csId, actor))
    {
        // csActor = actor;
        actor->csId = CS_ID_NONE;
        Sfx_PlaySuccessChime();
        return CS_ID_NONE;
    }
    return _CutsceneManager_Start(csId, actor);
}
