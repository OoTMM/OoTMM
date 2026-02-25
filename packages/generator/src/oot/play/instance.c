#include <combo.h>
#include <combo/play.h>
#include <combo/instances.h>
#include <combo/oot/save.h>

int Instance_Get(PlayState* play)
{
    switch (play->sceneId)
    {
    case SCE_OOT_GROTTOS:
        switch (play->roomCtx.curRoom.num)
        {
        case 0x00:
            /* Generic Grottos */
            switch (gGrottoData & 0x1f)
            {
            case 0x00: return INSTANCE_OOT_GROTTO_GENERIC_HYRULE_FIELD_MARKET;
            case 0x02: return INSTANCE_OOT_GROTTO_GENERIC_HYRULE_FIELD_SOUTHEAST;
            case 0x03: return INSTANCE_OOT_GROTTO_GENERIC_HYRULE_FIELD_OPEN;
            case 0x08: return INSTANCE_OOT_GROTTO_GENERIC_KAKARIKO;
            case 0x09: return INSTANCE_OOT_GROTTO_GENERIC_ZORA_RIVER;
            case 0x0c: return INSTANCE_OOT_GROTTO_GENERIC_KOKIRI_FOREST;
            case 0x14: return INSTANCE_OOT_GROTTO_GENERIC_LOST_WOODS;
            case 0x17: return INSTANCE_OOT_GROTTO_GENERIC_DEATH_MOUNTAIN_TRAIL;
            case 0x1a: return INSTANCE_OOT_GROTTO_GENERIC_DEATH_MOUNTAIN_CRATER;
            }
            break;
        case 0x09:
            switch (gLastScene)
            {
            case SCE_OOT_SACRED_FOREST_MEADOW: return INSTANCE_OOT_GROTTO_SCRUB2_SACRED_FOREST_MEADOW;
            case SCE_OOT_ZORA_RIVER: return INSTANCE_OOT_GROTTO_SCRUB2_ZORA_RIVER;
            case SCE_OOT_GERUDO_VALLEY: return INSTANCE_OOT_GROTTO_SCRUB2_GERUDO_VALLEY;
            case SCE_OOT_DESERT_COLOSSUS: return INSTANCE_OOT_GROTTO_SCRUB2_DESERT_COLOSSUS;
            }
            break;
        case 0x0c:
            switch (gLastScene)
            {
            case SCE_OOT_LON_LON_RANCH: return INSTANCE_OOT_GROTTO_SCRUB3_LON_LON_RANCH;
            case SCE_OOT_GORON_CITY: return INSTANCE_OOT_GROTTO_SCRUB3_GORON_CITY;
            case SCE_OOT_DEATH_MOUNTAIN_CRATER: return INSTANCE_OOT_GROTTO_SCRUB3_DEATH_MOUNTAIN_CRATER;
            case SCE_OOT_LAKE_HYLIA: return INSTANCE_OOT_GROTTO_SCRUB3_LAKE_HYLIA;
            }
            break;
        }
        break;
    case SCE_OOT_FAIRY_FOUNTAIN:
        switch (gLastScene)
        {
        case SCE_OOT_SACRED_FOREST_MEADOW: return INSTANCE_OOT_FAIRY_FOUNTAIN_SACRED_FOREST_MEADOW;
        case SCE_OOT_HYRULE_FIELD: return INSTANCE_OOT_FAIRY_FOUNTAIN_HYRULE_FIELD;
        case SCE_OOT_ZORA_RIVER: return INSTANCE_OOT_FAIRY_FOUNTAIN_ZORA_RIVER;
        case SCE_OOT_ZORA_DOMAIN: return INSTANCE_OOT_FAIRY_FOUNTAIN_ZORA_DOMAIN;
        case SCE_OOT_GERUDO_FORTRESS: return INSTANCE_OOT_FAIRY_FOUNTAIN_GERUDO_FORTRESS;
        }
        break;
    case SCE_OOT_GREAT_FAIRY_FOUNTAIN_UPGRADES:
        switch (play->spawn)
        {
        case 0: return INSTANCE_OOT_GREAT_FAIRY_FOUNTAIN_UPGRADES_DEATH_MOUNTAIN_TRAIL;
        case 1: return INSTANCE_OOT_GREAT_FAIRY_FOUNTAIN_UPGRADES_DEATH_MOUNTAIN_CRATER;
        case 2: return INSTANCE_OOT_GREAT_FAIRY_FOUNTAIN_UPGRADES_OUTSIDE_GANON_CASTLE;
        }
        break;
    case SCE_OOT_GREAT_FAIRY_FOUNTAIN_SPELLS:
        switch (play->spawn)
        {
        case 0: return INSTANCE_OOT_GREAT_FAIRY_FOUNTAIN_SPELLS_ZORA_FOUNTAIN;
        case 1: return INSTANCE_OOT_GREAT_FAIRY_FOUNTAIN_SPELLS_HYRULE_CASTLE;
        case 2: return INSTANCE_OOT_GREAT_FAIRY_FOUNTAIN_SPELLS_DESERT_COLOSSUS;
        }
        break;
    }

    return INSTANCE_NONE;
}
