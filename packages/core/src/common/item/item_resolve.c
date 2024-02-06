#include <combo.h>

s16 comboItemResolve(GameState_Play* play, s16 gi)
{
    int isNeg;

    isNeg = 0;
    if (gi == GI_NONE)
        return 0;
    if (gi < 0)
    {
        gi = -gi;
        isNeg = 1;
    }

    /* Resolve scene-specific items into their global counterparts */
    switch (gi)
    {
    case GI_MM_STRAY_FAIRY:
        switch (comboStrayFairyIndex())
        {
        case 0: gi = GI_MM_STRAY_FAIRY_WF; break;
        case 1: gi = GI_MM_STRAY_FAIRY_SH; break;
        case 2: gi = GI_MM_STRAY_FAIRY_GB; break;
        case 3: gi = GI_MM_STRAY_FAIRY_ST; break;
        case 4: gi = GI_MM_STRAY_FAIRY_TOWN; break;
        }
        break;
    case GI_OOT_SMALL_KEY:
        switch (comboOotDungeonScene(play, 0))
        {
        case SCE_OOT_TEMPLE_FOREST:             gi = GI_OOT_SMALL_KEY_FOREST;   break;
        case SCE_OOT_TEMPLE_FIRE:               gi = GI_OOT_SMALL_KEY_FIRE;     break;
        case SCE_OOT_TEMPLE_WATER:              gi = GI_OOT_SMALL_KEY_WATER;    break;
        case SCE_OOT_TEMPLE_SHADOW:             gi = GI_OOT_SMALL_KEY_SHADOW;   break;
        case SCE_OOT_TEMPLE_SPIRIT:             gi = GI_OOT_SMALL_KEY_SPIRIT;   break;
        case SCE_OOT_BOTTOM_OF_THE_WELL:        gi = GI_OOT_SMALL_KEY_BOTW;     break;
        case SCE_OOT_INSIDE_GANON_CASTLE:       gi = GI_OOT_SMALL_KEY_GANON;    break;
        case SCE_OOT_GERUDO_TRAINING_GROUND:    gi = GI_OOT_SMALL_KEY_GTG;      break;
        case SCE_OOT_THIEVES_HIDEOUT:           gi = GI_OOT_SMALL_KEY_GF;       break;
        case SCE_OOT_TREASURE_SHOP:             gi = GI_OOT_SMALL_KEY_TCG;      break;
        }
        break;
    case GI_MM_SMALL_KEY:
        switch (comboMmDungeonIndex())
        {
        case 0: gi = GI_MM_SMALL_KEY_WF; break;
        case 1: gi = GI_MM_SMALL_KEY_SH; break;
        case 2: gi = GI_MM_SMALL_KEY_GB; break;
        case 3: gi = GI_MM_SMALL_KEY_ST; break;
        }
        break;
    case GI_OOT_KEY_RING:
        switch (comboOotDungeonScene(play, 0))
        {
        case SCE_OOT_TEMPLE_FOREST:             gi = GI_OOT_KEY_RING_FOREST;   break;
        case SCE_OOT_TEMPLE_FIRE:               gi = GI_OOT_KEY_RING_FIRE;     break;
        case SCE_OOT_TEMPLE_WATER:              gi = GI_OOT_KEY_RING_WATER;    break;
        case SCE_OOT_TEMPLE_SHADOW:             gi = GI_OOT_KEY_RING_SHADOW;   break;
        case SCE_OOT_TEMPLE_SPIRIT:             gi = GI_OOT_KEY_RING_SPIRIT;   break;
        case SCE_OOT_BOTTOM_OF_THE_WELL:        gi = GI_OOT_KEY_RING_BOTW;     break;
        case SCE_OOT_INSIDE_GANON_CASTLE:       gi = GI_OOT_KEY_RING_GANON;    break;
        case SCE_OOT_GERUDO_TRAINING_GROUND:    gi = GI_OOT_KEY_RING_GTG;      break;
        case SCE_OOT_THIEVES_HIDEOUT:           gi = GI_OOT_KEY_RING_GF;       break;
        case SCE_OOT_TREASURE_SHOP:             gi = GI_OOT_KEY_RING_TCG;      break;
        }
        break;
    case GI_MM_KEY_RING:
        switch (comboMmDungeonIndex())
        {
        case 0: gi = GI_MM_KEY_RING_WF; break;
        case 1: gi = GI_MM_KEY_RING_SH; break;
        case 2: gi = GI_MM_KEY_RING_GB; break;
        case 3: gi = GI_MM_KEY_RING_ST; break;
        }
        break;
    case GI_OOT_BOSS_KEY:
        switch (comboOotDungeonScene(play, 1))
        {
        case SCE_OOT_TEMPLE_FOREST:             gi = GI_OOT_BOSS_KEY_FOREST;   break;
        case SCE_OOT_TEMPLE_FIRE:               gi = GI_OOT_BOSS_KEY_FIRE;     break;
        case SCE_OOT_TEMPLE_WATER:              gi = GI_OOT_BOSS_KEY_WATER;    break;
        case SCE_OOT_TEMPLE_SHADOW:             gi = GI_OOT_BOSS_KEY_SHADOW;   break;
        case SCE_OOT_TEMPLE_SPIRIT:             gi = GI_OOT_BOSS_KEY_SPIRIT;   break;
        case SCE_OOT_GANON_TOWER:               gi = GI_OOT_BOSS_KEY_GANON;    break;
        }
        break;
    case GI_MM_BOSS_KEY:
        switch (comboMmDungeonIndex())
        {
        case 0: gi = GI_MM_BOSS_KEY_WF; break;
        case 1: gi = GI_MM_BOSS_KEY_SH; break;
        case 2: gi = GI_MM_BOSS_KEY_GB; break;
        case 3: gi = GI_MM_BOSS_KEY_ST; break;
        }
        break;
    case GI_OOT_MAP:
        switch (comboOotDungeonScene(play, 0))
        {
        case SCE_OOT_INSIDE_DEKU_TREE:          gi = GI_OOT_MAP_DT;             break;
        case SCE_OOT_DODONGO_CAVERN:            gi = GI_OOT_MAP_DC;             break;
        case SCE_OOT_INSIDE_JABU_JABU:          gi = GI_OOT_MAP_JJ;             break;
        case SCE_OOT_TEMPLE_FOREST:             gi = GI_OOT_MAP_FOREST;         break;
        case SCE_OOT_TEMPLE_FIRE:               gi = GI_OOT_MAP_FIRE;           break;
        case SCE_OOT_TEMPLE_WATER:              gi = GI_OOT_MAP_WATER;          break;
        case SCE_OOT_TEMPLE_SHADOW:             gi = GI_OOT_MAP_SHADOW;         break;
        case SCE_OOT_TEMPLE_SPIRIT:             gi = GI_OOT_MAP_SPIRIT;         break;
        case SCE_OOT_BOTTOM_OF_THE_WELL:        gi = GI_OOT_MAP_BOTW;           break;
        case SCE_OOT_ICE_CAVERN:                gi = GI_OOT_MAP_IC;             break;
        }
        break;
    case GI_MM_MAP:
        switch (comboMmDungeonIndex())
        {
        case 0: gi = GI_MM_MAP_WF; break;
        case 1: gi = GI_MM_MAP_SH; break;
        case 2: gi = GI_MM_MAP_GB; break;
        case 3: gi = GI_MM_MAP_ST; break;
        }
        break;
    case GI_OOT_COMPASS:
        switch (comboOotDungeonScene(play, 0))
        {
        case SCE_OOT_INSIDE_DEKU_TREE:          gi = GI_OOT_COMPASS_DT;             break;
        case SCE_OOT_DODONGO_CAVERN:            gi = GI_OOT_COMPASS_DC;             break;
        case SCE_OOT_INSIDE_JABU_JABU:          gi = GI_OOT_COMPASS_JJ;             break;
        case SCE_OOT_TEMPLE_FOREST:             gi = GI_OOT_COMPASS_FOREST;         break;
        case SCE_OOT_TEMPLE_FIRE:               gi = GI_OOT_COMPASS_FIRE;           break;
        case SCE_OOT_TEMPLE_WATER:              gi = GI_OOT_COMPASS_WATER;          break;
        case SCE_OOT_TEMPLE_SHADOW:             gi = GI_OOT_COMPASS_SHADOW;         break;
        case SCE_OOT_TEMPLE_SPIRIT:             gi = GI_OOT_COMPASS_SPIRIT;         break;
        case SCE_OOT_BOTTOM_OF_THE_WELL:        gi = GI_OOT_COMPASS_BOTW;           break;
        case SCE_OOT_ICE_CAVERN:                gi = GI_OOT_COMPASS_IC;             break;
        }
        break;
    case GI_MM_COMPASS:
        switch (comboMmDungeonIndex())
        {
        case 0: gi = GI_MM_COMPASS_WF; break;
        case 1: gi = GI_MM_COMPASS_SH; break;
        case 2: gi = GI_MM_COMPASS_GB; break;
        case 3: gi = GI_MM_COMPASS_ST; break;
        }
        break;
    }

    if (isNeg)
        gi = -gi;
    return gi;
}
