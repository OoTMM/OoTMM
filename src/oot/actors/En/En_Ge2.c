#include <combo.h>

int EnGe2_IsPeaceful(void)
{
    return gSave.quest.gerudoCard;
}

PATCH_FUNC(0x80b23e68, EnGe2_IsPeaceful);
