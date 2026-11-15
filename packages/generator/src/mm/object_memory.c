#include <combo.h>
#include <combo/custom.h>
#include <combo/dma.h>
#include <combo/game_state.h>

static u32 sObjectBufferBudgetsVrom __attribute__((section(".data"))) = 0;
static u32 sObjectBufferSaved;
EXPORT_SYMBOL(MM_OBJECT_BUFFER_BUDGET_V1, sObjectBufferBudgetsVrom);
EXPORT_SYMBOL(MM_OBJECT_BUFFER_SAVED_V1, sObjectBufferSaved);

u32 comboMmObjectBufferSize(TwoHeadArena* tha, u32 original)
{
    PlayState* play = (PlayState*)((u8*)tha - offsetof(GameState, tha));
    DmaEntry dma;
    u32 header[4], row[4];
    const u16 forms[] = { OBJECT_LINK_CHILD, OBJECT_LINK_BOY, OBJECT_LINK_GORON,
                          OBJECT_LINK_ZORA, OBJECT_LINK_NUTS, OBJECT_STK, CUSTOM_OBJECT_ID_MM_ADULT_LINK };
    const u16 horses[] = { OBJECT_HORSE_LINK_CHILD, CUSTOM_OBJECT_ID_MM_ADULT_EPONA };
    u32 playerSize = 0, horseSize = 0, size;

    sObjectBufferSaved = 0;
    if (gSaveContext.sceneSetupId != 0 || !sObjectBufferBudgetsVrom ||
        (u32)play->sceneId >= 113 ||
        !comboDmaLookup(&dma, sObjectBufferBudgetsVrom) || dma.vstart != sObjectBufferBudgetsVrom ||
        dma.pend || dma.vend - dma.vstart != 16 + 113 * 16 ||
        comboDmaLoadFilePartial(header, dma.vstart, 0, sizeof(header)) != sizeof(header) ||
        header[0] != 0x4f424d31 || header[1] != 1 || header[2] != 113 || header[3] != 16 ||
        comboDmaLoadFilePartial(row, dma.vstart, 16 + play->sceneId * 16, sizeof(row)) != sizeof(row) ||
        row[3] != 1 || !row[0] || row[0] >= original ||
        !row[1] || row[1] > 35 || !row[2] || row[2] > 35)
        return original;

    for (u32 i = 0; i < ARRAY_COUNT(forms); ++i)
    {
        size = comboLoadObject(NULL, forms[i]);
        if (!size || size > original)
            return original;
        playerSize = MAX(playerSize, ALIGN16(size));
    }
    for (u32 i = 0; i < ARRAY_COUNT(horses); ++i)
    {
        size = comboLoadObject(NULL, horses[i]);
        if (!size || size > original)
            return original;
        horseSize = MAX(horseSize, ALIGN16(size));
    }
    size = ALIGN16(row[0] + row[1] * playerSize + row[2] * horseSize + 0x1000);
    if (size >= original)
        return original;
    sObjectBufferSaved = original - size;
    return size;
}

void* comboMmGiBufferAlloc(u32 original)
{
    u32 capacity = 0x2000;
    for (u32 i = 0; i < GI_COUNT; ++i)
    {
        u16 id = kExtendedGetItems[i].objectId;
        if (!(id & ~MASK_FOREIGN_OBJECT))
            continue;
        u32 size = comboLoadObject(NULL, id);
        if (!size || size > original)
        {
            capacity = original;
            break;
        }
        capacity = MAX(capacity, ALIGN16(size));
    }
    return ((void* (*)(u32))0x80102c60)(capacity);
}
PATCH_CALL(0x808420c4, comboMmGiBufferAlloc);
