#include <combo.h>
#include <combo/entrance.h>

int comboIsEntranceValidEpona(u32 entranceId)
{
    switch (entranceId)
    {
    case ENTR_OOT_FIELD_FROM_GERUDO_VALLEY:
    case ENTR_OOT_GERUDO_VALLEY_FROM_FIELD:
    case ENTR_OOT_FIELD_FROM_LON_LON_RANCH:
    case ENTR_OOT_LON_LON_RANCH_FROM_FIELD:
    case ENTR_OOT_FIELD_FROM_LAKE_HYLIA:
    case ENTR_OOT_LAKE_HYLIA_FROM_FIELD:
    case ENTR_OOT_VALLEY_FROM_GERUDO_FORTRESS:
    case ENTR_OOT_GERUDO_FORTRESS_FROM_VALLEY:
        return 1;
    default:
        return 0;
    }
}
