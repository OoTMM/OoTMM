#include <combo.h>
#include <combo/custom.h>

s32 EnHorse_GetLinkHorseSlot(ObjectContext* objectCtx, s16 id)
{
    if (comboIsLinkAdult())
    {
        id = CUSTOM_OBJECT_ID_MM_ADULT_EPONA;
    }
    return Object_GetSlot(objectCtx, id);
}

PATCH_CALL(0x8087cb6c, EnHorse_GetLinkHorseSlot);

s32 EnHorse_SpawnPersistentLinkHorse(ObjectContext* objectCtx, s16 id)
{
    if (comboIsLinkAdult())
    {
        id = CUSTOM_OBJECT_ID_MM_ADULT_EPONA;
    }
    return Object_SpawnPersistent(objectCtx, id);
}

PATCH_CALL(0x8087cb88, EnHorse_SpawnPersistentLinkHorse);
