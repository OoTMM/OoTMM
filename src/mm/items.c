#include <combo.h>

int SetChestItemInRange(void* actor, void* play, s16 itemId, float a, float b);

int hook_SetChestItemInRange(void* actor, void* play, s16 itemId, float a, float b)
{
    s32 override;
    itemId = -0xa;
    return SetChestItemInRange(actor, play, itemId, a, b);
}
