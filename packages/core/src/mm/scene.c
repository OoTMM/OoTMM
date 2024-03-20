#include <combo.h>
#include <combo/custom.h>

static EntranceTableEntry defaultEntrance = {
    0, /* Southern Swamp (Clear) */
    0, /* From Road to Southern Swamp */
    TRANS_TYPE_FADE_BLACK
};

EntranceTableEntry* Entrance_GetTableEntryCustom(u16 entrance)
{
    if (entrance >= 0xF000)
    {
        return &defaultEntrance;
    }

    return Entrance_GetTableEntry(entrance);
}

PATCH_CALL(0x8013231c, Entrance_GetTableEntryCustom);
PATCH_CALL(0x80132348, Entrance_GetTableEntryCustom);
PATCH_CALL(0x80132384, Entrance_GetTableEntryCustom);
PATCH_CALL(0x801323b0, Entrance_GetTableEntryCustom);

s32 Scene_LoadHumanLinkObject(ObjectContext* objectCtx, s16 id)
{
    if (comboIsLinkAdult())
    {
        id = CUSTOM_OBJECT_ID_MM_ADULT_LINK;
    }
    return Object_SpawnPersistent(objectCtx, id);
}

PATCH_CALL(0x8012f854, Scene_LoadHumanLinkObject);

s32 Object_SpawnPersistentCustom(ObjectContext* objectCtx, s16 id)
{
    u32 size;

    objectCtx->slots[objectCtx->num].id = id;
    size = comboLoadObject(objectCtx->slots[objectCtx->num].segment, id);

    if (objectCtx->num < ARRAY_SIZE(objectCtx->slots) - 1)
    {
        objectCtx->slots[objectCtx->num + 1].segment = (void*)ALIGN16((u32)objectCtx->slots[objectCtx->num].segment + size);
    }

    objectCtx->num++;
    objectCtx->numPersistentEntries = objectCtx->num;

    return objectCtx->num - 1;
}

PATCH_FUNC(0x8012f2e0, Object_SpawnPersistentCustom);

static void UpdateGameplayKeepForAdult(ObjectContext* objectCtx)
{
    if (comboIsLinkAdult())
    {
        u32 gameplayKeep = (u32)objectCtx->slots[gPlay->objectCtx.mainKeepIndex].segment;
        comboLoadObject((void*)(gameplayKeep + 0x25510), CUSTOM_OBJECT_ID_MM_ADULT_LINK_SPIN_ATTACK_VTX_1);
        comboLoadObject((void*)(gameplayKeep + 0x25A90), CUSTOM_OBJECT_ID_MM_ADULT_LINK_SPIN_ATTACK_VTX_2);
        comboLoadObject((void*)(gameplayKeep + 0x26810), CUSTOM_OBJECT_ID_MM_ADULT_LINK_SPIN_ATTACK_VTX_3);
        comboLoadObject((void*)(gameplayKeep + 0x5A2A0), CUSTOM_OBJECT_ID_MM_ADULT_LINK_MASK_MTX);

        u32* spinAttackChargingDL1 = (u32*)(gameplayKeep + 0x268F0);
        spinAttackChargingDL1[0] = 0xde000000;
        spinAttackChargingDL1[1] = 0x0405a2e0;

        u32* maskMatrixDL = (u32*)(gameplayKeep + 0x5a2e0);
        maskMatrixDL[0] = 0xe7000000;
        maskMatrixDL[1] = 0;
        maskMatrixDL[2] = 0xda380001;
        maskMatrixDL[3] = 0x0405a2a0;
        maskMatrixDL[4] = 0xdf000000;
        maskMatrixDL[5] = 0;
    }
}

void Object_LoadAllCustom(ObjectContext* objectCtx)
{
    s32 i;
    s32 id;

    for (i = 0; i < objectCtx->num; i++)
    {
        id = objectCtx->slots[i].id;

        comboLoadObject(objectCtx->slots[i].segment, id);
    }

    UpdateGameplayKeepForAdult(objectCtx);
}

PATCH_FUNC(0x8012f698, Object_LoadAllCustom);

void* Object_AllocateSlotCustom(ObjectContext* objectCtx, s32 slot, s16 id)
{
    u32 addr;
    u32 vromSize;
    const ObjectData* fileTableEntry;

    objectCtx->slots[slot].id = -id;
    objectCtx->slots[slot].dmaRequest.vromAddr = 0;

    if (!id)
        return 0;

    fileTableEntry = comboGetObjectData(id);

    vromSize = fileTableEntry->vromEnd - fileTableEntry->vromStart;

    addr = ((u32)objectCtx->slots[slot].segment) + vromSize;
    addr = ALIGN16(addr);

    return (void*)addr;
}

PATCH_FUNC(0x8012f73c, Object_AllocateSlotCustom);

void Object_UpdateEntriesCustom(ObjectContext* objectCtx)
{
    s32 i;
    ObjectStatus* entry = &objectCtx->slots[0];
    const ObjectData* objectFile;
    u32 size;

    for (i = 0; i < objectCtx->num; i++)
    {
        if (entry->id < 0)
        {
            s16 id = -entry->id;

            if (entry->dmaRequest.vromAddr == 0)
            {
                objectFile = comboGetObjectData(id);
                size = objectFile->vromEnd - objectFile->vromStart;

                if (size == 0)
                {
                    entry->id = 0;
                }
                else
                {
                    osCreateMesgQueue(&entry->loadQueue, &entry->loadMsg, 1);
                    RequestDma(&entry->dmaRequest, entry->segment, objectFile->vromStart, size, 0, &entry->loadQueue, NULL);
                }
            }
            else if (!osRecvMesg(&entry->loadQueue, NULL, OS_MESG_NOBLOCK))
            {
                entry->id = id;
            }
        }

        entry++;
    }
}

PATCH_FUNC(0x8012F4FC, Object_UpdateEntriesCustom);

void Object_AfterInitContext(void)
{
    UpdateGameplayKeepForAdult(&gPlay->objectCtx);
}
