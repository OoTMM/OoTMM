#include <combo.h>
#include <combo/instances.h>

int Instance_Get(PlayState* play)
{
    switch (play->sceneId)
    {
    case SCE_MM_GROTTOS:
        switch (play->roomCtx.curRoom.num)
        {
        case 0x04:
            /* Generic Grottos */
            switch (gGrottoData & 0x1f)
            {
            case 0x13: return INSTANCE_MM_GROTTO_GENERIC_PATH_TO_SNOWHEAD;
            case 0x14: return INSTANCE_MM_GROTTO_GENERIC_IKANA_VALLEY;
            case 0x15: return INSTANCE_MM_GROTTO_GENERIC_ZORA_CAPE;
            case 0x16: return INSTANCE_MM_GROTTO_GENERIC_ROAD_TO_IKANA;
            case 0x17: return INSTANCE_MM_GROTTO_GENERIC_GREAT_BAY_COAST;
            case 0x18: return INSTANCE_MM_GROTTO_GENERIC_IKANA_GRAVEYARD;
            case 0x19: return INSTANCE_MM_GROTTO_GENERIC_TWIN_ISLANDS;
            case 0x1a: return INSTANCE_MM_GROTTO_GENERIC_TERMINA_FIELD_PILLAR;
            case 0x1b: return INSTANCE_MM_GROTTO_GENERIC_MOUNTAIN_VILLAGE;
            case 0x1c: return INSTANCE_MM_GROTTO_GENERIC_WOODS_OF_MYSTERY;
            case 0x1d: return INSTANCE_MM_GROTTO_GENERIC_SOUTHERN_SWAMP;
            case 0x1e: return INSTANCE_MM_GROTTO_GENERIC_ROAD_TO_SOUTHERN_SWAMP;
            case 0x1f: return INSTANCE_MM_GROTTO_GENERIC_TERMINA_FIELD_TALL_GRASS;
            }
            break;
        case 0x0a:
            switch (gLastScene)
            {
            case SCE_MM_TERMINA_FIELD: return INSTANCE_MM_GROTTO_COW_TERMINA_FIELD;
            case SCE_MM_GREAT_BAY_COAST: return INSTANCE_MM_GROTTO_COW_GREAT_BAY_COAST;
            }
            break;
        }
        break;
    }

    return INSTANCE_NONE;
}
