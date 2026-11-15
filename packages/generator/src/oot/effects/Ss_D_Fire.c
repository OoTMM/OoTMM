#include <combo.h>
#include <combo/enemy_room_models.h>

void* comboOotDodongoFireObject(ObjectContext* ctx, int slot)
{
    if (Object_GetSlot(ctx, 0x0c) != slot)
        return NULL;

    return comboObjectSlotSegmentAndPin(ctx, slot);
}
