#include <combo.h>
#include <combo/common/scene.h>
#include <combo/common/cosmetics.h>

static EntranceTableEntry defaultEntrance = {
    0, /* Deku Tree */
    0, /* From Entrance */
    TRANS_TYPE_FADE_BLACK
};

EntranceTableEntry* Entrance_GetTableEntryCustom(u16 entrance)
{
    if (entrance >= 0x1000)
    {
        return &defaultEntrance;
    }

    EntranceTableEntry* gEntranceTable = (EntranceTableEntry*)(0x800f9c90);

    return &gEntranceTable[entrance];
}

/**
 * Returns the transition effect flags from a given entrance index.
 */
s32 Entrance_GetTransitionFlags(u16 entrance) {
    return Entrance_GetTableEntryCustom(entrance)->flags;
}

u8 gNightBgm;
EXPORT_SYMBOL(NIGHT_BGM, gNightBgm);

void Scene_CommandSoundSettings(PlayState* play, SceneCmd* cmd) {
    u8 natureAmbienceId;

    natureAmbienceId = cmd->soundSettings.natureAmbienceId;
    if(gNightBgm)
        natureAmbienceId = 0x13;

    play->sceneSequences.seqId = cmd->soundSettings.seqId;
    play->sceneSequences.natureAmbienceId = natureAmbienceId;

    if (gSaveContext.seqId == (u8)NA_BGM_DISABLED) {
        SEQCMD_RESET_AUDIO_HEAP(0, cmd->soundSettings.specId);
    }
}

PATCH_FUNC(0x80082478, Scene_CommandSoundSettings);
