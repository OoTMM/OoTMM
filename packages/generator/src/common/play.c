#include <combo.h>
#include <combo/play.h>
#include <combo/entrance.h>
#include <combo/context.h>
#include <combo/multi.h>

#if defined(GAME_OOT)
# define ENTR_INTERNAL_EXIT_GROTTO ENTR_OOT_INTERNAL_EXIT_GROTTO
#endif

#if defined(GAME_MM)
# define ENTR_INTERNAL_EXIT_GROTTO ENTR_MM_INTERNAL_EXIT_GROTTO
#endif

int Play_MergeMQ(int sceneId)
{
    switch (sceneId)
    {
    case SCE_OOT_DEKU_TREE_MQ: return SCE_OOT_DEKU_TREE;
    case SCE_OOT_DODONGO_CAVERN_MQ: return SCE_OOT_DODONGO_CAVERN;
    case SCE_OOT_INSIDE_JABU_JABU_MQ: return SCE_OOT_INSIDE_JABU_JABU;
    case SCE_OOT_TEMPLE_FOREST_MQ: return SCE_OOT_TEMPLE_FOREST;
    case SCE_OOT_TEMPLE_FIRE_MQ: return SCE_OOT_TEMPLE_FIRE;
    case SCE_OOT_TEMPLE_WATER_MQ: return SCE_OOT_TEMPLE_WATER;
    case SCE_OOT_TEMPLE_SPIRIT_MQ: return SCE_OOT_TEMPLE_SPIRIT;
    case SCE_OOT_TEMPLE_SHADOW_MQ: return SCE_OOT_TEMPLE_SHADOW;
    case SCE_OOT_BOTTOM_OF_THE_WELL_MQ: return SCE_OOT_BOTTOM_OF_THE_WELL;
    case SCE_OOT_ICE_CAVERN_MQ: return SCE_OOT_ICE_CAVERN;
    case SCE_OOT_GERUDO_TRAINING_GROUND_MQ: return SCE_OOT_GERUDO_TRAINING_GROUND;
    case SCE_OOT_INSIDE_GANON_CASTLE_MQ: return SCE_OOT_INSIDE_GANON_CASTLE;
    }

    return sceneId;
}

#if defined(GAME_OOT)
static u32 Play_HandleCrossFaroreEntrance(void)
{
    u32 entrance;
    u8 fwAge = gSave.age;
    RespawnData* fw = &gSharedCustomSave.mm.fw[fwAge];
    if (fw->data <= 0 || fw->entrance == ENTR_FW_CROSS)
    {
        gSave.info.fw.set = 0;

        gIsEntranceOverride = 0;
        return gSave.entrance;
    }
    entrance = fw->entrance | MASK_FOREIGN_ENTRANCE;
    gComboCtx.isFwSpawn = 1;
    gComboCtx.fwSpawnAge = fwAge;
    return entrance;
}

static u32 Play_SetDefaultGrottoRespawn(void)
{
    gIsEntranceOverride = 0;
    gSaveContext.respawnFlag = 2;
    gSaveContext.nextTransitionType = 3;
    return gSaveContext.respawn[1].entrance;
}
#endif

#if defined(GAME_MM)
static u32 Play_HandleCrossFaroreEntrance(void)
{
    u32 entrance;

    u8 fwAge = comboMmFwAge();
    OotFaroreWind* fw = Age_GetFaroreOot(fwAge);
    if (fw->set <= 0 || fw->entrance == ENTR_FW_CROSS)
    {
        gCustomSave.fw[fwAge].data = 0;
        gSaveContext.respawn[RESPAWN_MODE_HUMAN].data = 0;

        gIsEntranceOverride = 0;
        return gSave.entrance;
    }
    entrance = fw->entrance | MASK_FOREIGN_ENTRANCE;
    gComboCtx.isFwSpawn = 1;
    gComboCtx.fwSpawnAge = fwAge;
    return entrance;
}

static u32 Play_SetDefaultGrottoRespawn(void)
{
    gIsEntranceOverride = 0;
    gSaveContext.respawnFlag = 4;
    return gSaveContext.respawn[3].entrance;
}
#endif

void Play_TransitionDone(PlayState* play)
{
    u32 entrance;
    u32 originalEntrance;
    u32 fullEntrance;
    s32 override;
    u8 age;

    /* Resolve extended entrance */
    entrance = play->nextEntrance;
    if (entrance == ENTR_EXTENDED)
        entrance = g.nextEntrance;

    /* Resolve special entrances */
    switch (entrance)
    {
    case ENTR_FW_CROSS:
        entrance = Play_HandleCrossFaroreEntrance();
        break;
    case ENTR_CROSS_RESPAWN:
        entrance = gSharedCustomSave.respawn[CUSTOM_RESPAWN_MODE_DUNGEON_ENTRANCE].entrance | MASK_FOREIGN_ENTRANCE;
        gComboCtx.isDungeonEntranceSpawn = 1;
        break;
    }

    fullEntrance = entrance;

    /* Handle grotto exits */
    if (entrance == ENTR_INTERNAL_EXIT_GROTTO)
    {
        fullEntrance = Play_GrottoExitEntrance(play);
        if (Config_Flag(CFG_ER_GROTTOS))
            entrance = fullEntrance;
        if (entrance == ENTR_INTERNAL_EXIT_GROTTO)
            entrance = Play_SetDefaultGrottoRespawn();
    }

    /* Handle transition override */
    if (gIsEntranceOverride)
    {
        originalEntrance = fullEntrance;
        gIsEntranceOverride = 0;
        override = comboEntranceOverride(Play_EntranceForOverride(entrance));
        if (override != -1)
        {
            entrance = (u32)override;
            fullEntrance = entrance;
        }
#if defined(GAME_MM)
        g.isNextEntranceInitialSong = (entrance == ENTR_MM_CLOCK_TOWN_FROM_CLOCK_TOWER);
#endif
    }
    else
    {
        originalEntrance = 0xffffffff;
    }

    /* Send tracking infos */
#if defined(GAME_MM)
    fullEntrance ^= MASK_FOREIGN_ENTRANCE;
    if (originalEntrance != 0xffffffff)
        originalEntrance ^= MASK_FOREIGN_ENTRANCE;
    age = gOotSave.age;
#endif

#if defined(GAME_OOT)
    age = play->linkAgeOnLoad;
#endif
    Multi_InfoEntrance(originalEntrance, fullEntrance, age);

    entrance = Play_ApplyCustomEntrance(entrance);

    if (entrance & MASK_FOREIGN_ENTRANCE)
    {
        Play_GameSwitch(play, entrance & ~MASK_FOREIGN_ENTRANCE);
    }
    else
    {
        play->nextEntrance = entrance & 0xffff;
    }
}
