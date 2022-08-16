#include <combo.h>

int SetChestItemInRange(void* actor, void* play, s16 itemId, float a, float b);

int hook_SetChestItemInRange(void* actor, void* play, s16 itemId, float a, float b)
{
    u16 actorId;
    actorId = *(u16*)actor;
    if (actorId == 0x0a)
        itemId = -0x5;
    return SetChestItemInRange(actor, play, itemId, a, b);
}
