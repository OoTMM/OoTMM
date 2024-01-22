#include <combo.h>

void Interface_LoadItemIconCustom(u32 vrom, s32 id, void* dst, size_t size)
{
    DmaEntry dma;

    if (id < ITEM_MM_CUSTOM_MIN)
    {
        LoadIcon(vrom, id, dst, size);
    }
    else
    {
        switch(id)
        {
        case ITEM_MM_SPELL_FIRE:
            id = ITEM_OOT_SPELL_FIRE;
            break;
        case ITEM_MM_SPELL_WIND:
            id = ITEM_OOT_SPELL_WIND;
            break;
        case ITEM_MM_SPELL_LOVE:
            id = ITEM_OOT_SPELL_LOVE;
            break;
        }

        comboDmaLookupForeignId(&dma, 8);
        u32 textureFileAddress = dma.pstart;
        u32 textureOffset = 0x1000 * id;
        DMARomToRam((textureFileAddress + textureOffset) | PI_DOM1_ADDR2, dst, size);
    }
}

PATCH_CALL(0x80112BCC, Interface_LoadItemIconCustom)

extern u32 gCustomIconAddr;
extern size_t customIconSize;

u32 Interface_GetCustomIconTexture(GameState_Play* play, PauseContext* pauseCtx)
{
    u32* gItemIcons = (u32*)0x801c1e6c;
    u32 texture;
    u16 item = pauseCtx->equipTargetItem;
    if (item < ITEM_MM_CUSTOM_MIN)
    {
        texture = gItemIcons[item];
    }
    else
    {
        u8 customItem = item - ITEM_MM_CUSTOM_MIN;
        texture = gCustomIconAddr + (customIconSize * customItem);
    }
    return texture;
}

extern u8 gPlayerFormCustomItemRestrictions[5][8];

u8 Interface_GetItemRestriction(u8 playerForm, u8 item, GameState_Play* play)
{
    u8 (*gPlayerFormItemRestrictions)[0x72] = (u8(*)[0x72])0x801c2410;
    if (item < ITEM_MM_CUSTOM_MIN)
    {
        return gPlayerFormItemRestrictions[playerForm][item];
    }
    else
    {
        if (item == ITEM_MM_SPELL_WIND && play->interfaceCtx.restrictions.songOfSoaring) {
            return 0;
        }
        u8 customItem = item - ITEM_MM_CUSTOM_MIN;
        return gPlayerFormCustomItemRestrictions[playerForm][customItem];
    }
}
